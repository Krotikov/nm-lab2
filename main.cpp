#include <iostream>
#include "test.h"

int main() {
  TestNumNodsInPoints(&Func, &Der2);

  TestNumNods(&Func, &Der2);

  TestEFNN(&Func, &Der2);

  TestEFDV(&Func, &Der2);
  return 0;
}