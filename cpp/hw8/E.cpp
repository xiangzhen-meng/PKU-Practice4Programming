#include <iostream>
#include <set>
#include <cmath>
#include <cstdlib>
#include <utility>
using namespace std;

int main() {
  int n;
  if (!(cin >> n)) return 0;
  set<pair<int,int>> lst; // (power, id)

  while (n--) {
    int id, power;
    cin >> id >> power;

    lst.insert(make_pair(1000000000, 1));

    auto it = lst.lower_bound({power, id});
    pair<int,int> best;

    if (it == lst.begin()) {
      best = *it;
    } else if (it == lst.end()) {
      best = *prev(lst.end());
    } else {
      auto itn = it;
      auto itp = prev(it);
      long long d1 = llabs((long long)itp->first - power);
      long long d2 = llabs((long long)itn->first - power);
      if (d1 <= d2) best = *itp; else best = *itn;
    }

    cout << id << " " << best.second << '\n';
    lst.insert({power, id});
  }

  return 0;
}