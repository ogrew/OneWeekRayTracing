#include <iostream>
#include "ray.h"

vec3 color(const ray& r) 
{
    vec3 unit_direction = unit_vector(r.direction()); // -1.0 < y < 1.0
    float t = 0.5 * (unit_direction.y() + 1.0); // 0.0 < t < 1.0

    // t = 1.0 のとき青、t = 0.0のとき白(lerp = 線形補間)
    return (1.0 - t) * vec3(1.0,1.0,1.0) + t * vec3(0.5, 0.7, 1.0);
    
}

int main()
{
    int nx = 200;
    int ny = 100;
    int max = 255;

    std::cout << "P3\n" << nx << " " << ny << "\n" << max << "\n";

    vec3 lower_left_corner(-2.0, -1.0, -1.0);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);

    for (int y = ny - 1; y >= 0; y--)
    {
        for (int x = 0; x < nx; x++)
        {
            float u = float(x) / float(nx);
            float v = float(y) / float(ny);

            ray r(origin, lower_left_corner + u * horizontal + v * vertical);
            
            vec3 col = color(r);
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);

            std::cout << ir << "  " << ig << "  " << ib << "\n";
        }
    }
}