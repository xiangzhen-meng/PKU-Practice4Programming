#include <iostream>
#include <string>
using namespace std;


template <class T>
T SumArray(/*code here*/T* start, T* end) {
  // 在此处补充你的代码
  T sum = *start;
  int len = end - start;
  for (int i = 1; i < len; i++) {
    sum += *(start + i);
    cerr << "CERR: sum = " << sum << endl;
  }
  return sum;
}


int main() {
	string array[4] = { "Tom","Jack","Mary","John"};
	cout << SumArray(array,array+4) << endl;
	int a[4] = { 1, 2, 3, 4};  //提示：1+2+3+4 = 10
	cout << SumArray(a,a+4) << endl;
	return 0;
}