#include <iostream>
#include <string>

// 在此处补充你的代码
template<class T, class F>
void MyForeach(T* start, T* end, F fp) {
  for (T* i = start; i < end; i++) {
    fp(*i);
  }
}

void Print(std::string s)
{
	std::cout << s;
}
void Inc(int & n)
{
	++ n;
}
std::string array[100];
int a[100];

int main() {
	int m,n;
	while(std::cin >> m >> n) {
		for(int i = 0;i < m; ++i)
			std::cin >> array[i];
		for(int j = 0; j < n; ++j)
			std::cin >> a[j];
		MyForeach(array,array+m,Print);		 
		std::cout << std::endl;
		MyForeach(a,a+n,Inc);		 
		for(int i = 0;i < n; ++i)
			std::cout << a[i] << ",";
		std::cout << std::endl;
	}
	return 0;
}