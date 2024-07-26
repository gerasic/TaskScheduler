class BaseResult {
public:
    virtual ~BaseResult() = default;
};

template <typename T>
class Result : public BaseResult {
public:
    Result(T&& value) : value_(std::forward<T>(value)) {}
    
    template <typename CastType>
    operator CastType() {
        return static_cast<CastType>(value_);
    }

private:
    T value_;
};
