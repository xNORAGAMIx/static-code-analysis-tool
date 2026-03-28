int foo() { return 1; }

void test() {
    foo();          // ❌
    int x;          // ❌
    int y = 10;     // ❌
    int z = foo();  // ✅
}