#include <iostream>
using namespace std;
class C {
public:
  static int num;
  int curr_value;
  friend ostream& operator << (ostream& o, const C& c) = delete;
  friend ostream& operator << (ostream& o, C& c) {
    o << "() called " << num << " times, sum is " << c.curr_value;
    return o;
  }
// 在此处补充你的代码
  C() : curr_value(0) {}
  C(int x) : curr_value(x) {}
  C& operator() (int x) const {
    num++;
    C* c = new C(x + curr_value);
    return *c;
  }
  C& operator() (int x, int y) {
    num++;
    C* c = new C(x + y + curr_value);
    return *c;
  }
};
int C::num = 0;
// Code End

int main() {
  C c1;
  cout << c1(1)(2) << endl;
  cout << c1(3, 4) << endl;
  cout << c1(5, 6)(7) << endl;
  C c2;
  cout << c2(7)(8, 9) << endl;
  return 0;
}