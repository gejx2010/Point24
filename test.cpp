#include <iostream>
#include <cstdio>
using namespace std;

void make_array(int* a, int* b) {
  for (int i = 0; i < 10; i++) {
    a[i] = i;
    b[i] = i * i;
  }
}

int main() {
  int* a = new int[10];
  int* b = new int[10];
  make_array(a, b);
  for (int i = 0; i < 10; i++) {
    printf("%d: (%d, %d)\n", i, a[i], b[i]);
  }
  return 0;
}
