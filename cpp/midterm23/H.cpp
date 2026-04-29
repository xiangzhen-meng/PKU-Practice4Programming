// int 重载格式
// 59 行 string 怎么转换到 MyString 的？
// 这几个 const 加不加有事吗? 没啥事
#include <iostream>
#include <string>
using namespace std;

template <class T>
void print(T a) {
    cout << a << endl;
}

class MyString {
  public:
    string m_data;
    MyString(string a) : m_data(a) {}
    // 在此处补充你的代码
    operator string() {
      return m_data;
    }
    friend ostream& operator<< (ostream& output, MyString& m) {
      output << m.m_data;
      return output;
    }
    string operator+ (MyString& other) {
      return m_data + other.m_data;
    }
};

template<class T>
class MyTemplateClass {
  private:
    T val;
  public:
    MyTemplateClass(T x) : val(x) {}
    friend ostream& operator<< (ostream& output, MyTemplateClass& x) {
      output << x.val + x.val;
      return output;
    }
    operator int () {
      return int(val);
    }
    operator MyString() {
      return val;
    }
};
// Code End

int main()
{
    int m;
    int num;
    string str;
    cin >> m;
    for (; m>0; m--) {
        cin >> num >> str;
        MyTemplateClass<int> obj(num);
        print(int(obj));
        print(obj);
        MyTemplateClass<MyString> obj2(str);
        print(MyString(obj2));
        print(obj2);
    }
}