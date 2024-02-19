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
        double aspect_ratio = 1.0;
        int image_width = 100;

        void render(const hittable& world){
            // Render the scene
            initialize();

            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

            for (int j=0; j < image_height; ++j){
                //progress indicator
                std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
                for (int i = 0; i < image_width; ++i) {
                    auto pixel_center = pixel100_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
                    auto ray_direction = pixel_center - center;
                    ray r(center, ray_direction);

                    color pixel_color = ray_color(r, world);
                    write_color(std::cout, pixel_color);
                } 
            }

            std::clog << "\rDone.       \n";
        };

    private:
        // Private camera parameters
        int image_height;  
        point3 center;   // Camera center
        point3 pixel100_loc; // location of pixel 0,0
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
                auto pixel100_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);                        
        }

        color ray_color(const ray& r, const hittable& world) {
            hit_record rec;
            if (world.hit(r, interval(0, infinity), rec)) {
                return 0.5*(rec.normal + color(1,1,1));
            }

            vec3 unit_direction = unit_vector(r.direction());
            auto a = 0.5*(unit_direction.y() + 1.0);
            return (1.0 - a)*color(1.0, 1.0, 1.0) + a*color(0.5,0.7,1.0);
        }
};

#endif