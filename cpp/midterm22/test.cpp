#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <queue>
using namespace std;

int main() {
  set<int> s;
  s.insert(123);
  s.insert(4);
  queue<int> q;
  q.push(4);
  cout << *s.find(4) << endl;
  cout << "STL find: " << *find(s.begin(), s.end(), 4) << endl;
  return 0;
}