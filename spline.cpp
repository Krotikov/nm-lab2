#include "spline.h"

Grid::Grid(double lbord, double rbord, size_t numOfPoints, double (*func)(double)) : numOfPoints(numOfPoints), func(func) {
  points = new point_t[numOfPoints];

  for (size_t i = 0; i < numOfPoints; ++i) {
    double dx = (rbord - lbord) / (numOfPoints - 1);
    points[i].x = lbord + (double)i * dx;
    points[i].y = func(points[i].x);
  }
}

Grid::Grid(Grid const& grid)
  :numOfPoints(grid.numOfPoints), func(grid.func), points(new point_t[numOfPoints]) {
  for (size_t i = 0; i < numOfPoints; ++i) {
    points[i].x = grid.points[i].x;
    points[i].y = grid.points[i].y;
  }
}


bool Grid::PrintGrid(std::string xfilename, std::string yfilename) {
  std::ofstream xout, yout;

  if (xfilename == "cout") {
    std::ostream& xout = std::cout;
  }
  else {
    xout.open(xfilename);
    if (!xout.is_open()) {
      return false;
    }
  }

  if (yfilename == "cout") {
    std::ostream& xout = std::cout;
  }
  else {
    yout.open(yfilename);
    if (!yout.is_open()) {
      return false;
    }
  }

  for (size_t i = 0; i < numOfPoints; i++) {
    xout << this->points[i].x << " ";
    yout << this->points[i].y << " ";
  }

  xout.close();
  yout.close();

  return true;
}

Polynom::Polynom(point_t lbord, point_t rbord, double der2) {
  this->lbord.x = lbord.x;
  this->lbord.y = lbord.y;
  this->rbord.x = rbord.x;
  this->rbord.y = rbord.y;
  a = 0.5 * der2;
  b = (rbord.y - lbord.y) / (rbord.x - lbord.x) - (0.5 * der2) * (rbord.x - lbord.x);
  c = lbord.y;
}

Polynom::Polynom(double derPol, point_t lbord, point_t rbord) {
  this->lbord.x = lbord.x;
  this->lbord.y = lbord.y;
  this->rbord.x = rbord.x;
  this->rbord.y = rbord.y;
  a = (rbord.y - lbord.y) / ((rbord.x - lbord.x) * (rbord.x - lbord.x)) - derPol / (rbord.x - lbord.x);
  b = derPol;
  c = lbord.y;
}
  
double Polynom::PolyVal(double x) {
  double res = a * (x - lbord.x) * (x - lbord.x) + b * (x - lbord.x) + c;
  return res;
}

double Polynom::PolyDerVal(double x) {
  double res = 2 * a * (x - lbord.x)  + b;
  return res;
}

Spline::Spline(Grid const& grid, double der2) {
  this->func = grid.func;
  this->size = grid.numOfPoints - 1;  //num of polynoms
  this->pols = new Polynom[size];  

  this->pols[0] = Polynom(grid.points[0], grid.points[1], der2);

  for (size_t i = 1; i < this->size; ++i) {
    this->pols[i] = Polynom(this->pols[i - 1].PolyDerVal(grid.points[i].x), grid.points[i], grid.points[i + 1]);
  }
}

double Spline::Calc(double x) {
  if (x < pols[0].lbord.x || x > pols[size - 1].rbord.x)
    return 0;
  
  int i = 0;
  for (; pols[i].rbord.x < x; ++i);

  return pols[i].PolyVal(x);
}

bool Spline::PrintX(std::string const& filename) {
  std::ofstream out;
  if (filename == "cout") {
    std::ostream& out = std::cout;
  }
  else {
    out.open(filename);
    if (!out.is_open()) {
      return false;
    }
  }

  double dx = DX;
  for (double i = pols[0].lbord.x; i < pols[size-1].rbord.x; i += dx) {
    out << i << " ";
  }
  out.close();
  return true;
}

bool Spline::PrintY(std::string const& filename) {
  std::ofstream out;
  if (filename == "cout") {
    std::ostream& out = std::cout;
  }
  else {
    out.open(filename);
    if (!out.is_open()) {
      return false;
    }
  }

  double dx = DX;
  for (double i = pols[0].lbord.x; i < pols[size - 1].rbord.x; i += dx) {
    out << this->Calc(i) << " ";
  }
  out.close();
  return true;
}

bool Spline::ErrPrint(std::string const& filename) {
  std::ofstream out;
  if (filename == "cout") {
    std::ostream& out = std::cout;
  }
  else {
    out.open(filename);
    if (!out.is_open()) {
      return false;
    }
  }

  double dx = DX;
  for (double i = pols[0].lbord.x; i < pols[size - 1].rbord.x; i += dx) {
    out << (this->Calc(i) - this->func(i)) << " ";
  }
  out.close();
  return true;
}

double Spline::MaxErrCalc() {
  double max = 0;
  double dx = DX;
  for (double i = pols[0].lbord.x; i < pols[size - 1].rbord.x; i += dx) {
    double tmp = fabs(this->Calc(i) - this->func(i));
    if (tmp > max)
      max = tmp;
  }

  return max;
}

double Spline::MaxErrMid() {
  double max = 0;

  for (int i = 0; i < size; ++i) {
    double tmp = (pols[i].lbord.x + pols[i].rbord.x) / 2;
    double tres = fabs(pols[i].PolyVal(tmp) - func(tmp));
    if (tres > max)
      max = tres;
  }

  return max;
}

double Spline::MaxErrPoint() {
  //because of for checks n - 1 points
  //define max with n point to check all of them
  double max = fabs(pols[size - 1].PolyVal(pols[size - 1].rbord.x) - func(pols[size - 1].rbord.x)); 

  for (int i = 0; i < size; ++i) {
    double tres = fabs(pols[i].PolyVal(pols[i].lbord.x) - func(pols[i].lbord.x));
    if (tres > max)
      max = tres;
  }

  return max;
}
