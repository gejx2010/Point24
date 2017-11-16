#include "ComputePoint.h"
#include <iostream>
#include <cstdio>
#include <cmath>
#include <stack>
#include <vector>

using namespace std;

#define COMPILE false
#define MAX_NUM 1000

bool is_answer(float f, float a = 24.0) {
  return fabs(f - a) < 1e-3;
}

void push_stack(ComputePoint* p, stack<ComputePoint*>& st) {
  if (p != NULL) {
    st.push(p);
    push_stack(p->leftChild, st);
    push_stack(p->rightChild, st);
  }
}

void printPoint(ComputePoint* p) {
  stack<ComputePoint*> cur_stack;
  push_stack(p, cur_stack);
  while (!cur_stack.empty()) {
    ComputePoint* cp = cur_stack.top();
    if (cp->leftChild != NULL && cp->rightChild != NULL) {
      float ll = cp->leftChild->value;
      float rr = cp->rightChild->value;
      float ss = cp->value;
      if (is_answer(ll, int(ll))) {
        printf("%d ", int(ll));
      } else {
        printf("%f ", ll);
      }
      printf("%c ", cp->opr);
      if (is_answer(rr, int(rr))) {
        printf("%d ", int(rr));
      } else {
        printf("%f ", rr);
      }
      printf("= ");
      if (is_answer(ss, int(ss))) {
        printf("%d", int(ss));
      } else {
        printf("%f", ss);
      }
      printf("\n");
    }
    cur_stack.pop();
  }
}

int* combine_array(int* a, int size_a, int* b, int size_b) {
  int* res = new int[size_a + size_b];
  int rank = 0;
  for (int i = 0; i < size_a; i++) res[rank++] = a[i];
  for (int i = 0; i < size_b; i++) res[rank++] = b[i];
  return res;
}

ComputePoint* make_new_point(char opr, ComputePoint* a, ComputePoint* b, float value = 24.0) {
  ComputePoint* res = new ComputePoint;
  res->value = value;
  res->opr = opr;
  res->in_array = combine_array(a->in_array, a->size_array, b->in_array, b->size_array);
  res->size_array = a->size_array + b->size_array;
  res->leftChild = a;
  res->rightChild = b;
  return res;
}

bool combine_two_points(ComputePoint* a, ComputePoint* b, ComputePoint** res, int& sz) {
  if (COMPILE) {
    printf("@combine_two_points, a->size_array: %d, b->size_array: %d\n", a->size_array, b->size_array);
  }
  int rank = 0;
  res[rank++] = make_new_point('+', a, b, a->value + b->value);
  res[rank++] = make_new_point('-', a, b, a->value - b->value);
  res[rank++] = make_new_point('-', b, a, b->value - a->value)
;
  res[rank++] = make_new_point('*', a, b, a->value * b->value);
  if (!is_answer(b->value, 0)) res[rank++] = make_new_point('/', a, b, (float)a->value / b->value);
  if (!is_answer(a->value, 0)) res[rank++] = make_new_point('/', b, a, (float)b->value / a->value);
  sz = rank;
  if (COMPILE) {
    printf("@combine_two_points, size: %d, end.\n", sz);
  }
  return (sz != 0);
}

void import_vector(int* a, int sz, int* rank_array, int num, vector<int*>& b) {
  int i = 0, j = 0, k = 0;
  int* c = new int[sz];
  for (int i = 0; i < sz; i++) {
    if (rank_array[j] == i) {
      c[j] = a[i];
      j++;
    } else {
      c[num + k] = a[i];
      k++;
    }
  }
  b.push_back(c);
}

void select_combine_num(int* a, int sz, int num, vector<int*>& b, int& combine_size) {
  int* rank_array = new int[num];
  for (int i = 0; i < num; i++) rank_array[i] = i;
  int cur_end = sz - 1;
  int cur_layer = num - 1;
  combine_size = 0;
  while (true) {
    import_vector(a, sz, rank_array, num, b);
    combine_size++;
    bool no_next = false;
    while (true) {
      if (cur_layer == 0 && rank_array[cur_layer] == cur_end)  {
        no_next = true;
        break;
      }
      if (rank_array[cur_layer] == cur_end) { 
        cur_layer--, cur_end--; 
      } else {
        rank_array[cur_layer]++;
        break;
      }
    
    }
    if (no_next) break;
  }
}

void combine(int* a, int size, ComputePoint** r, int& rz ) {
  if (COMPILE) {
    printf("begin combine, with a_size: %d, a[0]: %d\n", size, a[0]);
  }
  if (size <= 0) return;
  if (size == 1) {
    if (COMPILE) {
      printf("@combine, with size 1, set value:%d\n", a[0]);
    }
    r[0]->set_value(a[0]);
    if (COMPILE) {
      printf("@combine, with size 1, after set value:%f\n", r[0]->value);
    }
    rz = 1;
  } else if (size == 2) {
    ComputePoint* mid = new ComputePoint[2];
    mid[0].set_value(a[0]);
    mid[1].set_value(a[1]);
    combine_two_points(&mid[0], &mid[1], r, rz);
  } else {
    // 最外层循环，表示选出几个作另一备用
    rz = 0;
    for (int i = 1; i <= size / 2; i++) {
      // 选出由i表示的所有组合数数组
      vector<int*> b;
      int combine_size;
      if (COMPILE) {
        printf("@combine, before select_combine_num.\n");
      }
      select_combine_num(a, size, i, b, combine_size);
      if (COMPILE) {
        printf("@combine, after select_combine_num, with i : %d, combine_size: %d\n", i, combine_size);
      }
      for (int j = 0; j < combine_size; j++) {
        if (COMPILE) {
          printf("@combine, left[%d] is:\n", j);
          for (int v = 0; v < i; v++) {
            printf("%d ", b[j][v]);
          }
          printf("\n");
          printf("@combine, right[%d] is:\n", j);
          for (int v = i; v < size; v++) {
            printf("%d ", b[j][v]);
          }
          printf("\n");
        }
        ComputePoint** left = new ComputePoint*[MAX_NUM];
        for (int v = 0; v < MAX_NUM; v++) {
          left[v] = new ComputePoint;
        }
        int left_size;
        combine(b[j], i, left, left_size);
        if (COMPILE) {
          printf("@combine, begin output left array.\n");
          printf("before:\n");
          for (int v = 0; v < i; v++) {
            printf("b[%d][%d]: %d\n", j, v, b[j][v]);
          }
          printf("after:\n");
          for (int v = 0; v < left_size; v++) {
            printf("%d: %f\n", v, left[v]->value);
          }
          printf("@combine, end output left array.\n");
        }
        ComputePoint** right = new ComputePoint*[MAX_NUM];
        for (int v = 0; v < MAX_NUM; v++) {
          right[v] = new ComputePoint;
        }
        int right_size;
        combine(b[j] + i, size - i, right, right_size);
        if (COMPILE) {
          printf("@combine, begin output right array.\n");
          printf("before:\n");
          for (int v = i; v < size; v++) {
            printf("b[%d][%d]:%d\n", j, v, b[j][v]);
          }
          printf("after:\n");
          for (int v = 0; v < right_size; v++) {
            printf("%d: %f\n", v, right[v]->value);
          }
          printf("@combine, end output right array.\n");
        }
        if (COMPILE) {
          printf("with left_size: %d, right_size: %d\n", left_size, right_size);
          printf("left[0]->value: %f\n", left[0]->value);
          printf("right[0]->value: %f\n", right[0]->value);
        }
        for (int j = 0; j < left_size; j++) {
          for (int k = 0; k < right_size; k++) {
            ComputePoint** mid = new ComputePoint*[MAX_NUM];
            for (int v = 0; v < MAX_NUM; v++) {
              mid[v] = new ComputePoint;
            }
            int mid_size;
            combine_two_points(left[j], right[k], mid, mid_size);
            if (COMPILE) {
              printf("@combine, after left & right, res is:\n");
              for (int v = 0; v < mid_size; v++) {
                printf("%d: %f\n", v, mid[v]->value);
              }
              printf("\n");
            }
            for (int m = 0; m < mid_size; m++) r[rz++] = mid[m];
          }
        }
      }
    }
  }
  if (COMPILE) {
    printf("@combine, after decode\n");
    int ttt;
    scanf("%d", &ttt);
  }
}

void compute24(int* a, int size) {
  if (COMPILE) {
    printf("begin compute 24 with a:\n");
    for (int i = 0; i < size; i++) {
      printf("%d ", a[i]);
    }
    printf("\n");
  }
  ComputePoint** root;
  root = new ComputePoint*[MAX_NUM];
  for (int j = 0; j < MAX_NUM; j++) {
    root[j] = new ComputePoint;
  }
  int root_size;
  combine(a, size, root, root_size);
  if (COMPILE) {
    printf("after combine, root_size: %d\n", root_size);
  } 
  int cnt = 0;
  for (int i = 0; i < root_size; i++) {
    ComputePoint* p = root[i];
    if (is_answer(p->value)) {
      printf("Solution %d:\n", ++cnt);
      printPoint(p);
    }
  }
  if (cnt <= 0) {
    printf("There is no solution.\n");
  }
}


