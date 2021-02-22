#ifndef TEST_H
#define TEST_H

#include "func.h"
#include "spline.h"

#define LBORD -2
#define RBORD 2
#define MAX_NODS 60
#define MIN_NODS 2
#define MIN_DER -50
#define MAX_DER 50

void TestNumNodsInPoints(double(*func)(double), double(*der2)(double));

void TestNumNods(double(*func)(double), double(*der2)(double));
/*
return 3 files
1 - with grid x coor
2 - with grid y coor
3 - with spline x coor //not sure
4 - with spline value
5 - with err value : S(x) - f(x)
*/

void TestEFNN(double(*func)(double), double(*der2)(double));
//EFNN - error from num nods
//interval: [2, 100]
//dx = 1

void TestEFDV(double(*func)(double), double(*der2)(double));
//EFDV = error from der value
//der2 exact == 2 + 4sin(2) ~= 5.64
//interval : [-50, 50]
//dx = 1e-2

#endif // !TEST_H