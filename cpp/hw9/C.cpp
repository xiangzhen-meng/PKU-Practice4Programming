#include <iostream>
#include <cstring>
using namespace std;
template <int bitNum>
struct MyBitset 
{
	char a[bitNum/8+1];
	MyBitset() { memset(a,0,sizeof(a));};
	void Set(int i,int v) {
		char & c = a[i/8];
		int bp = i % 8;
		if( v ) 
			c |= (1 << bp);
		else 
			c &= ~(1 << bp);
	}
  // 在此处补充你的代码
  int Get(int i) {
    char c = a[i/8];
    int bp = i % 8;
    return (c >> bp) & 1;
  }
  class transfer {
    private:
      MyBitset& bs;
      int idx;
    public:
      transfer(MyBitset& b, int i) : bs(b), idx(i) {}
      transfer& operator= (int v) {
        bs.Set(idx, v);
        return *this;
      }
      transfer& operator= (const transfer& t) {
        bs.Set(idx, (int)t);
        return *this;
      }
      operator int() const {
        return bs.Get(idx);
      }
      friend ostream& operator<< (ostream& output, transfer& t) {
        output << (int)t;
        return output;
      }
  };

  transfer operator[] (int i) {
    return transfer(*this, i);
  }
  // Code End

  void Print() {
		for(int i = 0;i < bitNum; ++i) 
			cout << (*this) [i];
		cout << endl;
	}
};

int main()
{
	int n;
	int i,j,k,v;
	while( cin >>  n) {
		MyBitset<20> bs;
		for(int i = 0;i < n; ++i) {
			int t;
			cin >> t;
			bs.Set(t,1);
		}
		bs.Print();
		cin >> i >> j >> k >> v;
		bs[k] = v;
		bs[i] = bs[j] = bs[k];
		bs.Print();
		cin >> i >> j >> k >> v;
		bs[k] = v;
		(bs[i] = bs[j]) = bs[k];
		bs.Print();
	}
	return 0;
}