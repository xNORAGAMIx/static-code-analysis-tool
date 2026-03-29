class A {
public:
    A& get() { // ❌ no ref qualifier
        return *this;
    }
};