#include <iostream>
#include <set>
using namespace std;

int factorCnt(int x) {
  int cnt = 0;
  int ori = x;
  for (int i = 2; 1LL * i * i <= x; ++i) {
    if (x % i == 0) {
      ++cnt;
      while (x % i == 0)
        x /= i;
    }
  }
  if (x > 1 && x != ori)
    ++cnt;
  return cnt;
}

struct cmp {
    bool operator()(const int &x, const int &y) const {
      int fx = factorCnt(x);
      int fy = factorCnt(y);
      if (fx != fy)
        return fx > fy;
      else
        return x > y;
    }
};

int main() {
  int n;
  cin >> n;
  multiset<int, cmp> lst;
  while (n--) {
    for (int i = 0; i < 10; i++) {
      int x;
      cin >> x;
      lst.insert(x);
    }
    cout << *lst.begin() << " " << *lst.rbegin() << endl;
    if (lst.size() == 1) {
      lst.erase(lst.begin());
    } else {
      lst.erase(lst.begin());
      lst.erase(--lst.end());
    }
  }
  return 0;
}