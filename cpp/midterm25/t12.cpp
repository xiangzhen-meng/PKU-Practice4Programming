#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

map<string, int> times;
map<string, int> pts;

int main() {
  int n, x, y;
  cin >> n >> x >> y;
  while (n--) {
    string cname, stuname;
    int score;
    cin >> cname >> stuname >> score;
    if (times.find(stuname) != times.end()) {
      times[stuname]++;
      pts[stuname] += score;
    } else {
      times[stuname] = 1;
      pts[stuname] = score;
    }
  }
  int m;
  cin >> m;
  while (m--) {
    string stuname;
    cin >> stuname;
    int t = times[stuname];
    int p = pts[stuname];
    if (t >= x && p / t > y) {
      cout << "yes" << endl;
    } else {
      cout << "no" << endl;
    }
  }
  return 0;
}