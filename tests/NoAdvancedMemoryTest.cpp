struct A {
    int x;
};

// ❌ Placement new (simulated without <new>)
void* operator new(unsigned long, void* p) { return p; }

void bad1(void* mem) {
    int* p = new (mem) int(5); // ❌
}

// ❌ Direct operator new call
void bad2() {
    void* p = ::operator new(4); // ❌
}

// ❌ Custom operator delete
void operator delete(void* p) {
}

// ✅ Normal usage
void good() {
    int x = 5;
}