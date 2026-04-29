#include <iostream>
using namespace std;
class MyArray {
  public:
    int *a;
    int n;
    MyArray(int _n, int A[]) : n(_n) {
      a = new int[n];
      for (int i = 0; i < n; i++)
        a[i] = A[i];
    }
    ~MyArray() { delete[] a; }
    MyArray operator++(int) {
      MyArray ret = MyArray(n, a);
      for (int i = 0; i < n; i++) {
        a[i] = a[i] + 1;
      }
      return ret;
    }
    MyArray operator++() {
      for (int i = 0; i < n; i++) {
        a[i] = a[i] + 1;
      }
      MyArray ret = MyArray(n, a);
      for (int i = 0; i < n; i++) {
        ret.a[i] = ret.a[i];
      }
      return ret;
    }
    friend ostream &operator<<(ostream &output, const MyArray &m) {
      for (int i = 0; i < m.n; i++) {
        output << m.a[i] << " ";
      }
      return output;
    }
    void operator-=(int x) {
      for (int i = 0; i < n; i++) {
        a[i] -= x;
      }
    }
    friend MyArray operator+(int x, const MyArray &m) {
      MyArray ret = MyArray(m.n, m.a);
      // for (int i = 0; i < m.n; i++) {
      //   cerr << "ERR" << ret.a[i];
      // }
      for (int i = 0; i < m.n; i++) {
        ret.a[i] = ret.a[i] + x;
      }
      return ret;
    }
    int &get_first() { return a[0]; }
    int get_last() { return a[n - 1]; }
};

int main() {
  int n;
  cin >> n;
  int a[105] = {0};
  for (int i = 0; i < n; i++)
    cin >> a[i];
  MyArray arr(n, a);

  cout << arr++ << endl;            // 对数组中所有元素+1，输出修改前的元素
  cout << ++arr << endl;            // 对数组中所有元素+1，输出修改后的元素
  arr -= 5;                         // 对数组中所有元素-5
  cout << 3 + arr << endl;          // 输出数组中所有元素+3的值
  arr.get_first() = arr.get_last(); // 将第一个元素值修改为改为最后一个元素的值
  cout << arr << endl;
  return 0;
}