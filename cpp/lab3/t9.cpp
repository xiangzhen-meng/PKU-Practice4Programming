#include <iostream>
using namespace std;

int gcd(int a, int b) { // 计算两个数的最大公约数
  return (!b) ? a : gcd(b, a % b);
}
class Fraction {
    int p, q;

  public:
    // Code Here
    Fraction() {}
    Fraction(int pp) : p(pp), q(1) {}
    Fraction(int pp, int qq) : p(pp), q(qq) {}
    Fraction &operator=(Fraction &other) {
      p = other.p;
      q = other.q;
      this->clean();
      return *this;
    }
    Fraction operator*(Fraction &other) {
      int np = p * other.p;
      int nq = q * other.q;
      Fraction ret(np, nq);
      ret.clean();
      return ret;
    }
    Fraction operator*(int n) {
      int np = p * n;
      Fraction ret(np, q);
      ret.clean();
      return ret;
    }
    void clean() {
      int g = gcd(p, q);
      p = p / g;
      q = q / g;
    }
    friend istream &operator>>(istream &input, Fraction& frac) {
      input >> frac.p >> frac.q;
      frac.clean();
      return input;
    }
    friend ostream &operator<<(ostream &output, const Fraction& frac) {
      if (frac.q == 1)
        output << frac.p;
      else
        output << frac.p << "/" << frac.q;
      return output;
    }
    // Code End
};

int main() {
  int testcases;
  cin >> testcases;
  while (testcases--) {
    Fraction a, b, two(2);
    cin >> a >> b;
    cout << a << " * " << b << " = " << a * b << endl;
    cout << "2 * " << a << " = " << two * a << endl;
    cout << b << " * 3 = " << b * 3 << endl;
  }
  return 0;
}