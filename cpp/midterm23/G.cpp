#include <iostream>
#include <string>
using namespace std;

class MyCout{
  public:
    // 在此处补充你的代码
    int count = 0;
    ~MyCout() {
      cout << count << endl;
    }
    void operator<< (int n) {
      count += n;
    }
};
// Code End
int main() {
    MyCout mycout;
    int n;
    while (cin >> n)
        mycout << n; 
    return 0;
}