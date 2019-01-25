#include <arrayfire.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <cstdio>

#define WIDTH 100
#define HEIGHT 100

using namespace af;
using std::abs;

array complex_grid(int width, int height, float zoom, float center[])
{
    // Generate sequences of length width, height
    array X = (iota(dim4(1, height), dim4(width, 1)) - (float)height / 2.0) / zoom + center[0];
    array Y = (iota(dim4(width, 1), dim4(1, height)) - (float)width / 2.0) / zoom + center[1];

    // Return the locations as a complex grid
    return complex(X, Y);
}

array mandelbrot(const array &in, int iter, float maxval)
{
    array C = in;
    array Z = C;
    array mag = constant(0, C.dims());

    for (int i = 1; i < iter; i++)
    {
        // Do the calculation
        Z = Z * Z + C;

        // Get indices where abs(Z) crosses maxval
        array cond = (abs(Z) > maxval).as(f32);
        mag = max(mag, cond * i);

        // If abs(Z) cross maxval, turn off those locations
        C = C * (1 - cond);
        Z = Z * (1 - cond);

        // Ensuring the JIT does not become too large
        eval(C, Z);
        mag.eval();
    }

    // Normalize
    return mag / maxval;
}

array normalize(array a)
{
    float mx = af::max<float>(a);
    float mn = af::min<float>(a);
    return (a - mn) / (mx - mn);
}

int main()
{
    int iter = 100;
    try
    {
        info();
        printf("** ArrayFire Fractals Demo **\n");
        Window window(WIDTH, HEIGHT, "Fractal Demo");
        window.setColorMap(AF_COLORMAP_VIRIDIS);

        float center[] = {-0.75f, 0.1f};
        // Keep zooming in for each frame
        for (int i = 10; i < 400; i++)
        {
            int zoom = i * i;
            if (!(i % 10))
            {
                printf("iteration: %d zoom: %d\n", i, zoom);
                fflush(stdout);
            }

            // Generate the grid at the current zoom factor
            array c = complex_grid(WIDTH, HEIGHT, zoom, center);

            iter = sqrt(abs(2 * sqrt(abs(1 - sqrt(5 * zoom))))) * 100;

            // Generate the mandelbrot image
            array mag = mandelbrot(c, iter, 1000);

            if (window.close())
                break;
            array mag_norm = normalize(mag);
            window.image(mag_norm);
        }
    }
    catch (exception &e)
    {
        fprintf(stderr, "%s\n", e.what());
        throw;
    }

    return 0;
}