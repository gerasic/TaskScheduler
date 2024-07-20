#include <memory>


class Any {
public:
    Any() : value_(new Inner<int>(0)){} 

    template <typename T> 
    Any(T src) : value_(new Inner<T>(std::forward<T>(src))) {}

    template <typename T> 
    Any& operator=(T src) { 
        value_ = std::make_unique<Inner<T>>(std::forward<T>(src));
        return *this;
    }

    template <typename T> 
    operator T&() {
        return *dynamic_cast<Inner<T>&>(*value_);
    }

private:
    struct InnerBase {
        using Pointer = std::unique_ptr<InnerBase>;
        virtual ~InnerBase() {}
    };

    template <typename T> struct Inner : InnerBase {
    public:
        Inner(T newval) : _value(std::move(newval)) {}

        T& operator* () {
            return _value; 
        }

        const T& operator* () const {
            return _value; 
        }

    private:
        T _value;
    };

    InnerBase::Pointer value_;
};