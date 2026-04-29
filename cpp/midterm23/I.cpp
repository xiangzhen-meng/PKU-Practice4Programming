// 位运算的题目
// 给 OI 爷鬼辣
#include <iostream>
using namespace std;

int bitManipulation4(int n) {
  return (n ^ (n + 1)) >> 1;
}

int main() {
    int t, n;
    cin >> t;
    while (t--) {
        cin >> n;
        cout << bitManipulation4(n) << endl;
    }
    return 0;
}