class A {
    int* data;
public:
    A& operator=(const A& other) {
        delete data;
        data = new int(*other.data);
        return *this;
    }
};