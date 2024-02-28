#include "general.h"

#include "camera.h"
<<<<<<< HEAD
#include "color.h"
#include "hittable_list.h"
#include "material.h"
=======
#include "hittable_list.h"
>>>>>>> 771f2eae743b5c1a914032651343ce16f1e72823
#include "sphere.h"


int main() {
    
    //World
    hittable_list world;

<<<<<<< HEAD
    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<dielectric>(1.5);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),  -0.4, material_left));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));
=======
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));
>>>>>>> 771f2eae743b5c1a914032651343ce16f1e72823

    //Camera
    camera cam;

    cam.aspect_ratio = 16.0/9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

<<<<<<< HEAD
    cam.vfov = 90;
    cam.lookfrom = point3(-2,2,1);
    cam.lookat   = point3(0,0,-1);
    cam.vup      = vec3(0,1,0);
    
=======
>>>>>>> 771f2eae743b5c1a914032651343ce16f1e72823
    //Render
    cam.render(world);

}

