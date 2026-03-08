#include <iostream>
using namespace std;

class Apple {
  // 在此处补充你的代码
  private:
    static int nTotalNumber;
  public:
  Apple() {
    this->nTotalNumber++;
    // cout << "constructor called, nTotalNumber = " << nTotalNumber << endl;
  }
  Apple(const Apple & a) {
    // this->nTotalNumber++;
    // cout << "copy constructor called" << endl;
  }
  ~Apple() {
    this->nTotalNumber--;
    // cout << "destructor called, nTotalNumber = " << nTotalNumber << endl;
  }

  static void PrintTotal() {
		cout << nTotalNumber << endl; 
	}

};

int Apple::nTotalNumber = 0;
Apple Fun(const Apple & a) {
	a.PrintTotal();
	return a;
}

int main()
{
	Apple * p = new Apple[4];
	Fun(p[2]);
	Apple p1,p2;
	Apple::PrintTotal ();
	delete [] p;
	p1.PrintTotal ();
	return 0;
}