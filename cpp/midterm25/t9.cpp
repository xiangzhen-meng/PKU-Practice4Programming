#include <iostream>
#include <vector>
using namespace std;

// Code Here
template <class Iter, class F = greater<int>>
int getWanted(Iter b, Iter e, F f = F()) {
  Iter m = b;
  for (Iter i = b + 1; i != e; i++) {
    if (f(*m, *i)) {
      m = i;
    }
  }
  return *(m);
}
// Code End

struct myComp {
    bool operator()(int a, int b) { return a % 10 < b % 10; }
};

int main() {
  int n;
  cin >> n;
  vector<int> a, b, c;
  for (int i = 0; i < n; ++i) {
    int x;
    cin >> x;
    a.push_back(x);
  }
  int cmd;
  while (cin >> cmd) {
    switch (cmd) {
    case 0:
      cout << getWanted<vector<int>::iterator, less<int>>(a.begin(), a.end())
           << endl;
      break;
    case 1:
      cout << getWanted<vector<int>::iterator>(a.begin(), a.end(), less<int>())
           << endl;
      break;
    case 2:
      cout << getWanted<vector<int>::iterator>(a.begin(), a.end(), myComp())
           << endl;
      break;
    case 3:
      cout << getWanted<vector<int>::iterator>(a.begin(), a.end()) << endl;
      break;
    }
  }
  return 0;
}