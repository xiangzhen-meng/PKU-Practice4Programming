#include <iostream>
using namespace std;
// 在此处补充你的代码
template<class T1, class T2>
class f {
   private:
      T1 tail;
   public:
      f(T1 t) : tail(t) {}
      T2 operator () (T2 x) {
         return x + tail;
      }
};
// Code End
int main()
{
   cout << f<int,int>(7)(9) << endl;   //16
   cout << f<string,string> (" hello!")("world")  <<endl; // world hello!
   cout << f<char,string> ('!')("world") << endl;
   
   
   return 0;    //world!
}