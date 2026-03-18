#include <cstring>
#include <iostream>
using namespace std;

class Array2 {
  // 在此处补充你的代码
  private:
    int r;
    int c;
  public:
    int** arr;
    Array2() {}
    Array2(int row, int col): r(row), c(col){
      arr = new int*[row];
      for (int i = 0; i < row; i++) {
        arr[i] = new int[col];
      }
    }
    int* operator[](int idx) {
      return arr[idx];
    }
    int operator()(int i, int j) {
      return arr[i][j];
    }
    Array2& operator=(const Array2& a) {
      r = a.r;
      c = a.c;
      arr = new int*[r];
      for (int i = 0; i < r; i++) {
        arr[i] = new int[c];
        for (int j = 0; j < c; j++) {
          arr[i][j] = a.arr[i][j];
        }
      }
      return *this;
    }
};

int main() {
  Array2 a(3, 4);
  int i, j;
  for (i = 0; i < 3; ++i)
    for (j = 0; j < 4; j++)
      a[i][j] = i * 4 + j;
  for (i = 0; i < 3; ++i) {
    for (j = 0; j < 4; j++) {
      cout << a(i, j) << ",";
    }
    cout << endl;
  }
  cout << "next" << endl;
  Array2 b;
  b = a;
  for (i = 0; i < 3; ++i) {
    for (j = 0; j < 4; j++) {
      cout << b[i][j] << ",";
    }
    cout << endl;
  }
  return 0;
}