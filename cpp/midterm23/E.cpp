// 有点问题，必须要把所有的东西都查重
// 继承 multimap<> 有问题吗？
#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>
#include <map>
using namespace std;

class MyMap: public multimap<string, int> {
// 在此处补充你的代码
  private:
    multimap<string, int> inmap;
    map<int, vector<string>, greater<int> > outmap;
  public:
    MyMap() {}
    friend istream& operator>>(istream& input, MyMap& mm) {
      string name;
      int score;
      input >> name >> score;
      mm.inmap.insert(make_pair(name, score));
      return input;
    }
    void getout() {
      for (auto & p : inmap) {
        string name = p.first;
        int score = p.second;
        outmap[score].push_back(name);
      }
    }
    friend ostream& operator<<(ostream& output, MyMap& mm) {
      mm.getout();
      for (auto & p : mm.outmap) {
        sort(p.second.begin(), p.second.end(), less<string>());
        output << p.first << " ";
        if (!p.second.empty()) output << *p.second.begin() << " ";
        for (auto i = p.second.begin() + 1; i != p.second.end(); i++) {
          if (*i != *(i - 1)) output << *i << " ";
        }
        output << endl;
      }
      return output;
    }
// Code End
};

int main()
{
    int n;
    cin >> n;
    MyMap mm;
    for (int i = 0; i < n; ++i)
        cin >> mm;
    cout<<mm;
	return 0; 
}