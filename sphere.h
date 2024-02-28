#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"
#include "interval.h"

class sphere : public hittable {
    public:
<<<<<<< HEAD
        sphere(point3 _center, double _radius, shared_ptr<material> _material)
         : center(_center), radius(_radius), mat(_material) {}
=======
        sphere(point3 _center, double _radius) : center(_center), radius(_radius) {}
>>>>>>> 771f2eae743b5c1a914032651343ce16f1e72823

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            // Variables to solve quadratic equation
            vec3 oc = r.origin() - center;
            auto a = r.direction().length_squared();
            auto half_b = dot(oc, r.direction());
            auto c = oc.length_squared() - radius*radius;
            //discriminant analysis
            auto discriminant = half_b*half_b - a*c;
            if (discriminant < 0) return false;
            auto sqrtd = sqrt(discriminant);
        
            //Find the nearest root that lies in the acceptable range
            auto root = (-half_b - sqrtd) / a;
            if (!ray_t.surrounds(root)){
                root = (-half_b + sqrtd) / a;
                if (!ray_t.surrounds(root))
                    return false;
            }

            rec.t = root;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
<<<<<<< HEAD
            rec.mat = mat;
=======
>>>>>>> 771f2eae743b5c1a914032651343ce16f1e72823

            return true;

        }

    private:
        point3 center;
        double radius;
<<<<<<< HEAD
        shared_ptr<material> mat;
=======
>>>>>>> 771f2eae743b5c1a914032651343ce16f1e72823
};

#endif