#pragma once

#include <ostream>
#include <memory>
#include <type_traits>
#include <utility>

#include "clonable.h"
#include "copier.h"

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
    /// @brief the entity type.
    using entity_type = std::unique_ptr<element_type>;

    /**
     * @brief constructs a new empty object.
     */
    clonable_ptr() noexcept = default;

    /**
     * @brief destructs this object.
     */
    ~clonable_ptr() { reset(); }

    /**
     * @brief constructs a new object.
     * @attention This operation may create a new copy of the target object.
     * @param other the copy source
     */
    clonable_ptr(clonable_ptr const& other) :
        entity_ { other.do_copy() }
    {}

    /**
     * @brief assigns the given object.
     * @attention This operation may create a new copy of the target object.
     * @param other the copy source
     * @return this
     */
    clonable_ptr& operator=(clonable_ptr const& other) {
        entity_ = other.do_copy();
        return *this;
    }

    /**
     * @brief constructs a new object.
     * @attention This operation just transfers the ownership of the object, so that this must not create any copies.
     * @param other the move source
     */
    clonable_ptr(clonable_ptr&& other) noexcept = default;

    /**
     * @brief assigns the given object.
     * @attention This operation just transfers the ownership of the object, so that this must not create any  copies.
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
    template<
            class U,
            class E,
            class = std::enable_if_t<std::is_constructible_v<pointer, std::add_pointer_t<U>>>>
    explicit clonable_ptr(std::unique_ptr<U, E>&& entity) noexcept :
        entity_ { copy_or_move(std::move(entity)) }
    {}

    /**
     * @brief constructs a new empty object.
     */
    clonable_ptr(std::nullptr_t) noexcept : // NOLINT
        clonable_ptr {}
    {}

    /**
     * @brief constructs a new object.
     * @param pointer the source object
     */
    explicit clonable_ptr(T* pointer) noexcept :
        entity_ { pointer }
    {}

    /**
     * @brief assigns the given object.
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
        entity_ = copy_or_move(std::move(entity));
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
        return entity_.release();
    }

    /**
     * @brief removes the managed object.
     */
    void reset(std::nullptr_t = nullptr) noexcept {
        entity_.reset();
    }

    /**
     * @brief assigns the given object.
     * @param pointer the source object
     */
    void reset(T* pointer) noexcept {
        entity_.reset(pointer);
    }

    /**
     * @brief swaps the managed objects with the given container.
     * @param other the target container
     */
    void swap(clonable_ptr& other) noexcept {
        std::swap(entity_, other.entity_);
    }

    /**
     * @brief returns whether or not the this container is empty.
     * @return true if this container is empty
     * @return false otherwise
     */
    [[nodiscard]] bool empty() const noexcept { return !entity_; }

    /**
     * @brief returns whether or not the this container has a managed object.
     * @return true if this container has a managed object
     * @return false otherwise
     */
    [[nodiscard]] bool has_value() const noexcept { return !empty(); }

    /**
     * @brief returns reference of the managed object.
     * @warning undefined behavior if this container is empty
     * @return the managed object
     */
    [[nodiscard]] reference value() const { return *entity_; }

    /**
     * @brief returns pointer to the managed object.
     * @return pointer to the managed object
     * @return nullptr if this container is empty
     */
    [[nodiscard]] pointer get() const noexcept { return entity_.get(); }

    /// @copydoc has_value()
    [[nodiscard]] explicit operator bool() const noexcept { return has_value(); }

    /// @copydoc value()
    [[nodiscard]] reference operator*() const { return value(); }

    /// @copydoc get()
    [[nodiscard]] pointer operator->() const noexcept { return get(); }

private:
    entity_type entity_ {};

    template<class U, class E>
    [[nodiscard]] static entity_type copy_or_move(std::unique_ptr<U, E>&& ptr) {
        if (!ptr) {
            return nullptr;
        }
        if constexpr (std::is_same_v<E, std::default_delete<U>>) {
            return std::move(ptr);
        }
        return copier_type::copy(*ptr);
    }

    [[nodiscard]] entity_type do_copy() const {
        if (!entity_) {
            return {};
        }
        return copier_type::copy(*entity_);
    }
};

/// @private
template<class T> clonable_ptr(T*) -> clonable_ptr<T>;

/// @private
template<class T, class D> clonable_ptr(std::unique_ptr<T, D>&&) -> clonable_ptr<T>;

/**
 * @brief creates a new instance and wrap it by clonable_ptr.
 * @tparam T the target type
 * @tparam Args the constructor parameter types of target type
 * @param args the constructor arguments
 * @return the created instance
 */
template<class T, class... Args>
inline clonable_ptr<T> make_clonable(Args&&... args) {
    return clonable_ptr<T>(std::make_unique<T>(std::forward<Args>(args)...));
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
