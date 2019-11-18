#pragma once

#include <utility>

namespace takatori::util {

/**
 * @brief a wrapper of single parameter member function.
 * @tparam T the object type
 * @tparam R the function result type
 * @tparam P the function parameter type
 * @tparam FuncPtr the member function pointer
 */
template<class T, class R, class P, R (T::*FuncPtr)(P)>
class consumer {
public:
    /// @brief the object type.
    using object_type = T;
    /// @brief the consumer result type.
    using result_type = R;
    /// @brief the consumer parameter type.
    using parameter_type = P;
    /// @brief the pointer to consumer function.
    static constexpr result_type (object_type::*function_ptr)(parameter_type) = FuncPtr;

    /**
     * @brief creates a new object.
     * @param object the receiver object of consumer function
     */
    explicit constexpr consumer(object_type& object) noexcept : object_(std::addressof(object)) {}

    /**
     * @brief returns the object.
     * @return the receiver object of consumer function
     */
    constexpr object_type& object() const noexcept { return *object_; }

    /**
     * @brief invokes the consumer function.
     * @param argument the argument of the consumer function
     * @return the invocation result
     */
    constexpr result_type operator()(parameter_type argument) const {
        return (object_->*function_ptr)(std::forward<parameter_type>(argument));
    }

private:
    object_type* object_;
};


} // namespace takatori::util
