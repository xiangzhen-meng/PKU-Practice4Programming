// 为什么21行会能够直接调用构造函数？？？初始值是怎么分配的？


#include <iostream>
using namespace std;

class A {
public:
// 在此处补充你的代码
  static int total;
  A() { total++;}
  A(int a, int b) { total++; }
  A(int x) { total++; }
  A(const A& other) { total++; }
  ~A() { total--; }
};

int A::total = 0;

int main() {
    A array1[4]; 
    cout << A::total << "\n";
    A array2[4] = {1, A(1,1)};
    cout << A::total << "\n";
    A a(array1[0]);
    cout << A::total << "\n";
    A * p = new A();
    cout << A::total << "\n";
    delete p;
    cout << A::total << "\n";
    return 0;
}