#include <cstring>
#include <iostream>
#include <string>
using namespace std;

class MyString {
    char *p;

  public:
    MyString(const char *s) {
      if (s) {
        p = new char[strlen(s) + 1];
        strcpy(p, s);
      } else
        p = NULL;
    }
    ~MyString() {
      if (p)
        delete[] p;
    }
    // 在此处补充你的代码
    MyString(const MyString &s) {
      if (s.p) {
        p = new char[strlen(s.p) + 1];
        strcpy(p, s.p);
      } else {
        p = NULL;
      }
    }
    friend ostream& operator<<(ostream& output, const MyString& s) {
      output << s.p;
      return output;
    }
    MyString& operator=(const MyString& s) {
      if (this == &s)
        return *this;
      delete[] p;
      if (s.p) {
        p = new char[strlen(s.p) + 1];
        strcpy(p, s.p);
      } else {
        p = NULL;
      }
      return *this;
    }
    MyString& Copy(const char* s) {
      int len = strlen(s);
      delete[] p;
      p = new char[len + 1];
      strcpy(p, s);
      return *this;
    }
    MyString& operator=(const char* s) {
      delete[] p;
      int len = strlen(s);
      p = new char[len + 1];
      strcpy(p, s);
      return *this;
    }

};

int main() {
  char w1[200], w2[100];
  while (cin >> w1 >> w2) {
    MyString s1(w1), s2 = s1;
    MyString s3(NULL);
    s3.Copy(w1);
    cout << s1 << "," << s2 << "," << s3 << endl;

    s2 = w2;
    s3 = s2;
    s1 = s3;
    cout << s1 << "," << s2 << "," << s3 << endl;
  }
  return 0;
}