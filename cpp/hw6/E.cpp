#include <iostream>
#include <string>

using namespace std;
template <class T>
class CMyistream_iterator{
  // 在此处补充你的代码
  private:
    istream& input;
    T* val;
  public:
    CMyistream_iterator(istream& inp) : input(inp) {
      val = new T;
      input >> *val;
    }
    ~CMyistream_iterator() {
      delete val;
    }
    T operator*() {
      return *val;
    }
    void operator++(int) {
      input >> *val;
    }
};



int main()  
{ 
	int t;
	cin >> t;
	while( t -- ) {
		 CMyistream_iterator<int> inputInt(cin);
		 int n1,n2,n3;
		 n1 = * inputInt; //读入 n1
		 int tmp = * inputInt;
		 cout << tmp << endl;
		 inputInt ++;   
		 n2 = * inputInt; //读入 n2
		 inputInt ++;
		 n3 = * inputInt; //读入 n3
		 cout << n1 << " " << n2<< " " << n3 << " ";
		 CMyistream_iterator<string> inputStr(cin);
		 string s1,s2;
		 s1 = * inputStr;
		 inputStr ++;
		 s2 = * inputStr;
		 cout << s1 << " " << s2 << endl;
	}
	 return 0;  
}