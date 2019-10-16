#pragma once

#include <iostream>
#include <memory>
#include <optional>
#include <type_traits>
#include <utility>

/// @brief utilities
namespace util {

/**
 * @brief a value view which may be empty.
 * This view does not have any ownership of values.
 * @tparam T the value type, must not be a reference type
 */
template<class T>
class optional_view {

    static_assert(!std::is_reference_v<T>, "value must not be a reference type");

public:
    /// @brief the value type.
    using value_type = T;

    /// @brief the pointer type.
    using pointer = std::add_pointer_t<T>;

    /// @brief the L-value reference type.
    using reference = std::add_lvalue_reference_t<T>;

    /// @brief the R-value reference type.
    using rvalue_reference = std::add_rvalue_reference_t<T>;

    /// @brief the iterator type.
    using iterator = pointer;

    /**
     * @brief creates a new empty instance.
     */
    optional_view() = default;

    /**
     * @brief creates a new empty instance.
     */
    constexpr optional_view(std::nullptr_t) noexcept {} // NOLINT

    /**
     * @brief creates a new instance.
     * @param entry the entry
     */
    constexpr optional_view(reference entry) noexcept : entry_(&entry) {} // NOLINT

    /**
     * @brief creates a new instance.
     * @param entry the entry
     */
    explicit constexpr optional_view(pointer entry) noexcept : entry_(entry) {}

    /**
     * @brief creates a new instance.
     * @tparam U the source value type
     * @param other the source view
     */
    template<
            class U,
            class = std::enable_if_t<std::is_constructible_v<pointer, typename optional_view<U>::pointer>>
    >
    constexpr optional_view(optional_view<U> other) noexcept : entry_(other.get()) {} // NOLINT

    /**
     * @brief returns whether or not this view is empty.
     * @return true if this view is empty
     * @return false otherwise
     */
    constexpr bool empty() const noexcept { return entry_ == nullptr; }

    /**
     * @brief returns whether or not this has any entries.
     * @return true if this has entry
     * @return false otherwise
     */
    constexpr bool has_value() const noexcept { return !empty(); }

    /**
     * @brief returns a pointer to the holding value.
     * @return pointer to the holding value
     * @return nullptr if this view is empty
     */
    constexpr pointer get() const { return entry_; }

    /**
     * @brief returns the holding value.
     * @return the holding value
     * @throws std::bad_optional_access if this view is empty
     */
    constexpr reference value() const& { check_(); return *entry_; }
    /// @copydoc value()
    constexpr rvalue_reference value() && { check_(); return *entry_; }

    /**
     * @brief rebinds the holding value.
     * @param entry the new value
     */
    void reset(std::nullptr_t entry = nullptr) noexcept { entry_ = entry; }

    /// @copydoc reset()
    void reset(reference entry) noexcept { entry_ = &entry; }

    /// @copydoc reset()
    void reset(pointer entry) noexcept { entry_ = entry; }

    /**
     * @brief swap entries between this and the given object.
     * @param other the target object
     */
    void swap(optional_view& other) noexcept { std::swap(entry_, other.entry_); }

    /// @copydoc has_value()
    explicit constexpr operator bool() const noexcept { return has_value(); }

    /// @copydoc value()
    constexpr reference operator*() const& { return value(); }
    /// @copydoc value()
    constexpr rvalue_reference operator*() && { return std::move(value()); }

    /// @copydoc value()
    constexpr pointer operator->() const { check_(); return entry_; }

    /**
     * @brief invokes this view.
     * @tparam Args the parameter types
     * @param args the invocation arguments
     * @return the invocation result
     * @throws std::bad_optional_access if this view is empty
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
     * @return end of iteration if this view is empty
     */
    iterator begin() const noexcept { return entry_ == nullptr ? nullptr : entry_; }

    /**
     * @brief returns a sentinel of the iterator.
     * @return a sentinel of the iterator
     * @see begin()
     */
    iterator end() const noexcept { return entry_ == nullptr ? nullptr : entry_ + 1; }  // NOLINT

    /**
     * @brief provides hash function of optional_view.
     * @tparam value_hash the hash function object for element values
     */
    template<class value_hash = std::hash<T>>
    struct hash {
        /**
         * @brief returns the hash code of the view.
         * @param value the target view
         * @return the corresponded hash code
         */
        std::size_t operator()(optional_view value) const noexcept;
    };

private:
    pointer entry_ {};

    void check_() const {
        if (entry_ == nullptr) {
            throw std::bad_optional_access();
        }
    }
};

static_assert(!std::is_constructible_v<optional_view<int>, int&&>);
static_assert(std::is_trivially_copyable_v<optional_view<int>>);
static_assert(std::is_trivially_destructible_v<optional_view<int>>);
static_assert(sizeof(optional_view<int>) == sizeof(int*));

/// @private
optional_view() -> optional_view<std::nullptr_t>;

/// @private
optional_view(std::nullptr_t) -> optional_view<std::nullptr_t>;

/// @private
template<class T> optional_view(T&) -> optional_view<T>;

/// @private
template<class T> optional_view(T*) -> optional_view<T>;

/**
 * @brief swaps entries between two views.
 * @tparam T the value type
 * @param a the first view
 * @param b the second view
 */
template<class T>
inline void swap(optional_view<T>& a, optional_view<T>& b) noexcept { a.swap(b); }

/**
 * @brief returns whether or not the two views are equivalent.
 * @tparam T the value type
 * @tparam equal_to the equivalent function object type for the element values
 * @param a the first view
 * @param b the second view
 * @return true if the two views are equivalent
 * @return false otherwise
 */
template<class T, class equal_to = std::equal_to<T>>
inline bool operator==(optional_view<T> a, optional_view<T> b) noexcept {
    if (!a) return b.empty();
    if (!b) return false;
    return equal_to{}(*a, *b);
}

/// @copydoc operator==()
template<class T, class equal_to = std::equal_to<T>>
inline bool operator==(optional_view<T> a, T const& b) noexcept {
    return a && equal_to{}(*a, b);
}

/// @copydoc operator==()
template<class T, class equal_to = std::equal_to<T>>
inline bool operator==(T const& a, optional_view<T> b) noexcept {
    return operator==<T, equal_to>(b, a);
}

/**
 * @brief returns whether or not the two views are different.
 * @tparam T the value type
 * @tparam equal_to the equivalent function object type for the element values
 * @param a the first view
 * @param b the second view
 * @return true if the two views are different
 * @return false otherwise
 */
template<class T, class equal_to = std::equal_to<T>>
inline bool operator!=(optional_view<T> a, optional_view<T> b) noexcept {
    return !operator==<T, equal_to>(a, b);
}

/// @copydoc operator!=()
template<class T, class equal_to = std::equal_to<T>>
inline bool operator!=(optional_view<T> a, T const& b) noexcept {
    return !operator==<T, equal_to>(a, b);
}

/// @copydoc operator!=()
template<class T, class equal_to = std::equal_to<T>>
inline bool operator!=(T const& a, optional_view<T> b) noexcept {
    return !operator==<T, equal_to>(a, b);
}

/**
 * @brief returns whether or not the first view is less than the second one.
 * Note that, empty views are always less than the non-empty views.
 * @tparam T the value type
 * @tparam equal_to the equivalent function object type for the element values
 * @param a the first view
 * @param b the second view
 * @return true if a < b
 * @return false otherwise
 */
template<class T, class less = std::less<T>>
inline bool operator<(optional_view<T> a, optional_view<T> b) noexcept {
    if (!a) return b.has_value();
    if (!b) return false;
    return less{}(*a, *b);
}

/// @copydoc operator<()
template<class T, class less = std::less<T>>
inline bool operator<(optional_view<T> a, T const& b) noexcept { return a ? less{}(*a, b) : true; }

/// @copydoc operator<()
template<class T, class less = std::less<T>>
inline bool operator<(T const& a, optional_view<T> b) noexcept { return b ? less{}(a, *b) : false; }

/**
 * @brief returns whether or not the first view is greater than the second one.
 * This compute the order by using (b < a).
 * @tparam T the value type
 * @tparam equal_to the equivalent function object type for the element values
 * @param a the first view
 * @param b the second view
 * @return true if a > b
 * @return false otherwise
 */
template<class T, class less = std::less<T>>
inline bool operator>(optional_view<T> a, optional_view<T> b) noexcept {
    return operator< <T, less>(b, a);
}

/// @copydoc operator>()
template<class T, class less = std::less<T>>
inline bool operator>(optional_view<T> a, T const& b) noexcept {
    return operator< <T, less>(b, a);
}

/// @copydoc operator>()
template<class T, class less = std::less<T>>
inline bool operator>(T const& a, optional_view<T> b) noexcept {
    return operator< <T, less>(b, a);
}

/**
 * @brief returns whether or not the first view is less than or equal to the second one.
 * This compute the order by using !(a > b).
 * Note that, empty views are always less than the non-empty views.
 * @tparam T the value type
 * @tparam equal_to the equivalent function object type for the element values
 * @param a the first view
 * @param b the second view
 * @return true if a < b
 * @return false otherwise
 */
template<class T, class less = std::less<T>>
inline bool operator<=(optional_view<T> a, optional_view<T> b) noexcept {
    return !operator> <T, less>(a, b);
}

/// @copydoc operator<=()
template<class T, class less = std::less<T>>
inline bool operator<=(optional_view<T> a, T const& b) noexcept {
    return !operator> <T, less>(a, b);
}

/// @copydoc operator<=()
template<class T, class less = std::less<T>>
inline bool operator<=(T const& a, optional_view<T> b) noexcept {
    return !operator> <T, less>(a, b);
}

/**
 * @brief returns whether or not the first view is greater than or equal to the second one.
 * This compute the order by using !(a < b).
 * @tparam T the value type
 * @tparam equal_to the equivalent function object type for the element values
 * @param a the first view
 * @param b the second view
 * @return true if a > b
 * @return false otherwise
 */
template<class T, class less = std::less<T>>
inline bool operator>=(optional_view<T> a, optional_view<T> b) noexcept {
    return !operator< <T, less>(a, b);
}

/// @copydoc operator>=()
template<class T, class less = std::less<T>>
inline bool operator>=(optional_view<T> a, T const& b) noexcept {
    return !operator< <T, less>(a, b);
}

/// @copydoc operator>=()
template<class T, class less = std::less<T>>
inline bool operator>=(T const& a, optional_view<T> b) noexcept {
    return !operator< <T, less>(a, b);
}

// documented
template<class T>
template<class element_hash>
inline std::size_t optional_view<T>::hash<element_hash>::operator()(optional_view<T> value) const noexcept {
    return value.entry_ == nullptr ? 0 : 1 + 31 * element_hash{}(*value.entry_);
}

/**
 * @brief converts the view by dynamic pointer cast.
 * @tparam T the destination value type
 * @tparam U the source value type
 * @param view the target view
 * @return the converted view
 * @return empty if the input view is empty, or failed to convert the view
 */
template<class T, class U>
inline optional_view<T> dynamic_pointer_cast(optional_view<U> view) noexcept {
    if (auto* pointer = dynamic_cast<typename optional_view<T>::pointer>(view.get()); pointer != nullptr) {
        return *pointer;
    }
    return {};
}

/**
 * @brief converts the view by dynamic reference cast.
 * @tparam T the destination value type
 * @tparam U the source value type
 * @param view the target view
 * @return the converted view
 * @return empty if the input view is empty
 * @throws std::bad_cast if conversion was failed
 */
template<class T, class U>
inline optional_view<T> dynamic_reference_cast(optional_view<U> view) {
    if (!view) {
        return {};
    }
    return dynamic_cast<typename optional_view<T>::reference>(*view);
}

/**
 * @brief appends string representation of the given value.
 * @tparam T the value type
 * @param out the output stream
 * @param value the target value
 * @return the output stream
 */
template<class T>
inline std::ostream& operator<<(std::ostream& out, optional_view<T> value) {
    if (!value) {
        return out << "(empty)";
    }
    return out << *value;
}

} // namespace util
