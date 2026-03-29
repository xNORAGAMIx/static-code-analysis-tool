class Base {
public:
    virtual void foo() {}
};

class Derived : public Base {
public:
    Derived() {
        foo(); // ❌ violation
    }

    void foo() override {}
};