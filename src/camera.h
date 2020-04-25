#ifndef CAMERAH
#define CAMERAH

#include "ray.h"
#include "random.h"

vec3 random_in_unit_disk() {
    vec3 p;
    do {
        p = 2.0 * vec3(random_double(), random_double(), 0) - vec3(1, 1, 0);
    } while (dot(p, p) >= 1.0);
    return p;
}

class camera {
    public:
        vec3 origin;
        vec3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
        vec3 u, v, w;
        float lens_radius;
        camera(vec3 look_from, vec3 look_at, vec3 v_up, float fov, float aspect, float aperture, float focus_dist) {
            lens_radius = aperture / 2; 

            float theta = fov * M_PI / 180; // fov: 視野角(度->ラジアン) 
            float half_height = tan(theta / 2);
            float half_width = aspect * half_height;

            origin = look_from;

            // (u, v, w) の正規直交基底
            w = unit_vector(look_from - look_at);
            u = unit_vector(cross(v_up, w)); 
            v = cross(w, u);

            lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
            horizontal = 2.0 * half_width * focus_dist * u;
            vertical = 2.0 * half_height * focus_dist * v;            
        }
        ray get_ray(float s, float t) {
            vec3 rd = lens_radius * random_in_unit_disk();
            vec3 offset = u * rd.x() + v * rd.y();
            return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
        }
};

#endif