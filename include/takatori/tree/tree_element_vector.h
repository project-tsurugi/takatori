#pragma once

#include "tree_fragment_traits.h"
#include "tree_element_util.h"

#include <takatori/util/optional_ptr.h>
#include <takatori/util/ownership_reference.h>
#include <takatori/util/reference_vector.h>

namespace takatori::tree {

template<class T>
class tree_element_vector;

template<class T1, class T2>
bool operator==(tree_element_vector<T1> const& a, tree_element_vector<T2> const& b) noexcept;

template<class T1, class T2>
bool operator!=(tree_element_vector<T1> const& a, tree_element_vector<T2> const& b) noexcept;

template<class T1, class T2>
bool operator<(tree_element_vector<T1> const& a, tree_element_vector<T2> const& b) noexcept;

template<class T1, class T2>
bool operator>(tree_element_vector<T1> const& a, tree_element_vector<T2> const& b) noexcept;

template<class T1, class T2>
bool operator<=(tree_element_vector<T1> const& a, tree_element_vector<T2> const& b) noexcept;

template<class T1, class T2>
bool operator>=(tree_element_vector<T1> const& a, tree_element_vector<T2> const& b) noexcept;

template<class T1, class T2>
int compare(tree_element_vector<T1> const& a, tree_element_vector<T2> const& b) noexcept;

template<class T>
std::ostream& operator<<(std::ostream& out, tree_element_vector<T> const& value);

/**
 * @brief a vector of tree elements.
 * @tparam T the element type
 */
template<class T>
class tree_element_vector {

    static_assert(is_tree_fragment_v<T>, "element type must be able to set its parent");

public:
    /// @brief the entity type
    using entity_type = util::reference_vector<T, util::null_copier<T>>;

    /// @brief the value type
    using value_type = typename entity_type::value_type;
    /// @brief the size type
    using size_type = typename entity_type::size_type;
    /// @brief the difference type
    using difference_type = typename entity_type::difference_type;
    /// @brief the L-value reference type
    using reference = typename entity_type::reference;
    /// @brief the const L-value reference type
    using const_reference = typename entity_type::const_reference;
    /// @brief the R-value reference type
    using rvalue_reference = typename entity_type::rvalue_reference;
    /// @brief the pointer type
    using pointer = typename entity_type::pointer;
    /// @brief the const pointer type
    using const_pointer = typename entity_type::const_pointer;

    /// @brief the iterator type
    using iterator = typename entity_type::iterator;
    /// @brief the const iterator type
    using const_iterator = typename entity_type::const_iterator;
    /// @brief the reverse iterator type
    using reverse_iterator = typename entity_type::reverse_iterator;
    /// @brief the const reverse iterator type
    using const_reverse_iterator = typename entity_type::const_reverse_iterator;

    /// @brief the parent element type
    using parent_type = typename tree_fragment_traits<value_type>::parent_type;

    /**
     * @brief constructs a new object.
     * @param parent the parent element
     */
    explicit tree_element_vector(util::optional_ptr<parent_type> parent) noexcept
        : parent_ { std::move(parent) }
    {}

    /**
     * @brief constructs a new object.
     * @attention This requires U <: T.
     * @tparam U the source element type
     * @tparam C the copier type (ignored)
     * @param parent the parent element
     * @param elements the source elements
     */
    template<class U, class C = typename util::reference_vector<value_type>::copier_type>
    explicit tree_element_vector(util::optional_ptr<parent_type> parent, util::reference_vector<U, C> elements) noexcept
        : parent_(std::move(parent)), elements_(std::move(elements))
    {
        bless();
    }

    /**
     * @brief assigns elements in the list.
     * @attention This requires U <: T.
     * @tparam U the element type
     * @tparam C the copier type
     * @param elements the elements
     * @return this
     */
    template<class U, class C = typename util::reference_vector<U>::copier_type>
    tree_element_vector& operator=(util::reference_vector<U, C> elements) noexcept {
        assign(std::move(elements));
        return *this;
    }

    /**
     * @brief change its parent element.
     * @param parent the parent element
     */
    void parent_element(parent_type* parent) {
        parent_.reset(parent);
        bless();
    }

    /**
     * @brief returns an element at the position.
     * @param position the element index on this vector
     * @return the element on the position
     * @throws std::out_of_bound if the position is out of bound
     */
    [[nodiscard]] reference at(size_type position) {
        return elements_.at(position);
    }

    /// @copydoc at()
    [[nodiscard]] const_reference at(size_type position) const {
        return elements_.at(position);
    }

    /**
     * @brief returns an element at the position.
     * @param position the element index on this vector
     * @return the element on the position
     * @warning undefined behavior if the position is out of bound
     */
    [[nodiscard]] reference operator[](size_type position) {
        return elements_[position];
    }

    /// @copydoc operator[]()
    [[nodiscard]] const_reference operator[](size_type position) const {
        return elements_[position];
    }

    /**
     * @brief returns reference of the first element
     * @return reference of the first element
     * @warning undefined behavior if this is empty
     */
    [[nodiscard]] reference front() {
        return elements_.front();
    }

    /// @copydoc front()
    [[nodiscard]] const_reference front() const {
        return elements_.front();
    }

    /**
     * @brief returns reference of the last element
     * @return reference of the last element
     * @warning undefined behavior if this is empty
     */
    [[nodiscard]] reference back() {
        return elements_.back();
    }

    /// @copydoc back()
    [[nodiscard]] const_reference back() const {
        return elements_.back();
    }

    /**
     * @brief returns whether or not this is empty.
     * @return true if this is empty
     * @return false otherwise
     */
    [[nodiscard]] bool empty() const noexcept {
        return elements_.empty();
    }

    /**
     * @brief returns the number of elements in this.
     * @return the number of elements
     */
    [[nodiscard]] size_type size() const noexcept {
        return elements_.size();
    }

    /**
     * @brief returns the capacity size of this.
     * @return the max number of elements to store without expanding this container
     */
    [[nodiscard]] size_type capacity() const noexcept {
        return elements_.capacity();
    }

    /**
     * @brief reserves the capacity.
     * @param size the number of elements to store without expanding this container
     */
    void reserve(size_type size) {
        elements_.reserve(size);
    }

    /**
     * @brief may shrink capacity size to fit to the current storing elements.
     */
    void shrink_to_fit() {
        elements_.shrink_to_fit();
    }

    /**
     * @brief removes all elements in this.
     */
    void clear() noexcept {
        elements_.clear();
    }

    /**
     * @brief assigns elements in the list.
     * @tparam U the element type
     * @tparam C the copier type
     * @param elements the elements
     */
    template<class U, class C = typename util::reference_vector<U>::copier_type>
    std::enable_if_t<std::is_convertible_v<U&, reference>>
    assign(util::reference_vector<U, C> elements) {
        elements_ = std::move(elements);
        bless();
    }

    /**
     * @brief removes the element on the given position.
     * @param position the target element position
     * @return the next position of the erased element
     */
    iterator erase(const_iterator position) noexcept {
        return elements_.erase(position);
    }

    /**
     * @brief removes the elements between the span.
     * @param first the starting position (inclusive)
     * @param last the ending position (exclusive)
     * @return the next position of the "last"
     */
    iterator erase(const_iterator first, const_iterator last) noexcept {
        return elements_.erase(first, last);
    }

    /**
     * @brief removes the last element.
     * @warning undefined behavior if this is empty
     */
    void pop_back() noexcept {
        elements_.pop_back();
    }

    /**
     * @brief inserts the given element.
     * @tparam U the value type
     * @tparam D the deleter type
     * @param position the insertion position
     * @param element the target element
     * @return the position where the element inserted
     */
    template<class U, class D>
    std::enable_if_t<std::is_convertible_v<U&, reference>, iterator>
    insert(const_iterator position, std::unique_ptr<U, D> element) {
        auto result = elements_.insert(position, std::move(element));
        bless(*result);
        return result;
    }

    /**
     * @brief appends the given element into the tail of this.
     * @tparam U the value type
     * @tparam D the deleter type
     * @param element the source element
     * @return the element
     */
    template<class U, class D>
    std::enable_if_t<std::is_convertible_v<U&, reference>, reference>
    push_back(std::unique_ptr<U, D> element) {
        auto&& result = elements_.push_back(std::move(element));
        bless(result);
        return result;
    }

    /**
     * @brief inserts a new element.
     * @tparam U the element type to insert
     * @tparam Args the constructor parameter type of U
     * @param position the insertion position
     * @param args the constructor arguments of U
     * @return the inserted position
     */
    template<class U = T, class... Args>
    iterator emplace(const_iterator position, Args&&... args) {
        auto result = elements_.template emplace<U>(position, std::forward<Args>(args)...);
        bless(*result);
        return result;
    }

    /**
     * @brief appends a new element into the tail of this.
     * @tparam U the element type to insert
     * @tparam Args the constructor parameter type of U
     * @param args the constructor arguments of U
     * @return the created element
     */
    template<class U = T, class... Args>
    reference emplace_back(Args&&... args) {
        auto&& result = elements_.template emplace_back<U>(std::forward<Args>(args)...);
        bless(result);
        return result;
    }

    /**
     * @brief replaces an existing element with the given element.
     * @tparam U the value type
     * @tparam D the deleter type
     * @param position the target element position
     * @param element the source element
     * @return the replacement
     */
    template<class U, class D>
    std::enable_if_t<std::is_convertible_v<U&, reference>, reference>
    put(const_iterator position, std::unique_ptr<U, D> element) {
        auto&& result = elements_.put(position, std::move(element));
        bless(result);
        return result;
    }

    /**
     * @brief replaces an existing element with a new element.
     * @tparam U the replacement type
     * @tparam Args the constructor parameter type of U
     * @param position the target element position
     * @param args the constructor arguments of U
     * @return the replacement
     */
    template<class U = T, class... Args>
    reference replace(const_iterator position, Args&&... args) {
        auto&& result = elements_.template replace<U>(position, std::forward<Args>(args)...);
        bless(result);
        return result;
    }

    /**
     * @brief releases element on the given position.
     * @param position the target position
     * @return a pair of the removed element, and the next position of the released element
     */
    [[nodiscard]] std::pair<std::unique_ptr<value_type>, iterator> release(const_iterator position) noexcept {
        auto result = elements_.release(position);
        if (auto&& p = std::get<0>(result)) {
            unbless(*p);
        }
        return result;
    }

    /**
     * @brief releases element between the given range.
     * @tparam C the copier type
     * @param first the first position (inclusive)
     * @param last the last position (exclusive)
     * @return a pair of the removed element, and the next position of the released element
     */
    template<class C = typename util::reference_vector<value_type>::copier_type>
    [[nodiscard]] std::pair<util::reference_vector<value_type, C>, iterator> release(const_iterator first, const_iterator last) noexcept {
        auto result = elements_.template release<C>(first, last);
        for (auto&& e : std::get<0>(result)) {
            unbless_element(e);
        }
        return result;
    }

    /**
     * @brief releases the last element.
     * @return the removed element
     * @warning undefined behavior if this is empty
     */
    [[nodiscard]] std::unique_ptr<value_type> release_back() noexcept {
        auto result = elements_.release_back();
        if (result) {
            unbless(*result);
        }
        return result;
    }

    /**
     * @brief releases all elements.
     * @tparam C the copier type
     * @return the released elements
     */
    template<class C = typename util::reference_vector<value_type>::copier_type>
    [[nodiscard]] util::reference_vector<value_type, C> release_elements() noexcept {
        unbless();
        util::reference_vector<value_type, C> result { std::move(elements_) };
        return result;
    }

    /**
     * @brief replaces an existing element with the given element, and returns the existing.
     * @tparam U the value type
     * @tparam D the deleter type
     * @param position the target element position
     * @param element the replacement
     * @return the existing element
     */
    template<class U, class D>
    [[nodiscard]] std::enable_if_t<
            std::is_convertible_v<U&, reference>,
            std::unique_ptr<value_type>>
    exchange(const_iterator position, std::unique_ptr<U, D> element) noexcept {
        auto index = static_cast<size_type>(std::distance(cbegin(), position));
        auto result = elements_.exchange(position, std::move(element));
        bless(at(index));
        if (result) {
            unbless(*result);
        }
        return result;
    }

    /**
     * @brief returns ownership reference of the element on the given position.
     * @details If the entry was modified via the returned ownership, it behaves like as if
     *      put(const_iterator, std::unique_ptr<U, D>) was called.
     * @attention the ownership reference will be invalidated after some elements are added or removed.
     * @param position the target position
     * @return the element on the given position
     */
    [[nodiscard]] util::ownership_reference<value_type> ownership(const_iterator position) {
        using ownership_ref = util::ownership_reference<value_type>;
        return ownership_ref {
                [position, this]() -> typename ownership_ref::pointer {
                    return std::addressof(at(static_cast<size_type>(position - cbegin())));
                },
                [position, this](typename ownership_ref::unique_pointer replacement)
                        -> typename ownership_ref::unique_pointer {
                    return exchange(position, std::move(replacement));
                }
        };
    }

    /**
     * @brief returns a forward iterator which points the beginning of this container.
     * @return the iterator of beginning (inclusive)
     */
    [[nodiscard]] iterator begin() noexcept {
        return elements_.begin();
    }

    /// @copydoc begin()
    [[nodiscard]] const_iterator begin() const noexcept {
        return elements_.begin();
    }

    /// @copydoc begin()
    [[nodiscard]] const_iterator cbegin() const noexcept {
        return elements_.cbegin();
    }

    /**
     * @brief returns a forward iterator which points the ending of this container.
     * @return the iterator of ending (exclusive)
     */
    [[nodiscard]] iterator end() noexcept {
        return elements_.end();
    }

    /// @copydoc end()
    [[nodiscard]] const_iterator end() const noexcept {
        return elements_.end();
    }

    /// @copydoc end()
    [[nodiscard]] const_iterator cend() const noexcept {
        return elements_.cend();
    }

    /**
     * @brief returns a backward iterator which points the reversed beginning of this container.
     * @return the reversed iterator of beginning (inclusive)
     */
    [[nodiscard]] reverse_iterator rbegin() noexcept {
        return elements_.rbegin();
    }

    /// @copydoc rbegin()
    [[nodiscard]] const_reverse_iterator rbegin() const noexcept {
        return elements_.rbegin();
    }

    /// @copydoc rbegin()
    [[nodiscard]] const_reverse_iterator crbegin() const noexcept {
        return elements_.crbegin();
    }

    /**
     * @brief returns a backward iterator which points the reversed ending of this container.
     * @return the reversed iterator of ending (exclusive)
     */
    [[nodiscard]] reverse_iterator rend() noexcept {
        return elements_.rend();
    }

    /// @copydoc rend()
    [[nodiscard]] const_reverse_iterator rend() const noexcept {
        return elements_.rend();
    }

    /// @copydoc rend()
    [[nodiscard]] const_reverse_iterator crend() const noexcept {
        return elements_.crend();
    }

    /**
     * @brief exchanges contents between this and the given container.
     * @param other the opposite container
     */
    void swap(tree_element_vector& other) noexcept {
        std::swap(elements_, other.elements_);
    }

private:
    util::optional_ptr<parent_type> parent_;
    entity_type elements_ {};

    void bless(reference element) { bless_element(parent_, element); }
    void unbless(reference element) { unbless_element(element); }

    void bless() {
        for (value_type& e : elements_) {
            bless(e);
        }
    }

    void unbless() {
        for (value_type& e : elements_) {
            unbless(e);
        }
    }

    template<class U>
    friend class tree_element_vector;

    template<class T1, class T2>
    friend bool operator==(tree_element_vector<T1> const& a, tree_element_vector<T2> const& b) noexcept; // NOLINT(readability-redundant-declaration)

    template<class T1, class T2>
    friend bool operator!=(tree_element_vector<T1> const& a, tree_element_vector<T2> const& b) noexcept; // NOLINT(readability-redundant-declaration)

    template<class T1, class T2>
    friend bool operator<(tree_element_vector<T1> const& a, tree_element_vector<T2> const& b) noexcept; // NOLINT(readability-redundant-declaration)

    template<class T1, class T2>
    friend bool operator>(tree_element_vector<T1> const& a, tree_element_vector<T2> const& b) noexcept; // NOLINT(readability-redundant-declaration)

    template<class T1, class T2>
    friend bool operator<=(tree_element_vector<T1> const& a, tree_element_vector<T2> const& b) noexcept; // NOLINT(readability-redundant-declaration)

    template<class T1, class T2>
    friend bool operator>=(tree_element_vector<T1> const& a, tree_element_vector<T2> const& b) noexcept; // NOLINT(readability-redundant-declaration)
    
    template<class T1, class T2>
    friend int compare(tree_element_vector<T1> const& a, tree_element_vector<T2> const& b) noexcept; // NOLINT(readability-redundant-declaration)

    template<class U>
    friend std::ostream& operator<<(std::ostream& out, tree_element_vector<U> const& value); // NOLINT(readability-redundant-declaration)
};

/// @private
template<class T, class C>
tree_element_vector(util::reference_vector<T, C>) -> tree_element_vector<T>;

/**
 * @brief returns whether or not the both vectors have equivalent elements.
 * @tparam T1 element type of the first vector
 * @tparam T2 element type of the second vector
 * @param a the first vector
 * @param b the second vector
 * @return true if a = b
 * @return false other wise
 */
template<class T1, class T2>
inline bool operator==(tree_element_vector<T1> const& a, tree_element_vector<T2> const& b) noexcept {
    return a.elements_ == b.elements_;
}

/**
 * @brief returns whether or not the both vectors have different elements.
 * @tparam T1 element type of the first vector
 * @tparam T2 element type of the second vector
 * @param a the first vector
 * @param b the second vector
 * @return true if a != b
 * @return false other wise
 */
template<class T1, class T2>
inline bool operator!=(tree_element_vector<T1> const& a, tree_element_vector<T2> const& b) noexcept {
    return a.elements_ != b.elements_;
}

/**
 * @brief returns whether or not the first vector is less than the second one, in lexicographic order.
 * @tparam T1 element type of the first vector
 * @tparam T2 element type of the second vector
 * @param a the first vector
 * @param b the second vector
 * @return true if a < b
 * @return false other wise
 */
template<class T1, class T2>
inline bool operator<(tree_element_vector<T1> const& a, tree_element_vector<T2> const& b) noexcept {
    return a.elements_ < b.elements_;
}

/**
 * @brief returns whether or not the first vector is greater than the second one, in lexicographic order.
 * @tparam T1 element type of the first vector
 * @tparam T2 element type of the second vector
 * @param a the first vector
 * @param b the second vector
 * @return true if a > b
 * @return false other wise
 */
template<class T1, class T2>
inline bool operator>(tree_element_vector<T1> const& a, tree_element_vector<T2> const& b) noexcept {
    return a.elements_ > b.elements_;
}

/**
 * @brief returns whether or not the first vector is less than or equal to the second one, in lexicographic order.
 * @tparam T1 element type of the first vector
 * @tparam T2 element type of the second vector
 * @param a the first vector
 * @param b the second vector
 * @return true if a <= b
 * @return false other wise
 */
template<class T1, class T2>
inline bool operator<=(tree_element_vector<T1> const& a, tree_element_vector<T2> const& b) noexcept {
    return a.elements_ <= b.elements_;
}

/**
 * @brief returns whether or not the first vector is greater than or equal to the second one, in lexicographic order.
 * @tparam T1 element type of the first vector
 * @tparam T2 element type of the second vector
 * @param a the first vector
 * @param b the second vector
 * @return true if a >= b
 * @return false other wise
 */
template<class T1, class T2>
inline bool operator>=(tree_element_vector<T1> const& a, tree_element_vector<T2> const& b) noexcept {
    return a.elements_ >= b.elements_;
}

/**
 * @brief compares elements in the given vectors.
 * @tparam T1 element type of the first vector
 * @tparam T2 element type of the second vector
 * @param a the first vector
 * @param b the second vector
 * @return < 0 if a < b
 * @return = 0 if a = b
 * @return > 0 if a > b
 */
template<class T1, class T2>
inline int compare(tree_element_vector<T1> const& a, tree_element_vector<T2> const& b) noexcept {
    return compare(a.elements_, b.elements_);
}

/**
 * @brief appends string representation of the given value.
 * @tparam T the element type
 * @param out the target output
 * @param value the target value
 * @return the output
 */
template<class T>
inline std::ostream& operator<<(std::ostream& out, tree_element_vector<T> const& value) {
    return out << value.elements_;
}

} // namespace takatori::tree
