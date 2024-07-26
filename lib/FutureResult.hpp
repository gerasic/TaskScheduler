template <typename T>
class FutureResult {
public:
    FutureResult(
        const std::vector<Any>& tasks, 
        std::vector<std::shared_ptr<BaseResult>>& results, 
        const size_t& index)

        : tasks_(tasks)
        , results_(results)
        , index_(index)
    {}

    operator T() {
        CompleteTask(index_);

        auto result = dynamic_cast<Result<T>*>(results_[index_].get());

        return static_cast<T>(*result);
    }

private:
    const std::vector<Any>& tasks_;
    std::vector<std::shared_ptr<BaseResult>>& results_;
    const size_t& index_;

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
