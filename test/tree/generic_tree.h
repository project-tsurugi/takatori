#pragma once

#include <memory>

#include <takatori/tree/tree_element.h>

#include <takatori/util/optional_ptr.h>

namespace takatori::tree {

template<class T>
class node : public tree_element {
public:
    using element_type = T;

    ~node() override = default;
    node(node const& other) = delete;
    node& operator=(node const& other) = delete;
    node(node&& other) noexcept = delete;
    node& operator=(node&& other) noexcept = delete;

    void parent_element(node<T>* parent_element) noexcept { parent_ = parent_element; }
    node<T>* parent_element() noexcept override { return parent_; }
    node<T> const* parent_element() const noexcept override { return parent_; }

    template<template<class> class Child>
    Child<T>& cast() { return dynamic_cast<Child<T>&>(*this); }

    template<template<class> class Child>
    Child<T> const& cast() const { return dynamic_cast<Child<T> const&>(*this); }

protected:
    node() = default;

private:
    node<T>* parent_;
};

template<class T>
class branch : public node<T> {
public:
    branch() = default;
    branch(std::unique_ptr<node<T>> left, std::unique_ptr<node<T>> right) noexcept
        : left_(std::move(left))
        , right_(std::move(right))
    {}

    util::optional_ptr<node<T>> left() noexcept { return *left_; }
    util::optional_ptr<node<T> const> left() const noexcept { return *left_; }
    branch<T>& left(std::unique_ptr<node<T>> left) noexcept { left_ = std::move(left); return *this; }

    util::optional_ptr<node<T>> right() noexcept { return *right_; }
    util::optional_ptr<node<T> const> right() const noexcept { return *right_; }
    branch<T>& right(std::unique_ptr<node<T>> right) noexcept { right_ = std::move(right); return *this; }

private:
    std::unique_ptr<node<T>> left_ {};
    std::unique_ptr<node<T>> right_ {};
};

template<class T>
class leaf : public node<T> {
public:
    explicit leaf(T&& value) noexcept : value_(std::move(value)) {}

    T& value() noexcept { return value_; }
    T const& value() const noexcept { return value_; }

private:
    T value_;
};

template<class T>
leaf(T&&) -> leaf<T>;

template<class T>
bool operator<(node<T> const& a, node<T> const& b);

template<class T>
inline bool operator==(node<T> const& a, node<T> const& b) {
    return !(a < b) && !(b < a);
}

template<class T>
inline bool operator!=(node<T> const& a, node<T> const& b) {
    return (a < b) || (b < a);
}

template<class T>
inline bool operator<(node<T> const& a, node<T> const& b) {
    auto a_branch = dynamic_cast<branch<T> const*>(&a);
    auto b_branch = dynamic_cast<branch<T> const*>(&b);

    if (a_branch && b_branch) {
        if (a_branch->left() < b_branch->left()) {
            return true;
        }
        if (b_branch->left() < b_branch->left()) {
            return false;
        }
        return a_branch->right() < b_branch->right();
    }

    auto a_leaf = dynamic_cast<leaf<T> const*>(&a);
    auto b_leaf = dynamic_cast<leaf<T> const*>(&b);

    if (a_leaf && b_leaf) {
        return a_leaf->value() < b_leaf->value();
    }

    return a_leaf && b_branch;
}

template<class T>
inline bool operator>(node<T> const& a, node<T> const& b) {
    return b < a;
}

template<class T>
inline bool operator<=(node<T> const& a, node<T> const& b) {
    return !(b < a);
}

template<class T>
inline bool operator>=(node<T> const& a, node<T> const& b) {
    return !(a < b);
}

} // namespace takatori::util
