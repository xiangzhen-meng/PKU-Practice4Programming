#include <iostream>
using namespace std;
class A {
  public:
	  int val;
	  A(int x) : val(x) {
      // cerr << "init with val" << endl;
    }
    A() : val(123) {
      // cerr << "init without val" << endl;
    }
    A(const A & a) {
      this->val = a.val;
      // cerr << "copy constructor called" << endl;
    }

    A& GetObj() {
      return *this;
    }

};

int main() {
	int m,n;
	A a;
	cout << a.val << endl;
	while(cin >> m >> n) {
		a.GetObj() = m;
		cout << a.val << endl;
		a.GetObj() = A(n);
		cout << a.val<< endl;
	}
	return 0;
}