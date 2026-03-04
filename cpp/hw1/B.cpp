#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;

class Complex {
  private:
    double r, i;

  public:
    void Print() { cout << r << "+" << i << "i" << endl; }
    Complex& operator=(const char *input) {
      double rr = 0, ii = 0;
      sscanf(input, "%lf+%lfi", &rr, &ii);
      r = rr;
      i = ii;
      return *this;
    }
};

int main() {
  Complex a;
  a = "3+4i";
  a.Print();
  a = "5+6i";
  a.Print();
  return 0;
}