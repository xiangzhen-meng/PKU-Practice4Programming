#include <cstdio>
#include <iostream>
using namespace std;

class f {
// 在此处补充你的代码
public:
  int num;
  f(int x) : num(x) {}
  f(int a, int b) : num(a * b) {}
  f operator() (int x) {
    return f(num + x);
  }
  friend ostream& operator<< (ostream& output, const f& func) {
    output << func.num;
    return output;
  }

};

int main() {
  cout << f(4)(5) << endl;
  cout << f(64)(36) << endl;
  cout << f(3)(5)(7) << endl;
  cout << f(3,8) << endl;
  cout << f(15,3) << endl;
  cout << f(7,10) << endl;
}