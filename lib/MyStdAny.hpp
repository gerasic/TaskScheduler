#include <memory>

#include "Result.hpp"
#include "Proxy.hpp"

class Any {
public:
    template <typename Func, typename... Args>
    Any(Func func, Args&&... args)
        : value_(std::make_shared<Proxy<Func, Args...>>(func, std::forward<Args>(args)...))
    {}

    std::shared_ptr<BaseResult> Execute() const {
        return value_->Execute();
    }

private:
    std::shared_ptr<BaseProxy> value_;
};
