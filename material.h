#ifndef MATERIALH
#define MATERIALH

#include "ray.h"
#include "hitable.h"
#include "random.h"

float schlick(float cosine, float ref_idx) {
    float r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1.0 - r0) * pow((1.0 - cosine), 5.0);
}

// attenuation -> 減衰
// scattered -> 散乱
class material {
    public:
        virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

// 【屈折】v:入射ray, n:法線(単位ベクトル), ni_over_nt:, refracted:
bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) {
    vec3 unit_v = unit_vector(v);
    float dt = dot(unit_v, n);
    float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1.0 - dt * dt);
    if(discriminant > 0) {
        refracted = ni_over_nt * (unit_v - n * dt) - n * sqrt(discriminant);
        return true;
    } else {
        return false;
    }
}

// 【反射】 v:入射ray, n:法線(単位ベクトル), 反射ray = v + 2 (v・n)*n
vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v, n) * n;
}

vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        p = 2.0 * vec3(random_double(), random_double(), random_double()) - vec3(1, 1, 1); // 各要素(-2 ~ 0)
    } while (p.squared_length() > 1.0);

    return p;
}

class dielectric : public material {
    public:
        float ref_idx;
        dielectric(float ri) : ref_idx(ri) {}
        virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
            vec3 outward_normal;
            vec3 reflected = reflect(r_in.direction(), rec.normal);
            float ni_over_nt;
            attenuation = vec3(1.0, 1.0, 1.0);
            vec3 refracted;

            float reflect_prob;
            float cosine;
            
            if(dot(r_in.direction(), rec.normal) > 0) {
                outward_normal = -rec.normal;
                ni_over_nt = ref_idx;
                cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
            } else {
                outward_normal = rec.normal;
                ni_over_nt = 1.0 / ref_idx;
                cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
            }

            if(refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
                reflect_prob = schlick(cosine, ref_idx);
            } else {
                reflect_prob = 1.0;
            }

            if(random_double() < reflect_prob) {
                scattered = ray(rec.p, reflected);
            } else {
                scattered = ray(rec.p, refracted);
            }
            return true;
        }
};

class lambertian : public material {
    public:
        vec3 albedo;
        lambertian(const vec3& a) : albedo(a) {}
        virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
             vec3 target = rec.p + rec.normal + random_in_unit_sphere();
             scattered = ray(rec.p, target-rec.p);
             attenuation = albedo;
             return true;
        }    
};

class metal : public material {
    public:
        metal(const vec3& a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; } // fuzziness: 反射のばらつき（不鮮明さ） = 1.0で完全鏡面反射
        virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const  {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }
        vec3 albedo;
        float fuzz;
};

#endif