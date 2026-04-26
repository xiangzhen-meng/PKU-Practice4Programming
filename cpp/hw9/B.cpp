#include <iostream>
using namespace std;
// 在此处补充你的代码
template<class F1, class F2, class T>
class combine {
  private:
    F1 fun1;
    F2 fun2;
  public:
    combine(F1 f1, F2 f2) : fun1(f1), fun2(f2) {}
    T operator() (T x) {
      return fun1(fun1(x) + fun2(x));
    }
};
// Code End
int main()
{
    auto Square = [] (double a) { return a * a; };
    auto Inc = [] (double a) { return a + 1; };
    cout << combine<decltype(Square),decltype(Inc),int>(Square,Inc)(3) << endl;
    cout << combine<decltype(Inc),decltype(Square),double>(Inc,Square)(2.5) << endl;

    return 0;
}