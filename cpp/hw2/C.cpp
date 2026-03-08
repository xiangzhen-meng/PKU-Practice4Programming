#include <iostream>
using namespace std;

class Sample {
  public:
	  int v;
    // 在此处补充你的代码
    Sample() {}
    Sample(int x) {
      this->v = x;
    }
    Sample(const Sample & x) {
      this->v = x.v + 2;
    }

};

void PrintAndDouble(Sample o) { // call copy constructor
	cout << o.v;
	cout << endl;
}

int main()
{
	Sample a(5);
  // a.v == 5

	Sample b = a; // copy constructor
	PrintAndDouble(b);

  // conversion constructor
	Sample c = 20;
	PrintAndDouble(c);

  // a.v == 5
	Sample d;
	d = a;
	cout << d.v;
	return 0;
}