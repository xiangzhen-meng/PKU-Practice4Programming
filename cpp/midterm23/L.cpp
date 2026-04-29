#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

struct lesson {
  map<string, int> stuList;
};

map<string, lesson> lessonList;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  while (n--) {
    string lesname, stuname;
    int score;
    cin >> lesname >> stuname >> score;
    lessonList[lesname].stuList[stuname] = score;
  }

  int m;
  cin >> m;
  while (m--) {
    int x;
    cin >> x;

    vector<string> queryLessons(x);
    for (int i = 0; i < x; ++i) {
      cin >> queryLessons[i];
    }

    map<string, int> scoreSum;
    if (x > 0) {
      auto it0 = lessonList.find(queryLessons[0]);
      if (it0 != lessonList.end()) {
        scoreSum = it0->second.stuList;
      }

      for (int i = 1; i < x && !scoreSum.empty(); ++i) {
        auto itLesson = lessonList.find(queryLessons[i]);
        if (itLesson == lessonList.end()) {
          scoreSum.clear();
          break;
        }

        for (auto itStu = scoreSum.begin(); itStu != scoreSum.end();) {
          auto itScore = itLesson->second.stuList.find(itStu->first);
          if (itScore == itLesson->second.stuList.end()) {
            itStu = scoreSum.erase(itStu);
          } else {
            itStu->second += itScore->second;
            ++itStu;
          }
        }
      }
    }

    string output;
    int best = -1;
    for (const auto &p : scoreSum) {
      if (p.second > best) {
        best = p.second;
        output = p.first;
      }
    }
    cout << output << '\n';
  }

  return 0;
}