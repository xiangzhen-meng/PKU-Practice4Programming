// 按值返回，注意复习一下
#include <iostream>
using namespace std;
class C {
	public:
		static int num;
		int curr_value;
		friend ostream& operator << (ostream& o, const C& c) {
			o << "() called " << num << " times, sum is " << c.curr_value;
			return o;
	  }
    // 在此处补充你的代码
    C() : curr_value(0) {}
    C(int x) : curr_value(x) {}
    C operator() (int x) {
      num++;
      return C(curr_value + x);
    }
    C operator() (int x, int y) {
      num++;
      return C(curr_value + x + y);
    }

    // Code End
};
int C::num = 0; 
int main() {
	C c1;
	cout << c1(1)(2) << endl;
	cout << c1(3, 4) << endl;
	cout << c1(5, 6)(7) << endl;
	C c2;
	cout << c2(7)(8, 9) << endl;
	return 0;
}