#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<string> strs;

string getString();
int getInt();

bool isNumber(const string &s, int &val) {
  if (s.empty() || s.length() > 5)
    return false;
  for (char c : s)
    if (!isdigit(c))
      return false;
  val = stoi(s);
  return val >= 0 && val <= 99999;
}

int getInt() {
  string op;
  cin >> op;
  if (op == "find") {
    string s = getString();
    int n = getInt();
    size_t pos = strs[n - 1].find(s);
    return (pos == string::npos) ? strs[n - 1].length() : (int)pos;
  } else if (op == "rfind") {
    string s = getString();
    int n = getInt();
    size_t pos = strs[n - 1].rfind(s);
    return (pos == string::npos) ? strs[n - 1].length() : (int)pos;
  } else {
    return stoi(op); 
  }
}

string getString() {
  string op;
  cin >> op;
  if (op == "copy") {
    int n = getInt();
    int x = getInt();
    int l = getInt();
    return strs[n - 1].substr(x, l);
  } else if (op == "add") {
    string s1 = getString();
    string s2 = getString();
    int v1, v2;
    if (isNumber(s1, v1) && isNumber(s2, v2)) {
      return to_string(v1 + v2);
    } else {
      return s1 + s2;
    }
  } else {
    return op;
  }
}

int main() {
  int n;
  cin >> n;
  for (int i = 0; i < n; ++i) {
    string s;
    cin >> s;
    strs.push_back(s);
  }

  string cmd;
  while (cin >> cmd && cmd != "over") {
    if (cmd == "copy") { 
    } else if (cmd == "insert") {
      string s = getString();
      int n = getInt();
      int x = getInt();
      strs[n - 1].insert(x, s);
    } else if (cmd == "reset") {
      string s = getString();
      int n = getInt();
      strs[n - 1] = s;
    } else if (cmd == "print") {
      int n = getInt();
      cout << strs[n - 1] << endl;
    } else if (cmd == "printall") {
      for (const auto &s : strs)
        cout << s << endl;
    }
  }
  return 0;
}