// Manual declarations (no includes)
extern "C" void *memcpy(void *dest, const void *src, unsigned long n);

namespace std {
    template <typename T>
    T* copy(T* first, T* last, T* d_first);
}

void valid_cases() noexcept {
    int a[10] = {0};
    int b[10] = {0};

    memcpy(b, a, sizeof(a)); // OK

    int c[10] = {0};
    int d[10] = {0};

    std::copy(c, c + 5, d); // OK
}

void invalid_cases() noexcept {
    int arr[10] = {0};

    // ❌ overlapping memcpy
    memcpy(arr + 1, arr, 5 * sizeof(int));

    // ❌ overlapping std::copy
    std::copy(arr, arr + 5, arr + 2);
}

int main() {
    valid_cases();
    invalid_cases();
    return 0;
}