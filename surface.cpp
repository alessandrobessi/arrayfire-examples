/*
g++ surface.cpp -o surface -lm -lafcpu
*/

#include <arrayfire.h>
#include <math.h>
#include <cstdio>

using namespace af;

static const int M = 50;
static const int N = 2 * M;

int main()
{

    try
    {
        info();
        Window window(800, 800, "3d Surface Example");
        window.setColorMap(AF_COLORMAP_PLASMA);
        window.setAxesTitles("X", "Y", "Z");

        const array x = iota(dim4(N, 1), dim4(1, N)) / M - 1;
        const array y = iota(dim4(1, N), dim4(N, 1)) / M - 1;

        static float t = 0;

        while (!window.close())
        {
            t += 0.05;
            array z = cos(2 * y + t) + sin(2 * x + t);
            window.surface(z);
        }
    }
    catch (exception &e)
    {
        fprintf(stderr, "%s\n", e.what());
        throw;
    }
    return 0;
}