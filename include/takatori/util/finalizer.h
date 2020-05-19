#pragma once

#include <functional>

namespace takatori::util {

/**
 * @brief calls a function on dispose.
 */
class finalizer {
public:
    /**
     * @brief creates a new instance which does nothing.
     */
    explicit finalizer() noexcept = default;

    /**
     * @brief creates a new instance.
     * @param disposer the disposer
     */
    explicit finalizer(std::function<void()> disposer) noexcept
        : disposer_(std::move(disposer))
    {}

    ~finalizer() {
        disposer_();
    }

    finalizer(finalizer const&) = delete;
    finalizer& operator=(finalizer const&) = delete;

    /**
     * @brief creates a new instance.
     * @param other the move source
     */
    finalizer(finalizer&& other) = default;

    /**
     * @brief assigns the given object.
     * @param other the move source
     * @return this
     */
    finalizer& operator=(finalizer&& other) = default;

    /**
     * @brief cancels the current finalize action and replace it with the given action.
     * @param disposer the disposer
     * @return this
     */
    finalizer& operator=(std::function<void()> disposer) noexcept {
        disposer_ = std::move(disposer);
        return *this;
    }

private:
    std::function<void()> disposer_ {};
};

} // namespace takatori::util
