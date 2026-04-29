#include <algorithm>
#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <string>
#include <map>
#include <set>

using namespace std;
typedef pair<string, int> PAIR;

class MyMap:public map<string, int> {
public:
// 在此处补充你的代码
  struct cmp {
    bool operator() (PAIR a, PAIR b) {
      if (a.second != b.second) return a.second > b.second;
      else if (a.first.length() != b.first.length()) return a.first.length() < b.first.length();
      else return a.first < b.first;
    }
  };
  map<string, int, cmp> mym;
  friend istream& operator>> (istream& input, MyMap& mm) {
    PAIR p;
    input >> p.first >> p.second;
    mm.insert(p);
    return input;
  }
  friend ostream& operator<< (ostream& output, MyMap& mm) {
    for (auto & p : mm) {
      output << p.first << " " << p.second << endl;;
    }
    return output;
  }
};

int main()
{
	int t;
	cin >> t;
	while(t--) {
		int n;
		cin >> n;
		MyMap mm;
		for (int i = 0; i < n; ++i)
			cin >> mm;
		cout<<mm;
	}
	return 0; 
}