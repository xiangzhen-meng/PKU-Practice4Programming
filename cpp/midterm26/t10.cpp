#include <iostream>
#include <queue>
#include <vector>
using namespace std;
int n;
vector<int> a;

int main() {
  cin >> n;
  for (int i = 0, x; i < n; ++i) {
    cin >> x;
    a.push_back(x);
  }
  // 在此处补充你的代码
  struct cmp1 {
      bool operator()(int a, int b) {
        if (a % 2 == 0 && b % 2 != 0)
          return false;
        if (a % 2 != 0 && b % 2 == 0)
          return true;
        else if (a % 2 != 0 && b % 2 != 0)
          return a > b;
        else
          return a < b;
      }
  };
  priority_queue<int, vector<int>, cmp1> pq;
  // Code End
  for (int x : a)
    pq.push(x);
  while (!pq.empty()) {
    cout << pq.top() << " ";
    pq.pop();
  }
  return 0;
}