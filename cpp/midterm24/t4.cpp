#include<iostream>
using namespace std;
class Midterm {
private:
  int val;
public:
// 在此处补充你的代码
  Midterm(int n) : val(n) {}
  Midterm(const Midterm& other) : val(other.val) {}
  friend Midterm operator+(const int& x, const Midterm& m) {
    return Midterm(m.val + x);
  }
  Midterm operator-(int x) {
    return Midterm(val - x);
  }
  Midterm operator++(int) {
    int ret = val;
    val += 1;
    return Midterm(ret);
  }
  Midterm& operator++() {
    val ++;
    return *this;
  }
  Midterm& operator-=(int x) {
    val -= x;
    return *this;
  }
  friend ostream& operator<< (ostream& output, const Midterm& m) {
    output << m.val;
    return output;
  }
  operator int() {
    return val;
  }
// Code End
};

int mean (int a, int b) {
    return (a+b)/2;
}

int main(){
  int n;
  cin >> n;
  Midterm b(n);
  cout  << b - 10 << endl;  //输出 n - 10 
  cout << ++b << endl;  //输出 n + 1 
  cout << b++ << endl; //输出 n + 1 
  ++b = n;  
  Midterm c = 2 + b; 
  ((c -= 1) -= 2) -= 3;
  cout << c <<endl;  //输出n-4 
  cout << mean(n, c) << endl; //输出 n-2 
  return 0;
}