#pragma once

#include <functional>
#include <memory>
#include <type_traits>
#include <variant>

#include "fail.h"

namespace takatori::util {

/**
 * @brief a smart pointer that may or may not have the ownership.
 * @tparam T the element type
 */
template<class T>
class maybe_shared_ptr {

    static_assert(!std::is_array_v<T>);

public:
    /// @brief the element type.
    using element_type = T;

    /// @brief the naked pointer type.
    using pointer = std::add_pointer_t<element_type>;

    /// @brief the shared pointer type.
    using shared_ptr = std::shared_ptr<element_type>;

    /// @brief the reference type.
    using reference = std::add_lvalue_reference_t<element_type>;

    /**
     * @brief creates a new empty instance.
     */
    constexpr maybe_shared_ptr(std::nullptr_t = nullptr) noexcept // NOLINT
        : entity_(std::in_place_index<naked_index>)
    {}

    /**
     * @brief creates a new instance without holding ownership.
     * @param element the pointer
     */
    explicit constexpr maybe_shared_ptr(pointer element) noexcept // NOLINT
        : entity_(std::in_place_index<naked_index>, element)
    {}

    /**
     * @brief creates a new instance with holding ownership.
     * @param element the shared pointer
     */
    maybe_shared_ptr(shared_ptr element) noexcept // NOLINT
        : entity_(std::in_place_index<shared_index>, std::move(element))
    {}

    /**
     * @brief creates a new instance from the other instance.
     * @tparam U the source type
     * @param element the copy type
     */
    template<
            class U,
            class = std::enable_if_t<
                    std::is_convertible_v<U*, T*>>>
    maybe_shared_ptr(maybe_shared_ptr<U> const& element) noexcept // NOLINT
        : entity_(build(element))
    {}

    /**
     * @brief creates a new instance from the other instance.
     * @tparam U the source type
     * @param element the move type
     */
    template<
            class U,
            class = std::enable_if_t<
                    std::is_convertible_v<U*, T*>>>
    maybe_shared_ptr(maybe_shared_ptr<U>&& element) noexcept // NOLINT
        : entity_(build(std::move(element)))
    {}

    /**
     * @brief releases the holding object.
     * @return this
     */
    constexpr maybe_shared_ptr& operator=(std::nullptr_t) noexcept {
        entity_ = decltype(entity_) { std::in_place_index<naked_index> };
        return *this;
    }

    /**
     * @brief assigns the given object without holding ownership.
     * @param element the pointer
     * @return this
     */
    constexpr maybe_shared_ptr& operator=(pointer element) noexcept {
        entity_ = decltype(entity_) { std::in_place_index<naked_index>, element };
        return *this;
    }

    /**
     * @brief assigns the given object with holding ownership.
     * @param element the shared pointer
     * @return this
     */
    maybe_shared_ptr& operator=(shared_ptr element) noexcept {
        entity_ = decltype(entity_) { std::in_place_index<shared_index>, std::move(element) };
        return *this;
    }

    /**
     * @brief assigns given object into this.
     * @tparam U the source type
     * @param element the copy type
     * @return this
     */
    template<
            class U,
            class = std::enable_if_t<
                    std::is_convertible_v<U*, T*>>>
    maybe_shared_ptr& operator=(maybe_shared_ptr<U> const& element) noexcept {
        entity_ = build(element);
        return *this;
    }

    /**
     * @brief assigns given object into this.
     * @tparam U the source type
     * @param element the move type
     * @return this
     */
    template<
            class U,
            class = std::enable_if_t<
                    std::is_convertible_v<U*, T*>>>
    maybe_shared_ptr& operator=(maybe_shared_ptr<U>&& element) noexcept {
        entity_ = build(std::move(element));
        return *this;
    }

    /**
     * @brief releases the holding object.
     */
    constexpr void reset() noexcept {
        *this = nullptr;
    }

    /**
     * @brief returns a naked pointer of the holding object.
     * @return the naked pointer
     */
    [[nodiscard]] pointer get() const noexcept {
        switch (entity_.index()) {
            case naked_index: return *std::get_if<pointer>(std::addressof(entity_));
            case shared_index: return std::get_if<shared_ptr>(std::addressof(entity_))->get();
        }
        fail();
    }

    /**
     * @brief returns a copy of ownership only if this holds ownership.
     * @return object as shared pointer if this has ownership
     * @return empty if this does not have ownership
     */
    [[nodiscard]] shared_ptr ownership() const noexcept {
        if (auto* r = std::get_if<shared_ptr>(std::addressof(entity_)); r != nullptr) {
            return *r;
        }
        return {};
    }

    /**
     * @brief returns whether or not this holds a non-null object.
     * @return true if this has non-null object
     * @return false otherwise
     */
    [[nodiscard]] explicit operator bool() const noexcept {
        return get() != nullptr;
    }

    /**
     * @brief returns a reference of the holding object.
     * @return the reference
     */
    [[nodiscard]] reference operator*() const noexcept {
        return *get();
    }

    /// @copydoc get()
    [[nodiscard]] pointer operator->() const noexcept {
        return get();
    }

private:
    static constexpr std::size_t naked_index = 0;
    static constexpr std::size_t shared_index = 1;
    std::variant<pointer, shared_ptr> entity_;

    template<class U>
    [[nodiscard]] static decltype(entity_) build(maybe_shared_ptr<U> const& v) noexcept {
        switch (v.entity_.index()) {
            case naked_index: return { *std::get_if<naked_index>(std::addressof(v.entity_)) };
            case shared_index: return { *std::get_if<shared_index>(std::addressof(v.entity_)) };
        }
        fail();
    }

    template<class U>
    [[nodiscard]] static decltype(entity_) build(maybe_shared_ptr<U>&& v) noexcept {
        switch (v.entity_.index()) {
            case naked_index: return { *std::get_if<naked_index>(std::addressof(v.entity_)) };
            case shared_index: return { std::move(*std::get_if<shared_index>(std::addressof(v.entity_))) };
        }
        fail();
    }

    template<class U>
    friend class maybe_shared_ptr;
    friend ::std::hash<maybe_shared_ptr<T>>;
};

/// @private
template<class T>
maybe_shared_ptr(T* element) -> maybe_shared_ptr<T>;

/// @private
template<class T>
maybe_shared_ptr(std::shared_ptr<T> element) -> maybe_shared_ptr<T>;

/**
 * @brief compares the each pointer of the maybe_shared_ptr.
 * @tparam T the first element type
 * @tparam U the second element type
 * @param a the first object
 * @param b the second object
 * @return true if the both have the same pointer
 * @return false otherwise
 */
template<class T, class U>
inline bool operator==(maybe_shared_ptr<T> const& a, maybe_shared_ptr<U> const& b) noexcept {
    return a.get() == b.get();
}

/**
 * @brief compares the each pointer of the maybe_shared_ptr.
 * @tparam T the first element type
 * @tparam U the second element type
 * @param a the first object
 * @param b the second object
 * @return true if the both have different pointer
 * @return false otherwise
 */
template<class T, class U>
inline bool operator!=(maybe_shared_ptr<T> const& a, maybe_shared_ptr<U> const& b) noexcept {
    return !(a == b);
}

/**
 * @brief compares the each pointer of the maybe_shared_ptr.
 * @tparam T the first element type
 * @tparam U the second element type
 * @param a the first object
 * @param b the second object
 * @return true if a < b
 * @return false otherwise
 */
template<class T, class U>
inline bool operator<(maybe_shared_ptr<T> const& a, maybe_shared_ptr<U> const& b) noexcept {
    return a.get() < b.get();
}

/**
 * @brief compares the each pointer of the maybe_shared_ptr.
 * @tparam T the first element type
 * @tparam U the second element type
 * @param a the first object
 * @param b the second object
 * @return true if a > b
 * @return false otherwise
 */
template<class T, class U>
inline bool operator>(maybe_shared_ptr<T> const& a, maybe_shared_ptr<U> const& b) noexcept {
    return a.get() > b.get();
}

/**
 * @brief compares the each pointer of the maybe_shared_ptr.
 * @tparam T the first element type
 * @tparam U the second element type
 * @param a the first object
 * @param b the second object
 * @return true if a <= b
 * @return false otherwise
 */
template<class T, class U>
inline bool operator<=(maybe_shared_ptr<T> const& a, maybe_shared_ptr<U> const& b) noexcept {
    return a.get() <= b.get();
}

/**
 * @brief compares the each pointer of the maybe_shared_ptr.
 * @tparam T the first element type
 * @tparam U the second element type
 * @param a the first object
 * @param b the second object
 * @return true if a < b
 * @return false otherwise
 */
template<class T, class U>
inline bool operator>=(maybe_shared_ptr<T> const& a, maybe_shared_ptr<U> const& b) noexcept {
    return a.get() >= b.get();
}

/**
 * @brief appends string representation of the given value.
 * @tparam T the element type
 * @param out the output stream
 * @param value the target value
 * @return the output stream
 */
template<class T>
inline std::ostream& operator<<(std::ostream& out, maybe_shared_ptr<T> const& value) {
    return out << value.get();
}

} // namespace takatori::util

/**
 * @brief provides hash code of takatori::util::maybe_shared_ptr.
 * @tparam T the element type
 */
template<class T>
struct std::hash<::takatori::util::maybe_shared_ptr<T>> {
    /**
     * @brief returns hash code of the given object.
     * @param object the target object
     * @return the computed hash code
     */
    std::size_t operator()(::takatori::util::maybe_shared_ptr<T> const& object) noexcept {
        return std::hash<T*>{}(object.get());
    }
};
