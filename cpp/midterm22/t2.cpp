// 好奇怪，重新看一下
#include <stdio.h>
#include <iostream>
using namespace std;
class MyInteger{
public:
  unsigned char C;
  MyInteger(unsigned char c='0'): C(c) {}
// 在此处补充你的代码
  int num;
  bool filled = false;
  MyInteger(int x) : num(x) {}

  void fillnum() {
    if (!filled) {
      filled = true;
      num = C - '0';
    }
  }
  MyInteger operator*(MyInteger& other) {
    this->fillnum();
    other.fillnum();
    filled = true;
    other.filled = true;
    int ans = num * other.num;
    return MyInteger(ans);
  }
  MyInteger operator+(MyInteger& other) {
    return MyInteger(num + other.num);
  }
  operator int() {
    return num;
  }

// Code End
};

int main() { 
  unsigned char m,n;
  cin >> m >> n;
  MyInteger n1(m), n2(n);
  MyInteger n3;
  n3 = n1*n2;
  MyInteger n4 = n1+n2+n3;
  cout << int(n3) << endl;
  cout << n1+n2+n3 << endl;
  return 0;
}