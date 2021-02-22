#include "test.h"

std::string FileName(std::string str, int symb, double(*func)(double)) {
  char sym = symb + '0';
  std::string tmp = str + sym + ".txt";
  std::string res;
  std::string beg;
  if (func == &Func)
    beg = "Func/";
  else
    beg = "noname/";
  res = beg + tmp;

  return res;
}

void TestNumNodsInPoints(double(*func)(double), double(*der2)(double)) {
  size_t numNods[] = { 3, 5, 7 };
  double rbord = RBORD;
  double lbord = LBORD;

  for (size_t i = 0; i < 3; ++i) {

    Grid grid(lbord, rbord, numNods[i], func);
    Spline spline(grid, der2(lbord));

    std::cout << "For " << numNods[i] << " points: " << std::endl;
    std::cout << "Max err in nods: " << spline.MaxErrPoint() << std::endl;
    std::cout << "Max err in mids: " << spline.MaxErrMid() << std::endl;
  }

  return;
}


void TestNumNods(double(*func)(double), double(*der2)(double)) {
  size_t numNods[] = { 3, 5, 7 };
  double rbord = RBORD;
  double lbord = LBORD;
  size_t size = (sizeof(numNods) / sizeof(size_t));

  for (int i = 0; i < size; ++i) {

    Grid grid(lbord, rbord, numNods[i], func);
    grid.PrintGrid(FileName("xpoints", (int)numNods[i], func), FileName("ypoints", (int)numNods[i], func));

    Spline spline(grid, der2(lbord));
    spline.PrintX(FileName("testX", (int)numNods[i], func));
    spline.PrintY(FileName("testY", (int)numNods[i], func));
    spline.ErrPrint(FileName("err", (int)numNods[i], func));
  }

  return;
}

void TestEFNN(double(*func)(double), double(*der2)(double)) {
  double rbord = RBORD;
  double lbord = LBORD;
  size_t maxNods = MAX_NODS;
  std::ofstream xout, yout;

  xout.open("Func/numNods.txt");
  yout.open("Func/maxErr.txt");

  for (size_t i = MIN_NODS; i < maxNods; ++i) {
    Grid grid(lbord, rbord, i, func);

    Spline spline(grid, der2(lbord));
    double res = spline.MaxErrCalc();

    xout << i << " ";
    yout << res << " ";
  }
  xout.close();
  yout.close();

  return;
}

void TestEFDV(double(*func)(double), double(*der2)(double)) {
  double rbord = RBORD;
  double lbord = LBORD;
  double dx = DX;
  size_t maxDer = MAX_DER;
  std::ofstream xout, yout;

  xout.open("Func/derVal.txt");
  yout.open("Func/maxErrDer.txt");

  for (double i = MIN_DER; i < maxDer; i += dx) {
    int numNods = 20;
    Grid grid(lbord, rbord, numNods, func);

    Spline spline(grid, i);
    double res = spline.MaxErrCalc();

    xout << i << " ";
    yout << res << " ";
  }
  xout.close();
  yout.close();

  return;
}