#ifndef QUAD_H
#define QUAD_H

#include <cmath>

#include "general.h"

#include "hittable.h"

class quad : public hittable {
    public:
        quad(const point3& _Q, const vec3& _u, const vec3& _v,  shared_ptr<material> m)
            : Q(_Q), u(_u), v(_v), mat(m) {
                auto n = cross(u, v);
                normal = unit_vector(n);
                D = dot(normal, Q);
                w = n / dot(n, n);
                
                set_bounding_box();
        }

        virtual void set_bounding_box() {
            bbox = aabb(Q, Q + u + v).pad();
        }
        
        aabb bounding_box() const override { return bbox; }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            auto denom = dot(normal, r.direction());

            // no hit if the ray is parallel to the plane
            if (fabs(denom) < 1e-8)
                return false;

            // false if the hit point param t is outside the ray interval
            auto t = (D - dot(normal, r.origin())) / denom;
            if (!ray_t.contains(t))
                return false;

            // determine if hit point is inside the 2d shape
            auto intersection = r.at(t);
            vec3 planar_hitpt_vector = intersection - Q;
            auto alpha = dot(w, cross(planar_hitpt_vector, v));
            auto beta = dot(w, cross(u, planar_hitpt_vector));

            if (!is_interior(alpha, beta, rec))
                return false;

            // ray hits the 2d shape; set the hit record and return true
            rec.t = t;
            rec.p = intersection;
            rec.mat = mat;
            rec.set_face_normal(r, normal);

                return true;
        }

        virtual bool is_interior(double a, double b, hit_record& rec) const{
            // Given hit point coordinates, return false if outside or true if inside
            // and set hit record to UV coord

            if ((a < 0) || (a > 1) || (b < 0) || (b > 1))
                return false;
            
            rec.u = a;
            rec.v = b;
            return true;
        }

        private:
            point3 Q;
            vec3 u, v;
            shared_ptr<material> mat;
            aabb bbox;
            vec3 normal;
            double D;
            vec3 w;

};

#endif