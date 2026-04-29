// 这个 const 的行为好怪异啊
#include <iomanip>
#include <iostream>
using namespace std;

class TNone {};
TNone None;
// 在此处补充你的代码
template<class T>
class Option {
  private:
    T val;
    bool hasval = false;
  public:
    Option() {}
    Option(T v) : val(v), hasval(true) {}
    Option(TNone x) {}
    bool has_value() {
      return hasval;
    }
    T& operator* () {
      return val;
    }
    const T& operator* () const {
      return val;
    }
    T value() {
      return val;
    }
    // ?
    Option& operator= (TNone x) {
      hasval = false;
      return *this;
    }
    operator bool() const {
      return hasval;
    }
};



int main() {
  cout << boolalpha;

  Option<int> a(0), b, c(1);
  cout << a.has_value() << endl;
  cout << b.has_value() << endl;
  b = a;
  *b += 10;
  cout << a.value() << endl;
  cout << b.value() << endl;
  c = None;
  cout << c.has_value() << endl;

  Option< Option<int> > x = None;
  const Option< Option<int> > y = a;
  Option< Option<int> > z = c;
  if (x)
    cout << "x has value" << endl;
  if (y)
    cout << "y has value" << endl;
  if (z)
    cout << "z has value" << endl;
  x = y;
  x = x;
  *x = b;
  **x = 20;
  // What are you funkling doing
  cout << x.value().value() + **y << endl;
  return 0;
}