#ifndef SPLINE_H
#define SPLINE_H

#include "func.h"

#define DX 1e-2

typedef struct {
  double x;
  double y;
}point_t;

class Grid {
public:
  Grid(double lbord, double rbord, size_t numOfPoints, double (*func)(double));

  Grid(Grid const& grid);

  void swap(Grid& a) {
    std::swap(numOfPoints, a.numOfPoints);
    std::swap(func, a.func);
    std::swap(points, a.points);
  }

  Grid& operator=(Grid const& grid) {
    if (this != &grid)
      Grid(grid).swap(*this);

    return *this;
  }

  bool PrintGrid(std::string xfilename, std::string yfilename);

  ~Grid() {
    delete[] points;
  }

  size_t numOfPoints;
  point_t* points;
  double (*func)(double);
};

class Polynom {
public:
  Polynom() {
    a = b = c = rbord.x = rbord.y = lbord.x = lbord.y = 0;
  }
  Polynom(point_t lbord, point_t rbord, double der2);
  Polynom(double derPol, point_t lbord, point_t rbord);

  double PolyVal(double x);

  double PolyDerVal(double x);

  double a;
  double b;
  double c;
  point_t rbord;
  point_t lbord;
};

class Spline {
public:
  Spline(Grid const& grid, double der2);

  double Calc(double x);

  bool PrintX(std::string const& filename);

  bool PrintY(std::string const& filename);

  bool ErrPrint(std::string const& filename);

  double MaxErrCalc();

  double MaxErrPoint();

  double MaxErrMid();

  ~Spline() {
    delete[] pols;
  }

  Polynom* pols;
  size_t size;
  double (*func)(double);
};

#endif /*SPLINE_H*/