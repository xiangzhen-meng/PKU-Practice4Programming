#include <iostream>
using namespace std;

class A {
protected:
    int value;
public:
    // Constructor
    A(int v = 0) : value(v) {}
    
    // Virtual destructor for proper inheritance cleanup
    virtual ~A() = default;
    
    virtual int get_value() const { 
        return value; 
    }
    
    // Overload operator+ to handle: Object + Pointer
    // Returns a new object of type A containing the sum
    A operator+(A* other) const {
        return A(this->value + other->get_value());
    }
    
    // Overload operator-> to return a pointer to the current object.
    // This makes the syntax (ResultObj)->get_value() valid.
    A* operator->() {
        return this;
    }
};

class B : public A {
public:
    B(int v) : A(v) {}
};

class C : public A {
public:
    C(int v) : A(v) {}
};

int main() {
    int t;
    cin >> t;
    while (t--) {
        int aa, bb, cc;
        cin >> aa >> bb >> cc;
        A a(aa);
        B b(bb);
        C c(cc);

        A* x = &a;
        A* y = &b;
        A* z = &c;

        cout << (x->get_value() + y->get_value() + z->get_value()) << " ";
        cout << ((*x) + y + z)->get_value() << endl;
    }
    return 0;
}