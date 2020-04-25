#include <iostream>

int main()
{
    int nx = 100;
    int ny = 100;
    int max = 255;

    std::cout << "P3\n" << nx << " " << ny << "\n" << max << "\n";

    for (int x = 1; x <= nx; x++)
    {
        for (int y = 1; y <= ny; y++)
        {
            int r = max * int(x) / int(nx);
            int g = max * int(y) / int(ny);
            int b = 50;

            std::cout << r << "  " << g << "  " << b << "\n";
        }
    }
}