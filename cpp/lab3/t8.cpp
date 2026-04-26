#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
using namespace std;

// Code Here
template <class T> class CMyClass {
  private:
    T *lst;
    int n;

  public:
    CMyClass(T *l, int nn) : n(nn) {
      lst = new T[n + 1];
      for (int i = 0; i < n; i++) {
        lst[i] = l[i];
      }
    }
    T &operator[](int i) { return lst[i]; }
    ~CMyClass() { delete[] lst; }
};
// Code End

int a[40];
int main(int argc, char **argv) {
  int t;
  scanf("%d", &t);
  while (t--) {
    int m;
    scanf("%d", &m);
    for (int i = 0; i < m; ++i)
      scanf("%d", a + i);
    char s[100];
    scanf("%s", s);
    CMyClass<int> b(a, m);
    CMyClass<char> c(s, strlen(s));
    printf("%d %c\n", b[5], c[7]);
  }
  return 0;
}