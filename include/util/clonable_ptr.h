#pragma once

#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>

#include "util/clonable.h"

namespace util {

/**
 * @brief smart pointer for clonable objects.
 * @tparam T the element type, must satisfy clonable concept
 * @see clonable.h
 */
template<class T, class Deleter = std::default_delete<T>>
class clonable_ptr {

    static_assert(is_clonable_v<T>);

public:
    /// @brief the element type.
    using element_type = T;

    /// @brief the pointer type.
    using pointer = std::add_pointer_t<T>;

    /// @brief the L-value reference type.
    using reference = std::add_lvalue_reference_t<T>;

    /// @brief the deleter type.
    using deleter_type = Deleter;

    /// @brief the corresponded std::unique_ptr type.
    using storage_type = std::unique_ptr<T, Deleter>;

    /**
     * @brief constructs a new empty object.
     */
    clonable_ptr() = default;

    /**
     * @brief destructs this object.
     */
    ~clonable_ptr() = default;

    /**
     * @brief constructs a new object.
     * This operation may create a new clone of the target object, via clone() function.
     * @param other the copy source
     */
    clonable_ptr(clonable_ptr const& other) : entity_(clone(other.entity_)) {}

    /**
     * @brief assigns the given object.
     * This operation may create a new clone of the target object, via clone() function.
     * @param other the copy source
     * @return this
     */
    clonable_ptr& operator=(clonable_ptr const& other) {
        entity_ = clone(other);
        return *this;
    }

    /**
     * @brief constructs a new object.
     * This operation just transfers the ownership of the object, so that this must not create any clone objects.
     * @param other the move source
     */
    clonable_ptr(clonable_ptr&& other) noexcept = default;

    /**
     * @brief assigns the given object.
     * This operation just transfers the ownership of the object, so that this must not create any clone objects.
     * @param other the move source
     * @return this
     */
    clonable_ptr& operator=(clonable_ptr&& other) noexcept = default;

    /**
     * @brief constructs a new object.
     * @tparam U the element type
     * @tparam E the deleter type
     * @param entity the source object
     */
    template<class U, class E, class = std::enable_if_t<std::is_assignable_v<storage_type, std::unique_ptr<U, E>&&>>>
    clonable_ptr(std::unique_ptr<U, E>&& entity) noexcept : entity_(std::move(entity)) {}  // NOLINT

    /**
     * @brief constructs a new empty object.
     */
    clonable_ptr(std::nullptr_t) noexcept {} // NOLINT

    /**
     * @brief constructs a new object.
     * If a custom deleter is required, use clonable_ptr(std::unique_ptr) instead.
     * @param pointer the source object
     */
    explicit clonable_ptr(T* pointer) noexcept : entity_(pointer) {}

    /**
     * @brief assigns the given object.
     * @tparam U the element type
     * @tparam E the deleter type
     * @param entity the source object
     * @return this
     */
    template<class U, class E, class = std::enable_if_t<std::is_assignable_v<storage_type, std::unique_ptr<U, E>&&>>>
    clonable_ptr& operator=(std::unique_ptr<U, E>&& entity) noexcept {
        entity_ = std::move(entity);
        return *this;
    }

    /**
     * @brief removes the managed object.
     * @return this
     */
    clonable_ptr& operator=(std::nullptr_t) noexcept {
        entity_ = nullptr;
        return *this;
    }

    /**
     * @brief releases ownership of the managed object and returns it as raw pointer.
     * @return the managed object
     */
    pointer release() noexcept { return entity_.release(); }

    /**
     * @brief removes the managed object.
     */
    void reset(std::nullptr_t = nullptr) noexcept { entity_.reset(nullptr); }

    /**
     * @brief assigns the given object.
     * @param pointer the source object
     */
    void reset(T* pointer) noexcept { entity_.reset(pointer); }

    /**
     * @brief swaps the managed objects with the given container.
     * @param other the target container
     */
    void swap(clonable_ptr& other) noexcept { entity_.swap(other.entity_); }

    /**
     * @brief returns whether or not the this container is empty.
     * @return true if this container is empty
     * @return false otherwise
     */
    bool empty() const noexcept { return !entity_; }

    /**
     * @brief returns whether or not the this container has a managed object.
     * @return true if this container has a managed object
     * @return false otherwise
     */
    bool has_value() const noexcept { return static_cast<bool>(entity_); }

    /**
     * @brief returns reference of the managed object.
     * If this container is empty, this operation is not defined.
     * @return the managed object
     */
    reference value() const { return *entity_; }

    /**
     * @brief returns pointer to the managed object.
     * @return pointer to the managed object
     * @return nullptr if this container is empty
     */
    pointer get() const noexcept { return entity_.get(); }

    /// @copydoc has_value()
    explicit operator bool() const noexcept { return has_value(); }

    /// @copydoc value()
    reference operator*() const { return value(); }

    /// @copydoc get()
    pointer operator->() const noexcept { return get(); }

    /**
     * @brief returns the view of the managed object.
     * @return the managed object
     */
    std::add_lvalue_reference_t<storage_type> storage() & noexcept { return entity_; }

    /// @copydoc storage()
    std::add_lvalue_reference_t<std::add_const_t<storage_type>> storage() const& noexcept { return entity_; }

    /// @copydoc storage()
    std::add_rvalue_reference_t<storage_type> storage() && noexcept { return std::move(entity_); }

    /**
     * @brief releases ownership of the managed object and returns it as unique pointer.
     * @return the managed object
     */
    storage_type release_storage() noexcept { return std::move(entity_); }

private:
    storage_type entity_ {};
};

/// @private
template<class T> clonable_ptr(T*) -> clonable_ptr<T>;

/// @private
template<class T, class D> clonable_ptr(std::unique_ptr<T, D>) -> clonable_ptr<T, D>;

/**
 * @brief creates a new instance and wrap it by clonable_ptr.
 * @tparam T the target type
 * @tparam Args the constructor parameter types of target type
 * @param args the constructor arguments
 * @return the created instance
 */
template<class T, class... Args>
inline clonable_ptr<T> make_clonable(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

/**
 * @brief exchanges the managed objects between two containers.
 * @tparam T the element type
 * @tparam D the deleter type
 * @param a the first container
 * @param b the second container
 */
template<class T, class D>
inline void swap(clonable_ptr<T, D>& a, clonable_ptr<T, D>& b) noexcept { a.swap(b); }

/**
 * @brief returns whether or not the both managed objects are identical.
 * @tparam T1 the element type of the first container
 * @tparam D1 the deleter type of the first container
 * @tparam T2 the element type of the second container
 * @tparam D2 the deleter type of the second container
 * @param a the first container
 * @param b the second container
 * @return true if a.get() == b.get()
 * @return false otherwise
 */
template<class T1, class D1, class T2, class D2>
inline bool operator==(clonable_ptr<T1, D1> const& a, clonable_ptr<T2, D2> const& b) { return a.get() == b.get(); }

/**
 * @brief returns whether or not the both managed objects are not identical.
 * @tparam T1 the element type of the first container
 * @tparam D1 the deleter type of the first container
 * @tparam T2 the element type of the second container
 * @tparam D2 the deleter type of the second container
 * @param a the first container
 * @param b the second container
 * @return true if a.get() != b.get()
 * @return false otherwise
 */
template<class T1, class D1, class T2, class D2>
inline bool operator!=(clonable_ptr<T1, D1> const& a, clonable_ptr<T2, D2> const& b) { return a.get() != b.get(); }

/**
 * @brief returns whether or not an object on the first container appears before the second one.
 * Note that, object on an empty container will be on the beginning of this order.
 * @tparam T1 the element type of the first container
 * @tparam D1 the deleter type of the first container
 * @tparam T2 the element type of the second container
 * @tparam D2 the deleter type of the second container
 * @param a the first container
 * @param b the second container
 * @return true if a.get() < b.get()
 * @return false otherwise
 */
template<class T1, class D1, class T2, class D2>
inline bool operator<(clonable_ptr<T1, D1> const& a, clonable_ptr<T2, D2> const& b) { return a.get() < b.get(); }

/**
 * @brief returns whether or not an object on the first container appears after the second one.
 * Note that, object on an empty container will be on the beginning of this order.
 * @tparam T1 the element type of the first container
 * @tparam D1 the deleter type of the first container
 * @tparam T2 the element type of the second container
 * @tparam D2 the deleter type of the second container
 * @param a the first container
 * @param b the second container
 * @return true if a.get() > b.get()
 * @return false otherwise
 */
template<class T1, class D1, class T2, class D2>
inline bool operator>(clonable_ptr<T1, D1> const& a, clonable_ptr<T2, D2> const& b) { return a.get() > b.get(); }

/**
 * @brief returns whether or not an object on the first container appears before the second one or the both are identical.
 * Note that, object on an empty container will be on the beginning of this order.
 * @tparam T1 the element type of the first container
 * @tparam D1 the deleter type of the first container
 * @tparam T2 the element type of the second container
 * @tparam D2 the deleter type of the second container
 * @param a the first container
 * @param b the second container
 * @return true if a.get() <= b.get()
 * @return false otherwise
 */
template<class T1, class D1, class T2, class D2>
inline bool operator<=(clonable_ptr<T1, D1> const& a, clonable_ptr<T2, D2> const& b) { return a.get() <= b.get(); }

/**
 * @brief returns whether or not an object on the first container appears after the second one or the both are identical.
 * Note that, object on an empty container will be on the beginning of this order.
 * @tparam T1 the element type of the first container
 * @tparam D1 the deleter type of the first container
 * @tparam T2 the element type of the second container
 * @tparam D2 the deleter type of the second container
 * @param a the first container
 * @param b the second container
 * @return true if a.get() >= b.get()
 * @return false otherwise
 */
template<class T1, class D1, class T2, class D2>
inline bool operator>=(clonable_ptr<T1, D1> const& a, clonable_ptr<T2, D2> const& b) { return a.get() >= b.get(); }

/**
 * @brief appends string representation of the given value.
 * @tparam T the value type
 * @tparam D the deleter type
 * @param out the output stream
 * @param value the target value
 * @return the output stream
 */
template<class T, class D>
inline std::ostream& operator<<(std::ostream& out, clonable_ptr<T, D> const& value) {
    return out << value.get();
}

} // namespace util
