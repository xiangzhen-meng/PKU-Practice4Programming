#include <iostream>
#include <list>
#include <algorithm>
#include <iterator>
using namespace std;

typedef list<int> LST;

LST a[200005];

int main() {
  int n;
  cin >> n;
  while(n--) {
    string manip;
    cin >> manip;
    if (manip == "new") {
      int id;
      cin >> id;
    } else if (manip == "add") {
      int id, num;
      cin >> id >> num;
      a[id].push_back(num);
    } else if (manip == "merge") {
      int id1, id2;
      cin >> id1 >> id2;
      if (id1 != id2) {
        a[id1].merge(a[id2]);
        a[id2].clear();
      }
    } else if (manip == "unique") {
      int id;
      cin >> id;
      a[id].sort();
      a[id].unique();
    } else if (manip == "out") {
      int id;
      cin >> id;
      if (!a[id].empty()) {
        a[id].sort();
        ostream_iterator<int> out(cout, " ");
        copy(a[id].begin(), a[id].end(), out);
      }
      cout << endl;
    }
  }
  return 0;
}