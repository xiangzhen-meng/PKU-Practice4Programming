#include <iostream>
#include <sstream>
#include <string>
using namespace std;
// 在此处补充你的代码
template <typename T> 
T parse_and_sum(const string &line) {
  stringstream ss(line);
  T value;
  T result = T();
  while (ss >> value) {
    result += value;
  }
  return result;
}
// Code End
int main() {
  int t;
  cin >> t;
  cin.ignore();
  while (t--) {
    string line1, line2;
    getline(cin, line1);
    getline(cin, line2);

    cout << parse_and_sum<int>(line1) << endl;
    cout << parse_and_sum<string>(line2) << endl;
  }
  return 0;
}