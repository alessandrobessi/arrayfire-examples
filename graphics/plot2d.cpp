/*
g++ plot2d.cpp -o plot2d -lm -lafcpu
*/

#include <arrayfire.h>
#include <cstdio>
#include <math.h>

using namespace af;

const int ITERATIONS = 100;
const float PRECISION = 1.0f / ITERATIONS;

int main(int argc, char *argv[])
{
    try
    {
        af::info();
        af::Window window(1024, 512, "2D Plot example");

        array Y;
        int sign = 1;
        array X = seq(-Pi, Pi, PRECISION);
        array noise = randn(X.dims(0)) / 5.f;

        window.grid(2, 1);

        float xbuf[] = {-5, 5};
        float ybuf[] = {-2, 2};
        const array xaxis(2, xbuf);
        const array yaxis(2, ybuf);

        window(0, 0).setAxesLimits(xaxis, yaxis, true);
        window(1, 0).setAxesLimits(xaxis, yaxis, true);

        double val = 0;
        while (!window.close())
        {
            Y = sin(X);
            window(0, 0).plot(X, Y);
            window(1, 0).scatter(X, Y + noise, AF_MARKER_POINT);
            window.show();
            X = X + PRECISION * float(sign);
            val += PRECISION * float(sign);
            if (val > af::Pi)
            {
                sign = -1;
            }
            else if (val < -af::Pi)
            {
                sign = 1;
            }
        }
    }
    catch (af::exception &e)
    {
        fprintf(stderr, "%s\n", e.what());
        throw;
    }
    return 0;
}