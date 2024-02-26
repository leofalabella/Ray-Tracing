#ifndef CAMERA_H
#define CAMERA_H

#include "general.h"

#include "color.h"
#include "hittable.h"

#include <iostream>

/* Class responsible for 
    1) construct and dispatch rays 
    2) use the results to render image */

class camera {
    public:
        // Public camera parameters
            //image
        double aspect_ratio = 1.0;     // width/height
        int    image_width = 100;      // width in pixels
        int    samples_per_pixel = 10; // count of random samples for each pixel
        int max_depth = 10;            // maximum number of bounces for a ray
        
        void render(const hittable& world){
            // Render the scene
            initialize();

            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

            for (int j=0; j < image_height; ++j){
                //progress indicator
                std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
                for (int i = 0; i < image_width; ++i) {
                    color pixel_color(0,0,0);
                    for (int sample = 0; sample < samples_per_pixel; ++sample){
                        ray r = get_ray(i, j);
                        pixel_color += ray_color(r, max_depth, world);
                    }
                    write_color(std::cout, pixel_color, samples_per_pixel);
                } 
            }

            std::clog << "\rDone.       \n";
        };

    private:
        // Private camera parameters
        int image_height;  
        point3 center;   // Camera center
        point3 pixel00_loc; // location of pixel 0,0
        vec3 pixel_delta_u; // offset to pixel to the right
        vec3 pixel_delta_v; // offset to pixel below

        void initialize(){
                // Calculate the image height, and ensure it's at least 1
                int image_height = static_cast<int>(image_width / aspect_ratio);
                image_height = (image_height < 1) ? 1 : image_height;

                center = point3(0,0,0);
                
                // Viewport dimensions
                auto focal_length = 1.0;
                auto viewport_height = 2.0;
                auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);
        
                // Calculate the vector across the horizontal and down the vertical viewport edges
                auto viewport_u = vec3(viewport_width, 0,0);
                auto viewport_v = vec3(0, -viewport_height, 0);

                // Horizontal and vertical delta vectors from pixel to pixel
                auto pixel_delta_u = viewport_u / image_width;
                auto pixel_delta_v = viewport_v / image_height;

                // Location of the upper left pixel
                auto viewport_upper_left = center - vec3(0,0,focal_length)
                                        -  viewport_u/2 - viewport_v/2;
                auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);                        
        }

        color ray_color(const ray& r, int depth, const hittable& world) {
            hit_record rec;

            // If we've exceeded the ray bounce limit, no more light is gathered.
            if (depth <= 0)
                return color(0,0,0);

            if (world.hit(r, interval(0.001, infinity), rec)) {
                vec3 direction = rec.normal + random_unit_vector();
                return 0.5 * ray_color(ray(rec.p, direction), depth-1, world);
            }

            vec3 unit_direction = unit_vector(r.direction());
            auto a = 0.5*(unit_direction.y() + 1.0);
            return (1.0 - a)*color(1.0, 1.0, 1.0) + a*color(0.5,0.7,1.0);
        }

        ray get_ray(int i, int j) const {
            // Get a randomly sampled camera ray for the pixel at location i,j
            
            auto pixel_center = pixel00_loc + i*pixel_delta_u + j*pixel_delta_v;
            auto pixel_sample = pixel_center + pixel_sample_square();

            auto ray_origin = center;
            auto ray_direction = pixel_sample - ray_origin;

            return ray(ray_origin, ray_direction);
        }

        vec3 pixel_sample_square() const {
            // Return a random point in the square surrounding a pixel at the origin
            auto px = -0.5 + random_double();
            auto py = -0.5 + random_double();
            return (px * pixel_delta_u) + (py * pixel_delta_v);
        }
};

#endif