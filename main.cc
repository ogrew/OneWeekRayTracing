#include <iostream>
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
#include "material.h"
#include "random.h"

vec3 color(const ray& r, hitable *world, int depth) 
{
    hit_record rec;
    if (world->hit(r, 0.001, FLT_MAX, rec)) {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.mat->scatter(r, rec, attenuation, scattered)) {
             return attenuation*color(scattered, world, depth+1);
        }
        else {
            return vec3(0,0,0);
        }
    } else {
        vec3 unit_direction = unit_vector(r.direction()); // -1.0 < y < 1.0
        float t = 0.5 * (unit_direction.y() + 1.0); // 0.0 < t < 1.0
        // t = 1.0 のとき青、t = 0.0のとき白(lerp = 線形補間)
        return (1.0 - t) * vec3(1.0,1.0,1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

int main()
{
    int nx = 600;
    int ny = 300;
    int ns = 100;
    int max = 255;

    std::cout << "P3\n" << nx << " " << ny << "\n" << max << "\n";

    hitable *list[5];
    list[0] = new sphere(vec3(0, 0, -1),        0.5, new lambertian(vec3(0.9, 0.2, 0.3)));
    list[1] = new sphere(vec3(0, -100.5, -1),   100, new lambertian(vec3(0.1, 0.8, 0.4)));
    list[2] = new sphere(vec3(1, 0, -1),        0.5, new metal(vec3(0.2, 0.6, 0.9), 0.7));
    list[3] = new sphere(vec3(-1, 0, -1),       0.5, new dielectric(1.5));
    list[4] = new sphere(vec3(-1, 0, -1),     -0.45, new dielectric(1.5));
    
    hitable *world = new hitable_list(list, 5);

    // cam(look_from, look_at, v_up, fov, aspect)
    vec3 look_from(3, 3, 2);
    vec3 look_at(0, 0, -1);
    float dist2focus = (look_from - look_at).length();
    float aperture = 1.0;
    camera cam(look_from, look_at, vec3(0, 1.0, 0), 20, float(nx) / float(ny), aperture, dist2focus);

    for (int y = ny - 1; y >= 0; y--) {
        for (int x = 0; x < nx; x++) {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                float u = float(x + random_double()) / float(nx);
                float v = float(y + random_double()) / float(ny);

                ray r = cam.get_ray(u, v);
                col += color(r, world, 0);
            }

            col /= float(ns);

            // ガンマ補正（ガンマ値 = 2）
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);

            std::cout << ir << "  " << ig << "  " << ib << "\n";
        }
    }
}