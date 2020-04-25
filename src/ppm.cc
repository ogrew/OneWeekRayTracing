#include <iostream>
#include "vec3.h"

// 第二章まで

int main()
{
    int nx = 200;
    int ny = 100;
    int max = 255;

    std::cout << "P3\n" << nx << " " << ny << "\n" << max << "\n";

    for (int y = ny - 1; y >= 0; y--)
    {
        for (int x = 0; x < nx; x++)
        {
            vec3 col(float(x) / float(nx), float(y) / float(ny), 0.5);
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);

            std::cout << ir << "  " << ig << "  " << ib << "\n";
        }
    }
}