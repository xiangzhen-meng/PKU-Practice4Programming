#include <cstring>
#include <iomanip>
#include <iostream>
using namespace std;
template <class T> 
class CArray3D {
  public:
    int x;
    int y;
    int z;
    T *p;

    CArray3D(int xx, int yy, int zz) {
      x = xx;
      y = yy;
      z = zz;
      p = new T[x * y * z];
    }
    ~CArray3D() { delete[] p; }
    // 在此处补充你的代码
    class CArray2D {
      private:
        T* pd;
        int zz;
      public:
        CArray2D(T* pdd, int zzz) : pd(pdd), zz(zzz) {}
        T* operator[] (int idx) {
          return (pd + idx * zz);
        }
    };
    CArray2D operator[] (int idx) {
      return CArray2D(p + (y * z) * idx, z);
    }
    void operator+ (CArray3D& other) {
      for (int i = 0; i < x * y * z; i++) {
        p[i] += other.p[i];
      }
    }
    operator T*() {
      return p;
    }
    // Code End
};

CArray3D<int> a(3, 4, 5);
CArray3D<int> aa(3, 4, 5);

void PrintA() {
  for (int i = 0; i < 3; ++i) {
    cout << "layer " << i << ":" << endl;
    for (int j = 0; j < 4; ++j) {
      for (int k = 0; k < 5; ++k)
        cout << a[i][j][k] << ",";
      cout << endl;
    }
  }
}
void PrintAA() {
  for (int i = 0; i < 3; ++i) {
    cout << "layer " << i << ":" << endl;
    for (int j = 0; j < 4; ++j) {
      for (int k = 0; k < 5; ++k)
        cout << aa[i][j][k] << ",";
      cout << endl;
    }
  }
}

int main() {
  int No = 0;
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 4; ++j)
      for (int k = 0; k < 5; ++k) {
        a[i][j][k] = No++;
        aa[i][j][k] = a[i][j][k] + a[i][j][0];
      }
  PrintA();
  PrintAA();
  a + aa; // 计算内部每个元素对应求和,更新a
  PrintA();

  memset(a, -1, 60 * sizeof(int)); // 注意这里
  memset(a[1][1], 0, 5 * sizeof(int));
  PrintA();

  return 0;
}