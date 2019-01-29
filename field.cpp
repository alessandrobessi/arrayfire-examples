/*
g++ field.cpp -o field -lm -lafcpu
*/

#include <arrayfire.h>
#include <cstdio>
#include <math.h>

using namespace af;

const float MINIMUM = -3.0f;
const float MAXIMUM = 3.0f;
const float STEP = 0.3f;

int main(int argc, char *argv[])
{
    try
    {
        af::info();
        af::Window window(1024, 512, "2D Vector Field");
        
        window.grid(1, 2);
        array range = seq(MINIMUM, MAXIMUM, STEP);
        array x = tile(range, 1, range.dims(0));
        array y = tile(range.T(), range.dims(0), 1);

        float scale = 5.0f;
        while (!window.close())
        {
            array points = join(1, flat(x), flat(y));
            array saddle = join(1, flat(x), -1.0f * flat(y));
            array bvals = cos(scale * (x * x + y * y));
            array bowl = join(1, constant(1, x.elements()), flat(bvals));

            window(0, 0).vectorField(points, saddle, "Saddle point");
            window(0, 1).vectorField(points, bowl, "Hilly bowl");
            window.show();
            
            scale -= 0.05f;
            if (scale < -5.0f)
                scale = 5.0f;
        }
    }
    catch (af::exception &e)
    {
        fprintf(stderr, "%s\n", e.what());
        throw;
    }
    return 0;
}