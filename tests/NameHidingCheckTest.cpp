class Base {
public:
    void foo(int);
};

class Derived : public Base {
public:
    void foo(double); // ❌ hides base function
};