class A {
public:
    operator int() { return 10; } // ❌
};

class B {
public:
    B(int x) {} // ❌ should be explicit
};