#include <vector>

#include "MyStdAny.hpp"
#include "FutureResult.hpp"

class TaskScheduler {
    struct Index {
    private:
        Index(size_t val) : idx_(val) {}
        size_t idx_;
        friend TaskScheduler;
    };

public:
    template <typename Func, typename... Args>
    Index Add(Func func, Args&&... args) {
        Index index{tasks_.size()};

        tasks_.push_back({func, std::forward<Args>(args)...});

        return index;
    }

    template <typename T>
    FutureResult<T> GetFutureResult(const Index& index) {
        CheckIndex(index.idx_);

        return {tasks_, results_, index.idx_};
    }

    template <typename T>
    T GetResult(Index index) {
        CompleteTask(index.idx_);

        auto result = dynamic_cast<Result<T>*>(results_[index.idx_].get());

        return static_cast<T>(*result);
    }

private:
    std::vector<Any> tasks_;
    std::vector<std::shared_ptr<BaseResult>> results_;

    void CompleteTask(size_t index) {
        CheckIndex(index);
        
        if (!results_[index]) {
            results_[index] = tasks_[index].Execute();
        }
    }

    void CheckIndex(size_t index) {
        if (index >= tasks_.size()) {
            throw std::out_of_range("No index " + std::to_string(index) + " in tasks_!");
        }

        if (index >= results_.size()) {
            results_.resize(index + 1, nullptr);
        }
    }
};
