template <typename T>
struct MyType {
    MyType(T val) : value_(new T(val)) {}

    MyType(const MyType& other) {
        value_ = new T(*other.value_);
    }

    MyType& operator=(const MyType& other) {
        if (this == &other) {
            return;
        }

        delete value_;
        value_ = new T(*other.value_);

        return *this;
    }

    ~MyType() {
        delete value_;
    }

    T GetValue() const {
        return *value_;
    }
private:
    T* value_;  
};

template <typename T>
MyType<T> operator+(const MyType<T>& mt1, const MyType<T>& mt2) {
    return {mt1.GetValue() + mt2.GetValue()};
}