#include <iostream>
#include <set>
#include <map>
#include <cmath>
using namespace std;

int main() {
  int n;
  if (!(cin >> n)) return 0;
  map<int, int> lst; // power, id
  lst.insert(make_pair(1000000000, 1));
  while (n--) {
    int id, power;
    cin >> id >> power;
    int best;
    auto it = lst.find(power);

    if (it != lst.end()) {
      best = it->second;
      it->second = min(id, it->second);
    } else {
      auto rgt = lst.lower_bound(power);
      if (rgt == lst.end()) best = prev(rgt)->second;
      else if (rgt == lst.begin()) best = rgt->second;
      else {
        auto lft = prev(rgt);
        int rabs = abs(rgt->first - power);
        int labs = abs(lft->first - power);
        if (rabs < labs) best = rgt->second;
        else if (labs < rabs) best = lft->second;
        else if (rgt->second < lft->second) best = rgt->second;
        else best = lft->second;
      }
      lst.insert(make_pair(power, id)); 
    }
    cout << id << " " << best << endl;
  }
  return 0;
}