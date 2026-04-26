#include <iostream>
#include <set>
using namespace std;

int main() {
  multiset<int> a;
  set<int> vis;
  int n;
  cin >> n;
  while (n--) {
    string manip;
    cin >> manip;
    int x;
    cin >> x;
    if (manip == "add") {
      a.insert(x);
      vis.insert(x);
      cout << a.count(x) << endl;
    } else if (manip == "del") {
      cout << a.count(x) << endl;
      a.erase(x);
    } else if (manip == "ask") {
      if (vis.count(x) != 0) cout << "1 " << a.count(x) << endl;
      else cout << "0 0" << endl;
    }
  }
  return 0;
}

