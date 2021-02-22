#include "func.h"

double Func(double x) {
  double res = x * x + 4 * sin(x) - 2;
  return res;
}

double Der1(double x) {
  double res = 2 * x + 4 * cos(x);
  return res;
}

double Der2(double x) {
  double res = 2 - 4 * sin(x);
  return res;
}

