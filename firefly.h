#ifndef _FIREFLY_H_
#define _FIREFLY_H_

typedef double (*ObjectiveFun) (double *coords);

typedef struct FireflyParameters
{
    double    alpha0;
    double    alphaMin;
    double    alpha;
    double    beta0;
    double    betaMin;
    double    gamma;
    double    delta;
    int       moveBestFly;
    int       alphaUpdateInterval;
} FireflyParameters;

typedef struct FireflyState
{
    int       totalNumIterations;
    int       shouldMinimise;
    int       dim;
    int       numFlies;
    int       foundNewOptimumSinceAlphaUpdate;
    double   **curPos;
    double   **nextPos;
    double    *curObj;
    double    *mins;
    double    *maxs;
    ObjectiveFun   objectiveFun;
    FireflyParameters params;
} FireflyConfig;

void set_default_parameters (FireflyParameters *params);
double firefly_optimise (FireflyConfig *conf, int numIterations, int verbose);
FireflyConfig *firefly_init (int numFlies, int dim, double *mins, double *maxs, int shouldMinimise, ObjectiveFun objFun);
void firefly_get_bestfly_all_time (FireflyConfig *conf, double **coords, double *obj);

#endif /* _FIREFLY_H_ */
