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

hitable *random_scene() {
    int n = 500;
    hitable **list = new hitable*[n+1];
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));

    int i = 1;
    int space = 11;
    for(int a = -space; a < space; a++) {
        for(int b = -space; b < space; b++) {
            float choose_mat = random_double();
            vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if((center - vec3(4, 0.2, 0)).length() > 0.9) {
                if(choose_mat < 0.75) { // diffuse
                    list[i++] = new sphere(
                        center, 
                        0.2,
                        new lambertian(vec3(random_double() * random_double(), random_double() * random_double(), random_double() * random_double()))
                    );
                } else if (choose_mat < 0.95) { // metal
                    list[i++] = new sphere(
                        center, 
                        0.25,
                        new metal(vec3(0.5 *(1 + random_double()), 0.5 *(1 + random_double()), 0.5 *(1 + random_double())), 0.5 * random_double())
                    );
                } else { // glass
                    list[i++] = new sphere(
                        center,
                        0.3,
                        new dielectric(1.5)
                    );
                }
            }
        }
    }

    list[i++] = new sphere(vec3(0, 1.2, 0),   1.2, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0),    1.0, new lambertian(vec3(0.8, 0.3, 0.2)));
    list[i++] = new sphere(vec3(4, 1, 0),     1.0, new metal(vec3(0.3, 0.4, 0.9), 0.1));

    return new hitable_list(list, i);
}

int main()
{
    int nx = 1280;
    int ny = 720;
    int ns = 100;
    int max = 255;

    std::cout << "P3\n" << nx << " " << ny << "\n" << max << "\n";
    
    hitable *world = random_scene();

    // cam(look_from, look_at, v_up, fov, aspect)
    vec3 look_from(13, 2, 3);
    vec3 look_at(-1, 0, 0);
    float dist2focus = (look_from - look_at).length();
    float aperture = 0.18;
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