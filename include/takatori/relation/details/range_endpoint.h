#pragma once

#include <initializer_list>
#include <vector>

#include "../endpoint_kind.h"

#include <takatori/tree/tree_fragment_vector.h>
#include <takatori/tree/tree_fragment_vector_forward.h>

#include <takatori/descriptor/relation.h>

#include <takatori/util/meta_type.h>
#include <takatori/util/object_creator.h>
#include <takatori/util/rvalue_reference_wrapper.h>

namespace takatori::relation::details {

/**
 * @brief represents end-point of range.
 * @tparam Parent the parent type
 * @tparam Key the key type
 */
template<class Parent, class Key>
class range_endpoint {
public:
    /// @brief the fragment owner type.
    using parent_type = Parent;

    /// @brief the key element type.
    using key = Key;

    /// @brief the end-point kind type.
    using kind_type = endpoint_kind;

    /**
     * @brief creates a new instance which represents unbound end-point.
     */
    range_endpoint() = default;

    /**
     * @brief creates a new instance.
     * @param keys pieces of key to point to target row
     * @param kind the end-point kind
     */
    range_endpoint(
            std::vector<key, util::object_allocator<key>> keys,
            kind_type kind) noexcept;

    /**
     * @brief creates a new instance.
     * @param key a piece of key to point to target row
     * @param kind the end-point kind
     * @attention this may take copies of arguments
     */
    range_endpoint(
            key&& key,
            kind_type kind);

    /**
     * @brief creates a new instance.
     * @param keys pieces of key to point to target row
     * @param kind the end-point kind
     * @attention this may take copies of arguments
     */
    range_endpoint(
            std::initializer_list<util::rvalue_reference_wrapper<key>> keys,
            kind_type kind);

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    explicit range_endpoint(range_endpoint const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    explicit range_endpoint(range_endpoint&& other, util::object_creator creator);

    /**
     * @brief returns the owner of this fragment.
     * @return the owner
     * @return nullptr if it is absent
     */
    parent_type* parent_element() noexcept;

    /// @copydoc parent_element()
    parent_type const* parent_element() const noexcept;

    /**
     * @brief sets the owner of this fragment.
     * @param parent the owner
     */
    void parent_element(parent_type* parent) noexcept;

    /**
     * @brief returns pieces of key to point to the target row.
     * @return the target row key
     */
    tree::tree_fragment_vector<key>& keys() noexcept;

    /// @brief keys()
    tree::tree_fragment_vector<key> const& keys() const noexcept;

    /**
     * @brief returns the end-point kind of this.
     * @return the end-point kind
     */
    kind_type kind() const noexcept;

    /**
     * @brief sets the end-point kind.
     * @param kind the end-point kind
     * @return this
     */
    range_endpoint& kind(kind_type kind) noexcept;

private:
    tree::tree_fragment_vector<key> keys_ {};
    kind_type kind_ { endpoint_kind::unbound };
    parent_type* parent_ {};
};

template<class Parent, class Key>
inline range_endpoint<Parent, Key>::range_endpoint(
        std::vector<key, util::object_allocator<key>> keys,
        kind_type kind) noexcept
    : keys_(nullptr, std::move(keys))
        , kind_(kind)
{}

template<class Parent, class Key>
inline range_endpoint<Parent, Key>::range_endpoint(key&& key, kind_type kind)
    : range_endpoint(
            { util::rvalue_reference_wrapper { std::move(key) } },
            kind)
{}

template<class Parent, class Key>
inline range_endpoint<Parent, Key>::range_endpoint(
        std::initializer_list<util::rvalue_reference_wrapper<key>> keys,
        kind_type kind)
    : range_endpoint(
            { keys.begin(), keys.end() },
            kind)
{}

template<class Parent, class Key>
inline
range_endpoint<Parent, Key>::range_endpoint(range_endpoint const& other, util::object_creator creator)
    : range_endpoint(
            tree::forward(creator, other.keys_),
            other.kind_)
{}

template<class Parent, class Key>
inline
range_endpoint<Parent, Key>::range_endpoint(range_endpoint&& other, util::object_creator creator)
    : range_endpoint(
            tree::forward(creator, std::move(other.keys_)),
            other.kind_)
{}

template<class Parent, class Key>
inline typename range_endpoint<Parent, Key>::parent_type*
range_endpoint<Parent, Key>::parent_element() noexcept {
    return parent_;
}

template<class Parent, class Key>
inline typename range_endpoint<Parent, Key>::parent_type const*
range_endpoint<Parent, Key>::parent_element() const noexcept {
    return parent_;
}

template<class Parent, class Key>
inline void
range_endpoint<Parent, Key>::parent_element(parent_type* parent) noexcept {
    parent_ = parent;
    tree::bless_element(parent_, keys_);
}

template<class Parent, class Key>
inline tree::tree_fragment_vector<typename range_endpoint<Parent, Key>::key>&
range_endpoint<Parent, Key>::keys() noexcept {
    return keys_;
}

template<class Parent, class Key>
inline tree::tree_fragment_vector<typename range_endpoint<Parent, Key>::key> const&
range_endpoint<Parent, Key>::keys() const noexcept {
    return keys_;
}

template<class Parent, class Key>
inline typename range_endpoint<Parent, Key>::kind_type
range_endpoint<Parent, Key>::kind() const noexcept {
    return kind_;
}

template<class Parent, class Key>
inline range_endpoint<Parent, Key>&
range_endpoint<Parent, Key>::kind(kind_type kind) noexcept {
    kind_ = kind;
    return *this;
}

/**
 * @brief returns whether or not the two elements are equivalent.
 * @tparam Parent the parent type
 * @tparam Key the key type
 * @param a the first element
 * @param b the second element
 * @return true if a == b
 * @return false otherwise
 */
template<class Parent, class Key>
inline bool
operator==(range_endpoint<Parent, Key> const& a, range_endpoint<Parent, Key> const& b) noexcept {
    return a.keys() == b.keys()
           && a.kind() == b.kind();
}

/**
 * @brief returns whether or not the two elements are different.
 * @tparam Parent the parent type
 * @tparam Key the key type
 * @param a the first element
 * @param b the second element
 * @return true if a != b
 * @return false otherwise
 */
template<class Parent, class Key>
inline bool
operator!=(range_endpoint<Parent, Key> const& a, range_endpoint<Parent, Key> const& b) noexcept {
    return !(a == b);
}

/**
 * @brief appends string representation of the given value.
 * @tparam Parent the parent type
 * @tparam Key the key type
 * @param out the target output
 * @param value the target value
 * @return the output
 */
template<class Parent, class Key>
inline std::ostream&
operator<<(std::ostream& out, range_endpoint<Parent, Key> const& value) {
    return out << "endpoint" << "("
               << "keys=" << value.keys() << ", "
               << "kind=" << value.kind() << ")";
}

} // namespace takatori::relation::details
