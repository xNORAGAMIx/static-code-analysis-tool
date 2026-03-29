#include "stdexcept"

void bad() {
    int x = 10;
    x++;  // should warn
}

void good() noexcept {
    int x = 10;
    x++;
}

void throws_func() {
    throw 1;  // valid (no warning)
}

void calls_throwing() {
    throws_func();  // should NOT warn (if rule is improved)
}

int safe_add(int a, int b) {
    return a + b;  // should warn
}

void guarded(int *ptr) {
    if (!ptr) return;
    *ptr = 10;  // irrelevant to this rule, but valid C++
}

int main() {
    bad();
    good();
    safe_add(1, 2);
    calls_throwing();
    guarded(nullptr);
    return 0;
}