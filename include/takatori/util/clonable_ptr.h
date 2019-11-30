#pragma once

#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>

#include "clonable.h"
#include "copier.h"
#include "object_creator.h"

namespace takatori::util {

/**
 * @brief smart pointer that may copy their contents.
 * @tparam T the element type
 * @tparam Copier the object copying strategy
 */
template<class T, class Copier = std::disjunction<clonable_copier<T>, standard_copier<T>>>
class clonable_ptr {

    static_assert(Copier::is_available, "must be copyable");

public:
    /// @brief the element type.
    using element_type = T;
    /// @brief the L-value reference type.
    using reference = std::add_lvalue_reference_t<T>;
    /// @brief the pointer type.
    using pointer = std::add_pointer_t<T>;
    /// @brief the object copier type
    using copier_type = Copier;

    /**
     * @brief constructs a new empty object.
     * @param creator the object creator
     */
    clonable_ptr(object_creator creator = {}) noexcept : creator_(creator) {} // NOLINT

    /**
     * @brief destructs this object.
     */
    ~clonable_ptr() { reset(); }

    /**
     * @brief constructs a new object.
     * @attention This operation may create a new copy of the target object.
     * @param other the copy source
     */
    clonable_ptr(clonable_ptr const& other)
        : pointer_(other.do_copy()), creator_(other.creator_)
    {}

    /**
     * @brief assigns the given object.
     * @attention This operation may create a new copy of the target object.
     * @param other the copy source
     * @return this
     */
    clonable_ptr& operator=(clonable_ptr const& other) {
        reset();
        pointer_ = other.do_copy();
        creator_ = other.creator_;
        return *this;
    }

    /**
     * @brief constructs a new object.
     * @attention This operation just transfers the ownership of the object, so that this must not create any copies.
     * @param other the move source
     */
    clonable_ptr(clonable_ptr&& other) noexcept : pointer_(other.release()), creator_(other.creator_) {}

    /**
     * @brief assigns the given object.
     * @attention This operation just transfers the ownership of the object, so that this must not create any  copies.
     * @param other the move source
     * @return this
     */
    clonable_ptr& operator=(clonable_ptr&& other) noexcept {
        reset();
        pointer_ = other.release();
        creator_ = other.creator_;
        return *this;
    }

    /**
     * @brief constructs a new object.
     * @attention This may create a new copy if the target object is not created by the given creator.
     * @tparam U the element type
     * @tparam E the deleter type
     * @param entity the source object
     * @param creator the object creator
     */
    template<
            class U,
            class E,
            class = std::enable_if_t<std::is_constructible_v<pointer, std::add_pointer_t<U>>>>
    explicit clonable_ptr(std::unique_ptr<U, E>&& entity, object_creator creator = {}) noexcept
        : pointer_(copy_or_move(std::move(entity), creator)), creator_(creator)
    {}

    /**
     * @brief constructs a new empty object.
     * @param creator the object creator
     */
    clonable_ptr(std::nullptr_t, object_creator creator = {}) noexcept // NOLINT
        : creator_(creator)
    {}

    /**
     * @brief constructs a new object.
     * @param pointer the source object
     * @param creator the object creator
     */
    explicit clonable_ptr(T* pointer, object_creator creator = {}) noexcept
        : pointer_(pointer), creator_(creator) {}

    /**
     * @brief assigns the given object.
     * @attention This may create a new copy if the target object is not created by the given creator.
     * @tparam U the element type
     * @tparam E the deleter type
     * @param entity the source object
     * @return this
     */
    template<
            class U,
            class E,
            class = std::enable_if_t<std::is_constructible_v<pointer, std::add_pointer_t<U>>>>
    clonable_ptr& operator=(std::unique_ptr<U, E>&& entity) {
        reset();
        pointer_ = copy_or_move(std::move(entity), creator_);
        return *this;
    }

    /**
     * @brief removes the managed object.
     * @return this
     */
    clonable_ptr& operator=(std::nullptr_t) noexcept {
        reset();
        return *this;
    }

    /**
     * @brief releases ownership of the managed object and returns it as raw pointer.
     * @return the managed object
     */
    pointer release() noexcept {
        auto* result = pointer_;
        pointer_ = nullptr;
        return result;
    }

    /**
     * @brief removes the managed object.
     */
    void reset(std::nullptr_t = nullptr) noexcept {
        if (pointer_ != nullptr) {
            creator_.delete_object(pointer_);
        }
        pointer_ = nullptr;
    }

    /**
     * @brief assigns the given object.
     * @param pointer the source object
     */
    void reset(T* pointer) noexcept {
        reset();
        pointer_ = pointer;
    }

    /**
     * @brief swaps the managed objects with the given container.
     * @param other the target container
     */
    void swap(clonable_ptr& other) noexcept {
        std::swap(pointer_, other.pointer_);
        std::swap(creator_, other.creator_);
    }

    /**
     * @brief returns whether or not the this container is empty.
     * @return true if this container is empty
     * @return false otherwise
     */
    bool empty() const noexcept { return pointer_ == nullptr; }

    /**
     * @brief returns whether or not the this container has a managed object.
     * @return true if this container has a managed object
     * @return false otherwise
     */
    bool has_value() const noexcept { return !empty(); }

    /**
     * @brief returns reference of the managed object.
     * @warning undefined behavior if this container is empty
     * @return the managed object
     */
    reference value() const { return *pointer_; }

    /**
     * @brief returns pointer to the managed object.
     * @return pointer to the managed object
     * @return nullptr if this container is empty
     */
    pointer get() const noexcept { return pointer_; }

    /// @copydoc has_value()
    explicit operator bool() const noexcept { return has_value(); }

    /// @copydoc value()
    reference operator*() const { return value(); }

    /// @copydoc get()
    pointer operator->() const noexcept { return get(); }

    /**
     * @brief returns the creator.
     * @return the creator
     */
    object_creator creator() const noexcept { return creator_; }

    /**
     * @brief returns the corresponded deleter.
     * @return the corresponded deleter
     */
    object_deleter deleter() const noexcept { return creator_; }

private:
    pointer pointer_ {};
    object_creator creator_;

    template<class U, class E>
    static pointer copy_or_move(std::unique_ptr<U, E>&& ptr, object_creator creator) {
        if (!ptr) {
            return nullptr;
        }
        if (creator.is_instance(ptr)) {
            return ptr.release();
        }
        return copier_type::copy(creator, *ptr);
    }

    pointer do_copy() const {
        if (pointer_ == nullptr) {
            return nullptr;
        }
        return copier_type::copy(creator_, *pointer_);
    }
};

/// @private
template<class T> clonable_ptr(T*) -> clonable_ptr<T>;

/// @private
template<class T> clonable_ptr(T*, object_creator) -> clonable_ptr<T>;

/// @private
template<class T, class D> clonable_ptr(std::unique_ptr<T, D>&&) -> clonable_ptr<T>;

/// @private
template<class T, class D> clonable_ptr(std::unique_ptr<T, D>&&, object_creator) -> clonable_ptr<T>;

/**
 * @brief creates a new instance and wrap it by clonable_ptr.
 * @tparam T the target type
 * @tparam Args the constructor parameter types of target type
 * @param creator the object creator
 * @param args the constructor arguments
 * @return the created instance
 */
template<class T, class... Args>
inline clonable_ptr<T> make_clonable(object_creator creator, Args&&... args) {
    return clonable_ptr<T>(creator.create_object<T>(std::forward<Args>(args)...), creator);
}

/**
 * @brief exchanges the managed objects between two containers.
 * @tparam T the element type
 * @param a the first container
 * @param b the second container
 */
template<class T>
inline void swap(clonable_ptr<T>& a, clonable_ptr<T>& b) noexcept { a.swap(b); }

/**
 * @brief returns whether or not the both managed objects are identical.
 * @tparam T the element type of the first container
 * @tparam U the element type of the second container
 * @param a the first container
 * @param b the second container
 * @return true if a.get() == b.get()
 * @return false otherwise
 */
template<class T, class U>
inline bool operator==(clonable_ptr<T> const& a, clonable_ptr<U> const& b) { return a.get() == b.get(); }

/**
 * @brief returns whether or not the both managed objects are not identical.
 * @tparam T the element type of the first container
 * @tparam U the element type of the second container
 * @param a the first container
 * @param b the second container
 * @return true if a.get() != b.get()
 * @return false otherwise
 */
template<class T, class U>
inline bool operator!=(clonable_ptr<T> const& a, clonable_ptr<U> const& b) { return a.get() != b.get(); }

/**
 * @brief returns whether or not an object on the first container appears before the second one.
 * @note object on an empty container will be on the beginning of this order.
 * @tparam T the element type of the first container
 * @tparam U the element type of the second container
 * @param a the first container
 * @param b the second container
 * @return true if a.get() < b.get()
 * @return false otherwise
 */
template<class T, class U>
inline bool operator<(clonable_ptr<T> const& a, clonable_ptr<U> const& b) { return a.get() < b.get(); }

/**
 * @brief returns whether or not an object on the first container appears after the second one.
 * @note object on an empty container will be on the beginning of this order.
 * @tparam T the element type of the first container
 * @tparam U the element type of the second container
 * @param a the first container
 * @param b the second container
 * @return true if a.get() > b.get()
 * @return false otherwise
 */
template<class T, class U>
inline bool operator>(clonable_ptr<T> const& a, clonable_ptr<U> const& b) { return a.get() > b.get(); }

/**
 * @brief returns whether or not an object on the first container appears before the second one or the both are identical.
 * @note object on an empty container will be on the beginning of this order.
 * @tparam T the element type of the first container
 * @tparam U the element type of the second container
 * @param a the first container
 * @param b the second container
 * @return true if a.get() <= b.get()
 * @return false otherwise
 */
template<class T, class U>
inline bool operator<=(clonable_ptr<T> const& a, clonable_ptr<U> const& b) { return a.get() <= b.get(); }

/**
 * @brief returns whether or not an object on the first container appears after the second one or the both are identical.
 * @note object on an empty container will be on the beginning of this order.
 * @tparam T the element type of the first container
 * @tparam U the element type of the second container
 * @param a the first container
 * @param b the second container
 * @return true if a.get() >= b.get()
 * @return false otherwise
 */
template<class T, class U>
inline bool operator>=(clonable_ptr<T> const& a, clonable_ptr<U> const& b) { return a.get() >= b.get(); }

/**
 * @brief appends string representation of the given value.
 * @tparam T the value type
 * @param out the output stream
 * @param value the target value
 * @return the output stream
 */
template<class T>
inline std::ostream& operator<<(std::ostream& out, clonable_ptr<T> const& value) {
    return out << value.get();
}

} // namespace takatori::util
