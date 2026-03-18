#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;

const int MAX = 110;

class CHugeInt {
  // 在此处补充你的代码
  private:
    char* p;
  public:
    CHugeInt(const char* s) {
      p = new char[strlen(s) + 1];
      strcpy(p, s);
    }
    CHugeInt(const int& n) {
      if (n == 0) {
        p = new char[2];
        p[0] = '0';
        p[1] = '\0';
        return;
      }
      int x = n;
      int len = 0;
      while (x) {
        x /= 10;
        len++;
      }
      p = new char[len + 1];
      p[len] = '\0';
      x = n;
      char* cur = p + len - 1;
      while (x) {
        *cur = char('0' + x % 10);
        x /= 10;
        cur--;
      }
    }
    CHugeInt(const int& len, char flag) {
      if (flag == 'l') p = new char[len];
    }
    CHugeInt(const CHugeInt& x) {
      p = new char[strlen(x.p) + 1];
      strcpy(p, x.p);
    }
    ~CHugeInt() {
      if (p) {
        delete[] p;
      }
    }
    CHugeInt operator+(const CHugeInt& b) const {
      int lena = strlen(p);
      int lenb = strlen(b.p);
      int maxLen = lena > lenb ? lena : lenb;
      char* rev = new char[maxLen + 2];

      int ia = lena - 1;
      int ib = lenb - 1;
      int carry = 0;
      int k = 0;
      while (ia >= 0 || ib >= 0 || carry) {
        int cur = carry;
        if (ia >= 0)
          cur += p[ia--] - '0';
        if (ib >= 0)
          cur += b.p[ib--] - '0';
        rev[k++] = char('0' + (cur % 10));
        carry = cur / 10;
      }

      CHugeInt tar(k + 1, 'l');
      for (int i = 0; i < k; ++i)
        tar.p[i] = rev[k - 1 - i];
      tar.p[k] = '\0';

      delete[] rev;
      return tar;
    }
    CHugeInt& operator+=(int n) {
      CHugeInt huge_n(n);
      *this = *this + huge_n;
      return *this;
    }
    CHugeInt& operator++() {
      CHugeInt n(1);
      *this = *this + n;
      return *this;
    }
    CHugeInt operator++(int) {
      CHugeInt n(1), a(*this);
      *this = *this + n;
      return a;
    }
    CHugeInt& operator=(const CHugeInt& x) {
      if (this == &x)
        return *this;
      delete[] p;
      int len = strlen(x.p);
      p = new char[len + 1];
      strcpy(p, x.p);
      return *this;
    }
    friend ostream& operator<<(ostream& output, const CHugeInt& s) {
      output << s.p;
      return output;
    }
    friend CHugeInt operator+(const int& n, const CHugeInt& a) {
      CHugeInt huge_n(n);
      huge_n = huge_n + a;
      return huge_n;
    }
};

int main() {
  char s[210];
  int n;

  while (cin >> s >> n) {
    CHugeInt a(s);
    CHugeInt b(n);

    cout << a + b << endl;
    cout << n + a << endl;
    cout << a + n << endl;
    b += n;
    cout << ++b << endl;
    cout << b++ << endl;
    cout << b << endl;
  }
  return 0;
}