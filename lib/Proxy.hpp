#include <memory>

class BaseProxy {
public:
    virtual ~BaseProxy() = default;
    virtual std::shared_ptr<class BaseResult> Execute() = 0; 
};

template <typename Func, typename... Args>
class Proxy : public BaseProxy {
public:
    using FuncResult = std::invoke_result_t<Func, Args...>;

    Proxy(Func func, Args&&... args)
        : func_(func)
        , args_(std::forward<Args>(args)...) 
    {}

    std::shared_ptr<BaseResult> Execute() override {
        return std::make_shared<Result<FuncResult>>(CallFunc(std::index_sequence_for<Args...>{}));
    }

private:
    Func func_;
    std::tuple<Args...> args_;


    template <std::size_t... Is>
    auto CallFunc(std::index_sequence<Is...>) {
        return func_(std::get<Is>(args_)...);
    }
};
