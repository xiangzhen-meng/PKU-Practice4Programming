#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

class Student {
  public:
    void input() {
      string line;
      getline(cin, line);
      stringstream ss(line);

      string token;
      getline(ss, token, ',');
      name = token;
      getline(ss, token, ',');
      age = stoi(token);
      getline(ss, token, ',');
      id = stoi(token);
      for (int i = 0; i < 4; i++) {
        getline(ss, token, ',');
        score[i] = stoi(token);
      }
    }
    void calculate() {
      int sum = 0;
      for (int i = 0; i < 4; i++) {
        sum += score[i];
      }
      avg_score = ((double)sum) / 4;
    }
    void output() {
      cout << name << "," << age << "," << id << "," << avg_score << endl;
    }
  private:
    string name;
    int age;
    int id;
    int score[4];
    double avg_score;
};

int main() {
  Student student;     // 定义类的对象
  student.input();     // 输入数据
  student.calculate(); // 计算平均成绩
  student.output();    // 输出数据
}