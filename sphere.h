#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"

class sphere: public hitable {
    public:
        sphere() {}
        vec3 center;
        float radius;
        material *mat;
        sphere(vec3 cen, float r, material *m) : center(cen), radius(r), mat(m) {};
        virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
};

bool sphere::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {
    vec3 oc = r.origin() - center;                  // oc = A - C
    float a = dot(r.direction(), r.direction());    // a = B・B
    float b = dot(oc, r.direction());               // b = 2tB・(A - C)
    float c = dot(oc, oc) - radius * radius;        // c = (A - C)・(A - C)-R^2
    float discriminant = b * b - a * c;

    if (discriminant > 0.0) {
        float tmp = (-b - sqrt(discriminant)) / a;
        if(tmp < tmax && tmp > tmin) {
            rec.t = tmp;
            rec.p = r.point_at_param(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat = mat;
            return true;
        }
        tmp = (-b + sqrt(discriminant)) / a;
        if(tmp < tmax && tmp > tmin) {
            rec.t = tmp;
            rec.p = r.point_at_param(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat = mat;
            return true;
        }
    }
    return false;
}

#endif