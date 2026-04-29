#include<iostream>
using namespace std;
int count(unsigned int x) {
// 在此处补充你的代码
  int cnt = 0;
  while(x) {
    cnt += x % 2;
    x /= 2;
  }
  return cnt;
}
int main() {
	int T; cin >> T;
	while (T--) {
		unsigned x; cin >> x;
		cout << count(x) << endl;
	}
	return 0;
}