#ifndef _COMPUTE_POINT_
#define _COMPUTE_POINT_
#include <iostream>

#define ARRAY_NUM 8

class ComputePoint {
public:
  float value;
  char opr;
  int* in_array;
  int size_array;
  ComputePoint* leftChild;
  ComputePoint* rightChild;
  ComputePoint(int v = 0, char c = ' ') : value(v), opr(c) { 
    leftChild = NULL; 
    rightChild = NULL;  
    in_array = new int[ARRAY_NUM]; 
    in_array[0] = v;
    size_array = 1; 
  }
  void set_value(int v = 0) {
    value = v;
    opr = ' '; 
    leftChild = NULL; 
    rightChild = NULL;  
    in_array = new int[ARRAY_NUM]; 
    in_array[0] = v;
    size_array = 1;
  }
};

class ComputeTree {
public:
  ComputePoint* root;
  ComputeTree() { root = NULL; }
};

void compute24(int* a, int size = 4);
#endif // _COMPUTE_POINT_
