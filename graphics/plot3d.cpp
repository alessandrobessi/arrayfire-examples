/*
g++ plot3d.cpp -o plot3d -lm -lafcpu
*/

#include <arrayfire.h>
#include <cstdio>
#include <math.h>

using namespace af;

const int ITERATIONS = 200;
const float PRECISION = 1.0f / ITERATIONS;

int main(int argc, char **argv)
{
    try
    {
        af::info();
        af::Window window(800, 800, "3D Line");
        window.setAxesTitles("X", "Y", "Z");

        float t = 0.1;
        array Z = seq(0.1f, 10.f, PRECISION);
        while (!window.close())
        {
            array Y = sin((Z * t) + t) / Z;
            array X = cos((Z * t) + t) / Z;

            X = max(min(X, 1.0), -1.0);
            Y = max(min(Y, 1.0), -1.0);

            window.plot(X, Y, Z);
            t += 0.05;
        }
    }
    catch (af::exception &e)
    {
        fprintf(stderr, "%s\n", e.what());
        throw;
    }
    return 0;
}