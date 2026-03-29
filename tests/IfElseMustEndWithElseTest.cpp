int foo(int x) {
    if (x == 0) {
        return 0;
    } else if (x == 1) {
        return 1;
    } // ❌ Missing final else
}

int bar(int x) {
    if (x == 0) {
        return 0;
    } else if (x == 1) {
        return 1;
    } else {
        return -1;
    } // ✅ Correct
}

int baz(int x) {
    if (x == 5) {
        return 5;
    } // ✅ Not part of else-if chain
}