struct A {
    int x;
};

void bad() {
    int* p = new int(5);   // ❌
    delete p;              // ❌
}

void also_bad() {
    A* obj = new A();      // ❌
}

void good() {
    int value = 10;        // ✅ no dynamic allocation
}

void also_good() {
    A obj;                 // ✅ stack allocation
}

void placement_ok() {
    void* mem = operator new(sizeof(int));
    int* p = new (mem) int(5); // ✅ placement new (ignored)
}