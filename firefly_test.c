#include <errno.h>
#include <stdio.h>
#include <math.h>

#include "firefly.h"

#define DIM 2

double minimise_this_function (double *coords)
{
    double x = coords[0];
    double y = coords[1];

    // this has clearly a minimum of 1 at x=2 and y=3
    //double z = 1 + (x+2)*(x+2) + (y+3)*(y+3);

    // this function has lots of local minima with its global minimum near x≈−6.1353,y≈−6.1887,
    // For of this function, see https://www.desmos.com/3d/xlwstpxucm
    double z = pow (sin(x)+0.35*x+2, 2) + pow (sin(y) + 0.5*y+3, 2);
    return z;
}

int main (int argc, char **argv)
{
    int    numFlies       = 25;
    int    dim            = DIM;
    double mins[DIM]      = {-20,-20};
    double maxs[DIM]      = { 20, 20};
    int    shouldMinimise = 1;

    FireflyConfig *conf = firefly_init (numFlies, dim, mins, maxs, shouldMinimise, minimise_this_function);
    for (int i=0; i<20; i++)
        firefly_optimise (conf, 10000, 2);

    double *coords, obj;
    firefly_get_bestfly_all_time (conf, & coords, & obj);
    printf ("optimisation done: lowest value %f found at x=%f y=%f", obj, coords[0], coords[1]);

    return 0;
}

