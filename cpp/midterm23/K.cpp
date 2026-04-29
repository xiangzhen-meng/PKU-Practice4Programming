#include <iostream>
using namespace std;
struct Network {
	int totalConnections;
	void lostAllConnections() {
		cout << "lostAllConnections" << endl;
	}
	Network():totalConnections(0) { }
	void print() {
		cout << totalConnections << endl;
	}
};
class Computer {
// 在此处补充你的代码
  private:
    Network& net;
    bool connected = false;
  public:
    Computer(Network& n) : net(n), connected(true) {
      net.totalConnections++;
    }
    Computer(Computer& c) : net(c.net) {
      if (c.connected){
        connected = true;
        net.totalConnections++;
      }
    }
    void connect(Network& n) {
      if (!connected) {
        net = n;
        net.totalConnections++;
        connected = true;
      }
    }
    void disConnect() {
      net.totalConnections--;
      connected = false;
      if (net.totalConnections == 0) net.lostAllConnections();
    }
    Computer& operator=(Computer& other) {
      if (!connected) {
        net = other.net;
        net.totalConnections++;
        connected = true;
      }
      return *this;
    }
    ~Computer() {
      if (connected) {
        net.totalConnections--;
        if (net.totalConnections == 0) net.lostAllConnections();
      }
    }
// Code End
};
int main()
{
	Network net;
	Computer c1(net);
	net.print();
	c1.disConnect();
	Computer c2(c1);
	net.print();
	c1.connect(net);
	c2 = c1;
	net.print();
	Computer * pc3 = new Computer(c2);
	net.print();
	c1.disConnect();
	net.print();
	delete pc3;
	net.print();
	return 0;
}