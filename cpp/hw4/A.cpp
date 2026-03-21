#include <cstdlib>
#include <iostream>
using namespace std;

/*
int strlen(const char *s) {
  int i = 0;
  for (; s[i]; ++i)
    ;
  return i;
}
void strcpy(char *d, const char *s) {
  int i = 0;
  for (i = 0; s[i]; ++i)
    d[i] = s[i];
  d[i] = 0;
}*/
int strcmp(const char *s1, const char *s2) {
  for (int i = 0; s1[i] && s2[i]; ++i) {
    if (s1[i] < s2[i])
      return -1;
    else if (s1[i] > s2[i])
      return 1;
  }
  return 0;
}/*
void strcat(char *d, const char *s) {
  int len = strlen(d);
  strcpy(d + len, s);
}
*/ 

class MyString {
  // 在此处补充你的代码
  private:
    char* s;
    int len;
  public:
    MyString(): len(0) {
      s = new char[20];
    }
    MyString(const char* inp) :len(strlen(inp)) {
      s = new char[len + 1];
      strcpy(s, inp);
    }
    MyString(int plen): len(plen) {
      s = new char[len + 1];
      s[0] = 0;
    }
    MyString(const MyString& a) : len(a.len) {
      s = new char[a.len + 1];
      strcpy(s, a.s);
    }
    ~MyString() {
      delete[] s;
    }

    MyString& operator=(const MyString a){
      delete[] s;
      len = a.len;
      s = new char[len + 1];
      strcpy(s, a.s);
      return *this;
    }
    bool operator<(const MyString a) {
      return strcmp(this->s, a.s) == -1;
    }
    bool operator==(const MyString a) {
      return strcmp(this->s, a.s) == 0;
    }
    bool operator>(const MyString a) {
      return strcmp(this->s, a.s) == 1;
    }
    MyString operator+(const MyString a) {
      int lensum = len + a.len;
      MyString ret(lensum);
      strcat(ret.s, this->s);
      strcat(ret.s, a.s);
      return ret;
    }
    friend MyString operator+(const char* a, MyString b) {
      MyString c(a);
      return c + b;
    }
    MyString& operator+=(const char* a) {
      MyString b(a);
      *this = *this + b;
      return *this;
    }

    char& operator[](int idx) {
      return *(s + idx);
    }
    MyString operator()(int start, const int length) const {
      MyString ret(length);
      for (int i = 0; i < length; i++) {
        ret.s[i] = s[start + i];
      }
      ret.s[length] = 0;
      return ret;
    }

    friend ostream& operator<<(ostream& output, const MyString& a) {
      output << a.s;
      return output;
    }
};

int CompareString(const void *e1, const void *e2) {
  MyString *s1 = (MyString *)e1;
  MyString *s2 = (MyString *)e2;
  if (*s1 < *s2)
    return -1;
  else if (*s1 == *s2)
    return 0;
  else if (*s1 > *s2)
    return 1;
  return 0;
}

int main() {
  MyString s1("abcd-"), s2, s3("efgh-"), s4(s1);
  MyString SArray[4] = {"big", "me", "about", "take"};
  cout << "1. " << s1 << s2 << s3 << s4 << endl;
  s4 = s3;
  s3 = s1 + s3;
  cout << "2. " << s1 << endl;
  cout << "3. " << s2 << endl;
  cout << "4. " << s3 << endl;
  cout << "5. " << s4 << endl;
  cout << "6. " << s1[2] << endl;
  s2 = s1;
  s1 = "ijkl-";
  s1[2] = 'A';
  cout << "7. " << s2 << endl;
  cout << "8. " << s1 << endl;
  s1 += "mnop";
  cout << "9. " << s1 << endl;
  s4 = "qrst-" + s2;
  cout << "10. " << s4 << endl;
  s1 = s2 + s4 + " uvw " + "xyz";
  cout << "11. " << s1 << endl;
  qsort(SArray, 4, sizeof(MyString), CompareString);
  for (int i = 0; i < 4; i++)
    cout << SArray[i] << endl;
  // s1的从下标0开始长度为4的子串
  cout << s1(0, 4) << endl;
  // s1的从下标5开始长度为10的子串
  cout << s1(5, 10) << endl;
  return 0;
}