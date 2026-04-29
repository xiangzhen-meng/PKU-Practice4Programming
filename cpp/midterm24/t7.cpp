#include <iostream>
#include <queue>
#include <string>
#include <vector>
using namespace std;

struct people {
  int hp;
  string name;
  people(int h, string n) : hp(h), name(n) {}
};

struct cmp {
  bool operator() (const people& a, const people& b) const {
    if (a.hp != b.hp) {
      return a.hp < b.hp;
    } else {
      return a.name < b.name;
    }
  }
};

priority_queue<people, vector<people>, cmp> q;

int main() {
  int n;
  string name;
  int hp;
  cin >> n;
  while (n--) {
    cin >> hp >> name;
    q.push(people(hp, name));
  }
  while (q.size() > 1) {
    people a = q.top();
    q.pop();
    people b = q.top();
    q.pop();
    if (a.hp == b.hp) continue;
    a.hp -= b.hp;
    q.push(a);
  }

  if (q.size() == 1) {
    people l = q.top();
    cout << l.name << endl;
  } else {
    cout << "-1" << endl;
  }

  return 0;
}