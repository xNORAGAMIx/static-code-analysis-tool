class A {
public:
    int* p;

    void set() {
        int x = 5;
        p = &x; // ❌
    }
};