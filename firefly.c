#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "firefly.h"

#define rnorm(sigma) gsl_ran_gaussian_ziggurat (r_global, sigma)
#define runif(a,b) ((a) + drand48() * ((b)-(a)))

#define STDFS stderr

static double *heavy_tail_ncube (int dim)
{
    static double *xs = NULL;
    static int allocDim = 0;
    if (allocDim < dim)
    {
        if (xs) free (xs);
        xs = malloc (sizeof (xs[0]) * dim);
        assert (xs);
        allocDim = dim;
    }

    // tan (M_PI * x) for x = 0, 0.1, 0.2, 0.3, 0.4, 0.5 is
    //                        0, 0.3, 0.7, 1.4, 3.1, inf =>
    // limit x value to 0.49 => tan (M_PI * 0.49) = 32

    double length = tan (M_PI * runif (0, 0.49));

    double sum = 0.0;
    for (int i=0; i<dim; i++)
    {
        xs[i] = runif (-1,1);
        sum += xs[i] * xs[i];
    }
    double radius = sqrt (sum);

    for (int i=0; i<dim; i++)
        xs[i] = length * (xs[i] / radius);

    return xs;
}

static double normalised_distance_squared (double *coords1, double *coords2, double *mins, double *maxs, int dim)
{
    double sum = 0.0;
    for (int i=0; i<dim; i++)
    {
        double dist = (coords1[i] - coords2[i]) / (maxs[i] - mins[i]);
        sum += dist * dist;
    }
    return sum;
}

void firefly_get_bestfly_all_time (FireflyConfig *conf, double **coords, double *obj)
{
    *coords = conf->curPos[conf->dim];
    *obj    = conf->curObj[conf->dim];
}

static void move_flies (FireflyConfig* conf)
{
    int dim               = conf->dim;
    int numFlies          = conf->numFlies;
    int shouldMinimise    = conf->shouldMinimise;

    double   *curObj      = conf->curObj;
    double  **curPos      = conf->curPos;
    double  **nextPos     = conf->nextPos;
    double   *mins        = conf->mins;
    double   *maxs        = conf->maxs;

    int       moveBestFly = conf->params.moveBestFly;
    double    alpha       = conf->params.alpha;
    double    beta0       = conf->params.beta0;
    double    betaMin     = conf->params.betaMin;
    double    gamma       = conf->params.gamma;

    ObjectiveFun objectiveFun = conf->objectiveFun;

    for (int fi0=0; fi0<numFlies; fi0++)
    {
        int moved = 0;
        double thisFlyObj = curObj[fi0];
        for (int fi1=0; fi1<numFlies; fi1++)
        {
            double otherFlyObj = curObj[fi1];
            if (shouldMinimise ? thisFlyObj > otherFlyObj : thisFlyObj < otherFlyObj)
            {
                double  normDist = normalised_distance_squared (curPos[fi0], curPos[fi1], mins, maxs, dim);
                double *rval     = heavy_tail_ncube (dim);

                for (int k=0; k<dim; k++)
                {
                    double xmin = mins[k];
                    double xmax = maxs[k];
                    double scale = xmax - xmin;

                    double dx = curPos[fi1][k] - curPos[fi0][k];
                    double beta = beta0 * exp (-gamma * normDist);
                    if (beta < betaMin)
                        beta = betaMin;
                    double delta = (beta * dx) + scale * rval[k] * alpha;

                    double x = curPos[fi0][k] + delta;
                    nextPos[fi0][k] = (x < xmin) ? xmin : (x > xmax) ? xmax : x;
                }
                moved = 1;
            }
        }

        if (!moved && moveBestFly)
        {
            double *rval = heavy_tail_ncube (dim);

            for (int k=0; k<dim; k++)
            {
                double xmin = mins[k];
                double xmax = maxs[k];
                double scale = xmax - xmin;
                double delta = scale * rval[k] * alpha;
                double x = curPos[fi0][k] + delta;
                nextPos[fi0][k] = (x < xmin) ? xmin : (x > xmax) ? xmax : x;
            }
        }

    }

    for (int fi=0; fi<numFlies; fi++)
    {
        for (int k=0; k<dim; k++)
            curPos[fi][k] = nextPos[fi][k];

        curObj[fi] = objectiveFun (curPos[fi]);
    }
}

int firefly_get_best_index (FireflyConfig *conf)
{
    int bestfly = 0;
    double *obj = conf->curObj;
    for (int k=0; k<conf->numFlies; k++)
        if (conf->shouldMinimise ? obj[k] < obj[bestfly] : obj[k] > obj[bestfly])
            bestfly = k;
    return bestfly;
}

double firefly_optimise (FireflyConfig *conf, int numIterations, int verbose)
{
    int bestfly = firefly_get_best_index (conf);
    double **curPos = conf->curPos;
    double  *curObj = conf->curObj;

    if (verbose > 1)
    {
        fprintf (STDFS, "iter:%7d alpha:%0.2e obj:%0.5e pos:",
                 conf->totalNumIterations, conf->params.alpha, curObj[bestfly]);
        for (int k=0; k<conf->dim; k++)
            fprintf (STDFS, "%s%8.4lf", (k?",":""), curPos[bestfly][k]);
        fprintf (STDFS, "\n");
    }

    int bestflyAllTime = conf->numFlies;
    int foundNewOptimum = 0;

    for (int iter=0; iter<numIterations; iter++)
    {
        move_flies (conf);

        bestfly = firefly_get_best_index (conf);

        if (conf->shouldMinimise ? curObj[bestflyAllTime] > curObj[bestfly] : curObj[bestflyAllTime] < curObj[bestfly])
        {
            foundNewOptimum = 1;
            conf->foundNewOptimumSinceAlphaUpdate = 1;
            curObj[bestflyAllTime] = curObj[bestfly];
            memcpy (curPos[bestflyAllTime], curPos[bestfly], sizeof (curPos[0][0]) * conf->dim);
        }

        conf->totalNumIterations++;
        if (conf->totalNumIterations % conf->params.alphaUpdateInterval == 0)
        {
            if (!conf->foundNewOptimumSinceAlphaUpdate)
                conf->params.alpha *= conf->params.delta;
            conf->foundNewOptimumSinceAlphaUpdate = 0;
        }
    }

    if (foundNewOptimum)
    {
        if (verbose > 0)
        {
            fprintf (STDFS, "                            obj:%0.5e pos:", curObj[bestflyAllTime]);
            for (int k=0; k<conf->dim; k++)
                fprintf (STDFS, "%s%8.4lf", (k?",":""), curPos[bestflyAllTime][k]);
            fprintf (STDFS, " highscore!\n");
        }
    }

    return conf->curObj[bestflyAllTime];
}

void set_default_parameters (FireflyParameters *params)
{
    bzero (params, sizeof (*params));

    params->moveBestFly = 1;
    params->alpha0      = 0.1;
    params->alphaMin    = 4e-5;
    params->alpha       = params->alpha0;
    params->beta0       = 1.0;
    params->betaMin     = 0.01;
    params->gamma       = 1;
    params->delta       = 0.98;
    params->alphaUpdateInterval = 100;
}

static inline void *safe_calloc (size_t count, size_t size)
{
    void *ptr = calloc (count, size);
    assert (ptr);
    return ptr;
}

FireflyConfig *firefly_init (int numFlies, int dim, double *mins, double *maxs, int shouldMinimise, ObjectiveFun objFun)
{
    srand48 (time (NULL));

    FireflyConfig* conf = safe_calloc (1, sizeof (*conf));

    set_default_parameters (& conf->params);

    conf->numFlies           = numFlies;
    conf->dim                = dim;
    conf->shouldMinimise     = shouldMinimise;
    conf->objectiveFun       = objFun;
    conf->totalNumIterations = 0;
    conf->foundNewOptimumSinceAlphaUpdate = 0;

    // allocate an additional fly to save the best one over all iterations
    conf->curPos  = safe_calloc (conf->numFlies+1, sizeof (conf->curPos[0]));
    conf->nextPos = safe_calloc (conf->numFlies+1, sizeof (conf->nextPos[0]));
    conf->curObj  = safe_calloc (conf->numFlies+1, sizeof (conf->curObj[0]));

    for (int fi=0; fi<numFlies+1; fi++)
    {
        conf->curPos[fi]  = safe_calloc (dim, sizeof (conf->curPos[0][0]));
        conf->nextPos[fi] = safe_calloc (dim, sizeof (conf->nextPos[0][0]));
    }

    conf->mins = safe_calloc (dim, sizeof (mins[0]));
    conf->maxs = safe_calloc (dim, sizeof (maxs[0]));

    memcpy (conf->mins, mins, dim * sizeof (mins[0]));
    memcpy (conf->maxs, maxs, dim * sizeof (maxs[0]));

    for (int fi=0; fi<conf->numFlies+1; fi++)
    {
        for (int k=0; k<conf->dim; k++)
            conf->curPos[fi][k] = runif (conf->mins[k], conf->maxs[k]);
        conf->curObj[fi] = conf->objectiveFun (conf->curPos[fi]);
    }

    return conf;
}
