#ifndef CAMERA_H
#define CAMERA_H

#include "general.h"

#include "color.h"
#include "hittable.h"
#include "material.h"

#include <iostream>

/* Class responsible for 
    1) construct and dispatch rays 
    2) use the results to render image */

class camera {
    public:
        // Public camera parameters
        //image
        int    image_width = 100;      // width in pixels
        int    image_height = 100;     // height in pixels
        int    samples_per_pixel = 10; // count of random samples for each pixel
        int    max_depth = 10;            // maximum number of bounces for a ray
        color background;
        
        double vfov = 90; // Vertical field of view in degrees
        point3 lookfrom = point3(0,0,-1);  // Point camera is looking from
        point3 lookat   = point3(0,0,0);   // Point camera is looking at
        vec3   vup      = vec3(0,1,0);     // Camera-relative "up" direction

        double defocus_angle = 0;  // Variation angle of rays through each pixel
        double focus_dist = 10;    // Distance from camera lookfrom point to plane of perfect focus


        void render(const hittable& world, Pixels& pixels, sf::RenderWindow& window, sf::Sprite& sprite, sf::Texture& tex) {
            initialize();

            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

            for (int j = 0; j < image_height; ++j) {
                std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
                for (int i = 0; i < image_width; ++i) {
                    color pixel_color(0,0,0);
                    color pixel_color_sum = color(0,0,0);
                    for (int sample = 0; sample < samples_per_pixel; ++sample) {
                        ray r = get_ray(i, j);
                        pixel_color = ray_color(r, max_depth, world);
                        pixels.accumulate(i, j, pixel_color);
                        pixel_color_sum += pixel_color; 
                    }
                    write_color(std::cout, pixel_color_sum, samples_per_pixel);
                }
                tex.update(pixels.get_pixels(image_width, image_height));
                window.clear();
                window.draw(sprite);
                window.display();
            }
            std::clog << "\rDone.                 \n";
        }

    private:
        // Private camera parameters  
        point3 center;   // Camera center
        point3 pixel00_loc; // location of pixel 0,0
        vec3 pixel_delta_u; // offset to pixel to the right
        vec3 pixel_delta_v; // offset to pixel below
        vec3 u, v, w; // Camera coordinate system
        vec3   defocus_disk_u;  // Defocus disk horizontal radius
        vec3   defocus_disk_v;  // Defocus disk vertical radius

        void initialize(){
            center = lookfrom;
            
            // Viewport dimensions
            auto theta = degrees_to_radians(vfov);
            auto h = tan(theta/2);
            auto viewport_height = 2 * h * focus_dist;
            auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);

            // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            // Calculate the vector across the horizontal and down the vertical viewport edges
            vec3 viewport_u = viewport_width * u;    // Vector across viewport horizontal edge
            vec3 viewport_v = viewport_height * -v;  // Vector down viewport vertical edge

            // Horizontal and vertical delta vectors from pixel to pixel
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            // Location of the upper left pixel
            auto viewport_upper_left = center - (focus_dist * w) - viewport_u/2 - viewport_v/2;
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v); 

            // Calculate the camera defocus disk basis vectors.
            auto defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle / 2));
            defocus_disk_u = u * defocus_radius;
            defocus_disk_v = v * defocus_radius;                       
        }

        ray get_ray(int i, int j) const {
            // Get a randomly sampled camera ray for the pixel at location i,j.

            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto pixel_sample = pixel_center + pixel_sample_square();

            auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
            auto ray_direction = pixel_sample - ray_origin;

            return ray(ray_origin, ray_direction);
        }

        vec3 pixel_sample_square() const {
            // Returns a random point in the square surrounding a pixel at the origin.
            auto px = -0.5 + random_double();
            auto py = -0.5 + random_double();
            return (px * pixel_delta_u) + (py * pixel_delta_v);
        }

        point3 defocus_disk_sample() const {
        // Returns a random point in the camera defocus disk.
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }
    
    color ray_color(const ray& r, int depth, const hittable& world) const {
        hit_record rec;

        // If we've exceeded the ray bounce limit, no more light is  gathered.
        if (depth <= 0)
            return color(0,0,0);

        // If the ray hits nothing, return the background color.
        if (!world.hit(r, interval(0.001, infinity), rec))
            return background;

        ray scattered;
        color attenuation;
        color color_from_emission = rec.mat->emitted(rec.u, rec.v, rec.p);

        if (!rec.mat->scatter(r, rec, attenuation, scattered))
            return color_from_emission;

        color color_from_scatter = attenuation * ray_color(scattered, depth-1, world);
        
        return color_from_emission + color_from_scatter;
    }
};

#endif