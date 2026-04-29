#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>
#include <map>
using namespace std;

class MyMap{
// 在此处补充你的代码
  private:
    map<int, vector<string>, greater<int> > scoreOut;
    multimap<string, int> scoreIn;
  public:
    friend istream& operator>>(istream& input, MyMap& mm) {
      string name;
      int score;
      input >> name >> score;
      mm.scoreIn.insert(make_pair(name, score));
      return input;
    }
    void getOut() {
      for (auto & p : scoreIn) {
        string name = p.first;
        int score = p.second;
        scoreOut[score].push_back(name);
      }
    }
    friend ostream& operator<<(ostream& output, MyMap& mm) {
      mm.getOut();
      for (auto& p : mm.scoreOut) {
        int score = p.first;
        auto q = p.second;
        output << score << " ";
        sort(q.begin(), q.end(), less<string>());
        for (auto i = q.begin(); i != q.end(); i++) {
          if (*i == *(i - 1)) continue;
          output << *i << " ";
        }
        output << endl;
      }
      return output;
    }
// Code End
};

int main() {
    int n;
    cin >> n;
    MyMap mm;
    for (int i = 0; i < n; ++i)
        cin >> mm;
    cout<<mm;
    return 0; 
}