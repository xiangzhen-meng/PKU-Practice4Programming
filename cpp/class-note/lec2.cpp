#include <cstdio>
#include <iostream>
using namespace std;

class Complex {
  private:
    double real, imagine;

  public:
    Complex() {}
    explicit Complex(double r, double i) {
      
      real = r;
      imagine = i;
    }
    Complex(Complex &c) {
      real = c.real;
      imagine = c.imagine;
    }
    ~Complex() {}
};

int main() {
  double r, i;
  cin >> r >> i;
  Complex x(r, i);
  // Complex y = {3, 4}; // 还可以这样写！
  Complex y = Complex();
  return 0;
}