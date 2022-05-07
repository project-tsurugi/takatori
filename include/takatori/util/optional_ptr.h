#pragma once

#include <functional>
#include <ostream>
#include <memory>
#include <optional>
#include <type_traits>
#include <utility>

#include "exception.h"
#include "hash.h"
#include "infect_qualifier.h"

namespace takatori::util {

/// @cond IMPL_DEFS
namespace impl {

template<class T, class = void>
class optional_ptr_base {
public:
    constexpr optional_ptr_base() = default;
    ~optional_ptr_base() = default;
    optional_ptr_base(optional_ptr_base const&) noexcept = delete;
    optional_ptr_base& operator=(optional_ptr_base const&) noexcept = delete;
    constexpr optional_ptr_base(optional_ptr_base&&) noexcept = default;
    constexpr optional_ptr_base& operator=(optional_ptr_base&&) noexcept = default;
};

template<class T>
class optional_ptr_base<T, std::enable_if_t<std::is_const_v<T>>> {
public:
    constexpr optional_ptr_base() = default;
    ~optional_ptr_base() = default;
    constexpr optional_ptr_base(optional_ptr_base const&) noexcept = default;
    constexpr optional_ptr_base& operator=(optional_ptr_base const&) noexcept = default;
    constexpr optional_ptr_base(optional_ptr_base&&) noexcept = default;
    constexpr optional_ptr_base& operator=(optional_ptr_base&&) noexcept = default;

};

} // namespace impl
/// @endcond

/**
 * @brief a pointer like object that can handle empty pointer strictly.
 * @details This reference does not have any ownership of values.
 * @tparam T the value type, must not be a reference type
 */
template<class T>
class optional_ptr : impl::optional_ptr_base<T> {

    static_assert(!std::is_reference_v<T>, "value must not be a reference type");

public:
    /// @brief the value type.
    using value_type = T;
    /// @brief the element type.
    using element_type = T;
    /// @brief the pointer type.
    using pointer = std::add_pointer_t<T>;
    /// @brief the pointer type.
    using const_pointer = std::add_pointer_t<std::add_const_t<T>>;
    /// @brief the L-value reference type.
    using reference = std::add_lvalue_reference_t<T>;
    /// @brief the L-value reference type.
    using const_reference = std::add_lvalue_reference_t<std::add_const_t<T>>;
    /// @brief the R-value reference type.
    using rvalue_reference = std::add_rvalue_reference_t<T>;
    /// @brief the iterator type.
    using iterator = pointer;
    /// @brief the const iterator type.
    using const_iterator = const_pointer;

    /**
     * @brief creates a new empty instance.
     */
    constexpr optional_ptr() noexcept = default;

    /**
     * @brief destroys this object.
     */
    ~optional_ptr() = default;

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    constexpr optional_ptr(optional_ptr const& other) noexcept = default;

    /**
     * @brief assigns the given object.
     * @param other the copy source
     * @return this
     */
    constexpr optional_ptr& operator=(optional_ptr const& other) noexcept = default;

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    constexpr optional_ptr(optional_ptr&& other) noexcept = default;

    /**
     * @brief assigns the given object.
     * @param other the move source
     * @return this
     */
    constexpr optional_ptr& operator=(optional_ptr&& other) noexcept = default;

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    constexpr optional_ptr(optional_ptr& other) noexcept : // NOLINT
        impl::optional_ptr_base<T> {},
        entry_ { other.entry_ }
    {}

    /**
     * @brief assigns the given object.
     * @param other the copy source
     * @return this
     */
    constexpr optional_ptr& operator=(optional_ptr& other) noexcept { // NOLINT
        entry_ = other.entry_;
        return *this;
    }

    /**
     * @brief creates a new empty instance.
     */
    constexpr optional_ptr(std::nullptr_t) noexcept : // NOLINT
        impl::optional_ptr_base<T> {}
    {}

    /**
     * @brief creates a new empty instance.
     */
    constexpr optional_ptr(std::nullopt_t) noexcept : // NOLINT
        impl::optional_ptr_base<T> {}
    {}

    /**
     * @brief creates a new instance.
     * @param entry the entry
     */
    constexpr optional_ptr(reference entry) noexcept : // NOLINT
        impl::optional_ptr_base<T> {},
        entry_ { std::addressof(entry) }
    {}

    /**
     * @brief creates a new instance.
     * @param entry the entry
     */
    explicit constexpr optional_ptr(pointer entry) noexcept :
        impl::optional_ptr_base<T> {},
        entry_ { entry }
    {}

    /**
     * @brief creates a new instance.
     * @tparam U the source value type
     * @param other the source reference
     */
    template<
            class U,
            class = std::enable_if_t<std::is_constructible_v<pointer, typename optional_ptr<U>::pointer>>
    >
    constexpr optional_ptr(optional_ptr<U>& other) noexcept : // NOLINT
        impl::optional_ptr_base<T> {},
        entry_ { other.get() }
    {}

    /**
     * @brief assigns the given object.
     * @param other the copy source
     * @return this
     */
    template<
            class U,
            class = std::enable_if_t<std::is_assignable_v<pointer&, typename optional_ptr<U>::pointer>>
    >
    constexpr optional_ptr& operator=(optional_ptr<U>& other) noexcept { // NOLINT
        entry_ = other.get();
        return *this;
    }

    /**
     * @brief creates a new instance.
     * @tparam U the source value type
     * @param other the source reference
     */
    template<
            class U,
            class = std::enable_if_t<
                    std::is_constructible_v<
                            pointer,
                            typename optional_ptr<U>::const_pointer>>
    >
    constexpr optional_ptr(optional_ptr<U> const& other) noexcept : // NOLINT
        impl::optional_ptr_base<T> {},
        entry_ { other.get() }
    {}

    /**
     * @brief assigns the given object.
     * @param other the copy source
     * @return this
     */
    template<
            class U,
            class = std::enable_if_t<
                    std::is_assignable_v<
                            pointer&,
                            typename optional_ptr<U>::const_pointer>>
    >
    constexpr optional_ptr& operator=(optional_ptr<U> const& other) noexcept {
        entry_ = other.get();
        return *this;
    }

    /**
     * @brief creates a new instance.
     * @tparam U the source value type
     * @param other the source reference
     */
    template<
            class U,
            class = std::enable_if_t<std::is_constructible_v<pointer, typename optional_ptr<U>::pointer>>
    >
    constexpr optional_ptr(optional_ptr<U>&& other) noexcept : // NOLINT
        impl::optional_ptr_base<T> {},
        entry_ { other.get() }
    {}

    /**
     * @brief assigns the given object.
     * @param other the copy source
     * @return this
     */
    template<
            class U,
            class = std::enable_if_t<std::is_assignable_v<pointer&, typename optional_ptr<U>::pointer>>
    >
    constexpr optional_ptr& operator=(optional_ptr<U>&& other) noexcept {
        entry_ = other.get();
        return *this;
    }

    /**
     * @brief returns whether or not this reference is empty.
     * @return true if this reference is empty
     * @return false otherwise
     */
    [[nodiscard]] constexpr bool empty() const noexcept {
        return entry_ == nullptr;
    }

    /**
     * @brief returns whether or not this has any entries.
     * @return true if this has entry
     * @return false otherwise
     */
    [[nodiscard]] constexpr bool has_value() const noexcept {
        return !empty();
    }

    /**
     * @brief returns a pointer to the holding value.
     * @return pointer to the holding value
     * @return nullptr if this reference is empty
     */
    [[nodiscard]] constexpr pointer get() {
        return entry_;
    }

    /// @copydoc get()
    [[nodiscard]] constexpr const_pointer get() const {
        return entry_;
    }

    /**
     * @brief returns the holding value.
     * @return the holding value
     * @throws std::bad_optional_access if this reference is empty
     */
    constexpr reference value() {
        check_();
        return *entry_;
    }

    /// @copydoc value()
    [[nodiscard]] constexpr const_reference value() const {
        check_();
        return *entry_;
    }

    /**
     * @brief rebinds the holding value.
     * @param entry the new value
     */
    void reset(std::nullptr_t entry = nullptr) noexcept {
        entry_ = entry;
    }

    /// @copydoc reset()
    void reset(reference entry) noexcept {
        entry_ = std::addressof(entry);
    }

    /// @copydoc reset()
    void reset(pointer entry) noexcept {
        entry_ = entry;
    }

    /**
     * @brief swap entries between this and the given object.
     * @param other the target object
     */
    void swap(optional_ptr& other) noexcept {
        std::swap(entry_, other.entry_);
    }

    /// @copydoc has_value()
    [[nodiscard]] explicit constexpr operator bool() const noexcept {
        return has_value();
    }

    /// @copydoc value()
    constexpr reference operator*() {
        return value();
    }

    /// @copydoc value()
    constexpr const_reference operator*() const {
        return value();
    }

    /// @copydoc value()
    constexpr pointer operator->() {
        check_();
        return entry_;
    }

    /// @copydoc value()
    constexpr const_pointer operator->() const {
        check_();
        return entry_;
    }

    /**
     * @brief invokes this reference.
     * @tparam Args the parameter types
     * @param args the invocation arguments
     * @return the invocation result
     * @throws std::bad_optional_access if this reference is empty
     * @throws any exception may occur in the invocation
     */
    template<class... Args>
    std::invoke_result_t<value_type, Args...> operator()(Args&&... args) const noexcept(std::is_nothrow_invocable_v<value_type, Args...>) {
        check_();
        return value()(std::forward<Args>(args)...);
    }

    /**
     * @brief returns an iterator that points to the holding value.
     * @return an iterator that points to the holding value
     * @return end of iteration if this reference is empty
     */
    [[nodiscard]] iterator begin() noexcept {
        return entry_ == nullptr ? nullptr : entry_;
    }

    /// @copydoc begin()
    [[nodiscard]] const_iterator begin() const noexcept {
        return cbegin();
    }

    /// @copydoc begin()
    [[nodiscard]] const_iterator cbegin() const noexcept {
        return entry_ == nullptr ? nullptr : entry_;
    }

    /**
     * @brief returns a sentinel of the iterator.
     * @return a sentinel of the iterator
     * @see begin()
     */
    [[nodiscard]] iterator end() noexcept { // NOLINT
        return entry_ == nullptr ? nullptr : entry_ + 1;
    }

    /// @copydoc end()
    [[nodiscard]] const_iterator end() const noexcept { // NOLINT
        return cend();
    }

    /// @copydoc end()
    [[nodiscard]] const_iterator cend() const noexcept { // NOLINT
        return entry_ == nullptr ? nullptr : entry_ + 1;
    }

private:
    pointer entry_ {};

    void check_() const {
        if (entry_ == nullptr) {
            throw_exception(std::bad_optional_access());
        }
    }
};

/// @private
optional_ptr() -> optional_ptr<std::nullptr_t>;

/// @private
optional_ptr(std::nullptr_t) -> optional_ptr<std::nullptr_t>;

/// @private
template<class T> optional_ptr(T&&) -> optional_ptr<std::remove_reference_t<T>>;

/// @private
template<class T> optional_ptr(T*) -> optional_ptr<T>;

/// @private
template<class T> optional_ptr(optional_ptr<T>&) -> optional_ptr<T>;

/// @private
template<class T> optional_ptr(optional_ptr<T> const&) -> optional_ptr<T const>;

/// @private
template<class T> optional_ptr(optional_ptr<T>&&) -> optional_ptr<T>;

/**
 * @brief swaps entries between two references.
 * @tparam T the value type
 * @param a the first reference
 * @param b the second reference
 */
template<class T>
inline void swap(optional_ptr<T>& a, optional_ptr<T>& b) noexcept { a.swap(b); }

/**
 * @brief returns whether or not the two references are equivalent.
 * @tparam T the value type
 * @tparam equal_to the equivalent function object type for the element values
 * @param a the first reference
 * @param b the second reference
 * @return true if the two references are equivalent
 * @return false otherwise
 */
template<class T, class U>
inline constexpr bool operator==(optional_ptr<T> const& a, optional_ptr<U> const& b) noexcept {
    if (!a) return b.empty();
    if (!b) return false;
    return a.get() == b.get() || *a == *b;
}

/// @copydoc operator==(optional_ptr<T> const&, optional_ptr<U> const&)
template<class T, class U>
inline constexpr bool operator==(optional_ptr<T> const& a, U const& b) noexcept {
    return a && (a.get() == std::addressof(b) || *a == b);
}

/// @copydoc operator==(optional_ptr<T> const&, optional_ptr<U> const&)
template<class T, class U>
inline constexpr bool operator==(T const& a, optional_ptr<U> const& b) noexcept { return b == a; }

/**
 * @brief returns whether or not the reference is absent.
 * @tparam T the reference type
 * @param ref the reference
 * @return true if the reference is absent
 * @return false otherwise
 */
template<class T>
inline constexpr bool operator==(optional_ptr<T> const& ref, std::nullptr_t) noexcept {
    return !ref;
}

/**
 * @brief returns whether or not the reference is absent.
 * @tparam T the reference type
 * @param ref the reference
 * @return true if the reference is absent
 * @return false otherwise
 */
template<class T>
inline constexpr bool operator==(std::nullptr_t, optional_ptr<T> const& ref) noexcept {
    return !ref;
}

/**
 * @brief returns whether or not the two references are different.
 * @tparam T the value type
 * @tparam equal_to the equivalent function object type for the element values
 * @param a the first reference
 * @param b the second reference
 * @return true if the two references are different
 * @return false otherwise
 */
template<class T, class U>
inline constexpr bool operator!=(optional_ptr<T> const& a, optional_ptr<U> const& b) noexcept { return !(a == b); }

/// @copydoc operator!=(optional_ptr<T> const&, optional_ptr<U> const&)
template<class T, class U>
inline constexpr bool operator!=(optional_ptr<T> const& a, U const& b) noexcept { return !(a == b); }

/// @copydoc operator!=(optional_ptr<T> const&, optional_ptr<U> const&)
template<class T, class U>
inline constexpr bool operator!=(T const& a, optional_ptr<U> const& b) noexcept { return !(a == b); }

/**
 * @brief returns whether or not the reference is present.
 * @tparam T the reference type
 * @param ref the reference
 * @return true if the reference is present
 * @return false otherwise
 */
template<class T>
inline constexpr bool operator!=(optional_ptr<T> const& ref, std::nullptr_t) noexcept {
    return ref;
}

/**
 * @brief returns whether or not the reference is present.
 * @tparam T the reference type
 * @param ref the reference
 * @return true if the reference is present
 * @return false otherwise
 */
template<class T>
inline constexpr bool operator!=(std::nullptr_t, optional_ptr<T> const& ref) noexcept {
    return ref;
}

/**
 * @brief returns whether or not the first reference is less than the second one.
 * @note empty references are always less than the non-empty references.
 * @tparam T the value type
 * @tparam equal_to the equivalent function object type for the element values
 * @param a the first reference
 * @param b the second reference
 * @return true if a < b
 * @return false otherwise
 */
template<class T, class U>
inline constexpr bool operator<(optional_ptr<T> const& a, optional_ptr<U> const& b) noexcept {
    if (!a) return b.has_value();
    if (!b) return false;
    return *a < *b;
}

/// @copydoc operator<()
template<class T, class U>
inline constexpr bool operator<(optional_ptr<T> const& a, U const& b) noexcept { return a ? *a < b : true; }

/// @copydoc operator<()
template<class T, class U>
inline constexpr bool operator<(T const& a, optional_ptr<U> const& b) noexcept { return b ? a < *b : false; }

/**
 * @brief returns whether or not the first reference is greater than the second one.
 * @details This compute the order by using (b < a).
 * @tparam T the value type
 * @tparam equal_to the equivalent function object type for the element values
 * @param a the first reference
 * @param b the second reference
 * @return true if a > b
 * @return false otherwise
 */
template<class T, class U>
inline constexpr bool operator>(optional_ptr<T> const& a, optional_ptr<U> const& b) noexcept { return b < a; }

/// @copydoc operator>()
template<class T, class U>
inline constexpr bool operator>(optional_ptr<T> const& a, U const& b) noexcept { return b < a; }

/// @copydoc operator>()
template<class T, class U>
inline constexpr bool operator>(T const& a, optional_ptr<U> const& b) noexcept { return b < a; }

/**
 * @brief returns whether or not the first reference is less than or equal to the second one.
 * @details This compute the order by using !(b < a).
 * @note empty references are always less than the non-empty references.
 * @tparam T the value type
 * @tparam equal_to the equivalent function object type for the element values
 * @param a the first reference
 * @param b the second reference
 * @return true if a < b
 * @return false otherwise
 */
template<class T, class U>
inline constexpr bool operator<=(optional_ptr<T> const& a, optional_ptr<U> const& b) noexcept { return !(b < a); }

/// @copydoc operator<=()
template<class T, class U>
inline constexpr bool operator<=(optional_ptr<T> const& a, U const& b) noexcept { return !(b < a); }

/// @copydoc operator<=()
template<class T, class U>
inline constexpr bool operator<=(T const& a, optional_ptr<U> const& b) noexcept { return !(b < a); }

/**
 * @brief returns whether or not the first reference is greater than or equal to the second one.
 * @details This compute the order by using !(a < b).
 * @tparam T the value type
 * @tparam equal_to the equivalent function object type for the element values
 * @param a the first reference
 * @param b the second reference
 * @return true if a > b
 * @return false otherwise
 */
template<class T, class U>
inline constexpr bool operator>=(optional_ptr<T> const& a, optional_ptr<U> const& b) noexcept { return !(a < b); }

/// @copydoc operator>=()
template<class T, class U>
inline constexpr bool operator>=(optional_ptr<T> const& a, U const& b) noexcept { return !(a < b); }

/// @copydoc operator>=()
template<class T, class U>
inline constexpr bool operator>=(T const& a, optional_ptr<U> const& b) noexcept { return !(a < b); }

/**
 * @brief converts the reference by dynamic pointer cast.
 * @tparam T the destination value type
 * @tparam U the source value type
 * @param value the target reference
 * @return the converted reference
 * @return empty if the input reference is empty, or failed to convert the reference
 */
template<class T, class U>
inline optional_ptr<util::infect_const_t<U, T>> dynamic_pointer_cast(optional_ptr<U>& value) noexcept {
    using destination_type = optional_ptr<util::infect_const_t<U, T>>;
    return destination_type(dynamic_cast<typename destination_type::pointer>(value.get()));
}

/// @copydoc dynamic_pointer_cast()
template<class T, class U>
inline optional_ptr<T const> dynamic_pointer_cast(optional_ptr<U> const& value) noexcept {
    using destination_type = optional_ptr<T const>;
    return destination_type(dynamic_cast<typename destination_type::pointer>(value.get()));
}

/// @copydoc dynamic_pointer_cast()
template<class T, class U>
inline optional_ptr<util::infect_const_t<U, T>> dynamic_pointer_cast(optional_ptr<U>&& value) noexcept {
    using destination_type = optional_ptr<util::infect_const_t<U, T>>;
    return destination_type(dynamic_cast<typename destination_type::pointer>(value.get()));
}

/// @copydoc dynamic_pointer_cast()
template<class T, class U>
inline optional_ptr<util::infect_const_t<U, T>> dynamic_pointer_cast(U& value) noexcept {
    optional_ptr<std::remove_reference_t<U>> ptr { value };
    return dynamic_pointer_cast<T>(ptr);
}

/**
 * @brief converts the reference by dynamic reference cast.
 * @tparam T the destination value type
 * @tparam U the source value type
 * @param value the target reference
 * @return the converted reference
 * @return empty if the input reference is empty
 * @throws std::bad_cast if conversion was failed
 */
template<class T, class U>
inline optional_ptr<util::infect_const_t<U, T>> dynamic_reference_cast(optional_ptr<U>& value) {
    if (!value) return {};
    using destination_type = optional_ptr<util::infect_const_t<U, T>>;
    return optional_ptr(dynamic_cast<typename destination_type::reference>(*value));
}

/// @copydoc dynamic_reference_cast()
template<class T, class U>
inline optional_ptr<T const> dynamic_reference_cast(optional_ptr<U> const& value) {
    if (!value) return {};
    using destination_type = optional_ptr<T const>;
    return optional_ptr(dynamic_cast<typename destination_type::reference>(*value));
}

/// @copydoc dynamic_reference_cast()
template<class T, class U>
inline optional_ptr<util::infect_const_t<U, T>> dynamic_reference_cast(optional_ptr<U>&& value) {
    if (!value) return {};
    using destination_type = optional_ptr<util::infect_const_t<U, T>>;
    return optional_ptr(dynamic_cast<typename destination_type::reference>(*value));
}

/**
 * @brief appends string representation of the given value.
 * @tparam T the value type
 * @param out the output stream
 * @param value the target value
 * @return the output stream
 */
template<class T>
inline std::ostream& operator<<(std::ostream& out, optional_ptr<T> const& value) {
    if (!value) {
        return out << "(empty)";
    }
    return out << *value;
}

} // namespace takatori::util

/**
 * @brief provides hash function of optional_reference.
 * @tparam value_hash the hash function object for element values
 */
template<class T>
struct std::hash<takatori::util::optional_ptr<T>> {
    /**
     * @brief returns the hash code of the reference.
     * @param value the target reference
     * @return the corresponded hash code
     */
    std::size_t operator()(takatori::util::optional_ptr<T> const& value) const noexcept {
        return value.get() == nullptr ? takatori::util::hash()
                                      : takatori::util::hash(1, value.value());
    }
};
