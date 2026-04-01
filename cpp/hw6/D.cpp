#include <iostream>
using namespace std;

class MyCin {
  // 在此处补充你的代码
  private:
    bool bstop;
  public:
    MyCin() : bstop(false) {}
    operator bool() {
      return !bstop;
    }
    MyCin& operator>>(int& x) {
      cin >> x;
      if (x == -1) bstop = true;
      return *this;
    }
};

int main() {
  MyCin m;
  int n1, n2;
  while (m >> n1 >> n2)
    cout << n1 << " " << n2 << endl;
  return 0;
}