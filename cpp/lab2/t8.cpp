#include <iostream>
#include <map>
using namespace std;
// 在此处补充你的代码
class A {
private:
  int n;
public:
  static int count;
  A() { count++; }
  A(int x) : n(x) { count++; }
  A(const A& other) { count++; }
  virtual ~A() {
    count--;
    cout << "A::destructor" << endl;
  }
};

class B: public A {
public:
  B(int x) : A(x) {}
  ~B() { cout << "B::destructor" << endl; }
};
// end
int A::count = 0;
void func(B b) {}

int main() {
  A a1(5), a2;
  cout << A::count << endl;
  B b1(4);
  cout << A::count << endl;
  func(b1);
  cout << A::count << endl;
  A *pa = new B(4);
  cout << A::count << endl;
  delete pa;
  cout << A::count << endl;
  return 0;
}