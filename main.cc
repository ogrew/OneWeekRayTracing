#include <iostream>
#include <random>
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"

vec3 color(const ray& r, hitable *world) 
{
    hit_record rec;
    if(world->hit(r, 0.0, FLT_MAX, rec)) {
        return 0.5 * vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
    } else {
        vec3 unit_direction = unit_vector(r.direction()); // -1.0 < y < 1.0
        float t = 0.5 * (unit_direction.y() + 1.0); // 0.0 < t < 1.0
        // t = 1.0 のとき青、t = 0.0のとき白(lerp = 線形補間)
        return (1.0 - t) * vec3(1.0,1.0,1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

int main()
{
    int nx = 200;
    int ny = 100;
    int ns = 100;
    int max = 255;

    std::cout << "P3\n" << nx << " " << ny << "\n" << max << "\n";

    hitable *list[2];
    list[0] = new sphere(vec3(0, 0, -1), 0.5);
    list[1] = new sphere(vec3(0, -100.5, -1), 100);
    hitable *world = new hitable_list(list, 2);

    camera cam;

    std::random_device d;
    std::uniform_real_distribution<double> rnd(0.0,1.0);

    for (int y = ny - 1; y >= 0; y--) {
        for (int x = 0; x < nx; x++) {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                float u = float(x + rnd(d)) / float(nx);
                float v = float(y + rnd(d)) / float(ny);

                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_param(2.0);
                col += color(r, world);
            }

            col /= float(ns);

            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);

            std::cout << ir << "  " << ig << "  " << ib << "\n";
        }
    }
}