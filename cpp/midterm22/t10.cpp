#include <cstring>
#include <iostream>
#include <string.h>
using namespace std;
// 在此处补充你的代码
template <class T> 
class MyAccumulate {
  public:
    T operator()(T *p, int n, T (*func)(T)) {
      T ret = func(p[0]);
      for (int i = 1; i < n; i++) {
        ret += func(p[i]);
      }
      return ret;
    }
};

template <class T, class F> T Myaccumulate(T *p, int n, F func) {
  T ret = func(p[0]);
  for (int i = 1; i < n; i++) {
    ret += func(p[i]);
  }
  return ret;
}

// Code End
int sqr(int n) { return n * n; }
string rev(string s) {
  return string(s.rbegin(), s.rend());
  ;
}
int main() {
  int a[100];
  string b[100];
  int n;
  cin >> n;
  for (int i = 0; i < n; ++i)
    cin >> a[i];
  for (int i = 0; i < n; ++i)
    cin >> b[i];
  cout << Myaccumulate(a, n, sqr) << endl;
  cout << Myaccumulate(b, n, rev) << endl;
  cout << MyAccumulate<int>()(a, n - 1, sqr) << endl;
  cout << MyAccumulate<string>()(b + 1, n - 1, rev) << endl;
  return 0;
}