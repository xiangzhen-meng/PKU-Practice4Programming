#include <iostream>
using namespace std;
class Sample {
public:
	int my_value;
  // 在此处补充你的代码
  static int sum;
  Sample(int x) : my_value(x) {
    sum += my_value;
  }
  Sample(const Sample& other) : my_value(other.my_value) {
    sum += my_value;
  }
  Sample() : my_value(0) {}
  ~Sample() {
    sum -= my_value;
  }
};
int Sample::sum = 0;
// Code End
int main()
{
	Sample a(5);
	cout<<Sample::sum<<endl;
	Sample b = a;
	cout << Sample::sum << endl;
	Sample c;
	cout << Sample::sum << endl;
	Sample * d = new Sample(20);
	cout << Sample::sum<<endl;
	delete d;
	cout << Sample::sum<<endl;
	return 0;
}