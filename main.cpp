#include "ComputePoint.h"
#include <iostream>

using namespace std;

int main() {
  while (true) {
    int* a = new int[4];
    cin >> a[0] >> a[1] >> a[2] >> a[3];
    compute24(a, 4);
    delete[] a;
  }
  return 0;
}
