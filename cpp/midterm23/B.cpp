#include <iostream>
using namespace std;

class A {
private:
    int v;
public:
  A() {v = 1;}
// 在此处补充你的代码
  void printV() const {
    cout << v << endl;
  }
  void printV() {
    cout << "2" << endl;
  }

// Code End
};


int main() {
    const A obj1;
    A obj2;

    obj1.printV();
    obj2.printV();

    A* p1 = (A*)(&obj1);
    A* p2 = &obj2;

    p1->printV();
    p2->printV();

    return 0;
}