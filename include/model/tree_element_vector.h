#pragma once

#include "model/tree_fragment_traits.h"
#include "model/tree_element_util.h"

#include "util/reference_vector.h"

namespace model {

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
     * @param creator the object creator
     */
    explicit tree_element_vector(parent_type& parent, util::object_creator creator = {}) noexcept;

    /**
     * @brief constructs a new object.
     * This requires U <: T.
     * @tparam U the source element type
     * @tparam C the copier type (ignored)
     * @param elements the source elements
     */
    template<class U, class C = typename util::reference_vector<value_type>::copier_type>
    explicit tree_element_vector(parent_type& parent, util::reference_vector<U, C> elements) noexcept;

    /**
     * @brief assigns elements in the list.
     * This requires U <: T.
     * @tparam U the element type
     * @tparam C the copier type
     * @param elements the elements
     * @return this
     */
    template<class U, class C = typename util::reference_vector<U>::copier_type>
    tree_element_vector& operator=(util::reference_vector<U, C> elements) noexcept;

    /**
     * @brief returns an element at the position.
     * @param position the element index on this vector
     * @return the element on the position
     * @throws std::out_of_bound if the position is out of bound
     */
    reference at(size_type position) { return elements_.at(position); }

    /// @copydoc at()
    const_reference at(size_type position) const { return elements_.at(position); }

    /**
     * @brief returns an element at the position.
     * @param position the element index on this vector
     * @return the element on the position
     * @warning undefined behavior if the position is out of bound
     */
    reference operator[](size_type position) { return elements_[position]; }

    /// @copydoc operator[]()
    const_reference operator[](size_type position) const { return elements_[position]; }

    /**
     * @brief returns reference of the first element
     * @return reference of the first element
     * @warning undefined behavior if this is empty
     */
    reference front() { return elements_.front(); }

    /// @copydoc front()
    const_reference front() const { return elements_.front(); }

    /**
     * @brief returns reference of the last element
     * @return reference of the last element
     * @warning undefined behavior if this is empty
     */
    reference back() { return elements_.back(); }

    /// @copydoc back()
    const_reference back() const { return elements_.back(); }

    /**
     * @brief returns whether or not this is empty.
     * @return true if this is empty
     * @return false otherwise
     */
    bool empty() const noexcept { return elements_.empty(); }

    /**
     * @brief returns the number of elements in this.
     * @return the number of elements
     */
    size_type size() const noexcept { return elements_.size(); }

    /**
     * @brief returns the capacity size of this.
     * @return the max number of elements to store without expanding this container
     */
    size_type capacity() const noexcept { return elements_.capacity(); }

    /**
     * @brief reserves the capacity.
     * @param size the number of elements to store without expanding this container
     */
    void reserve(size_type size) { elements_.reserve(size); }

    /**
     * @brief may shrink capacity size to fit to the current storing elements.
     */
    void shrink_to_fit() { elements_.shrink_to_fit(); }

    /**
     * @brief removes all elements in this.
     */
    void clear() noexcept { elements_.clear(); }

    /**
     * @brief assigns elements in the list.
     * @tparam U the element type
     * @tparam C the copier type
     * @param elements the elements
     */
    template<class U, class C = typename util::reference_vector<U>::copier_type>
    std::enable_if_t<std::is_convertible_v<U&, reference>>
    assign(util::reference_vector<U, C> elements);

    /**
     * @brief removes the element on the given position.
     * @param position the target element position
     * @return the next position of the erased element
     */
    iterator erase(const_iterator position) noexcept;

    /**
     * @brief removes the elements between the span.
     * @param first the starting position (inclusive)
     * @param last the ending position (exclusive)
     * @return the next position of the "last"
     */
    iterator erase(const_iterator first, const_iterator last) noexcept;

    /**
     * @brief removes the last element.
     * @warning undefined behavior if this is empty
     */
    void pop_back() noexcept;

    /**
     * @brief inserts the given element.
     * @attention the element must be created by an this container's equivalent object_creator.
     * @tparam U the value type
     * @tparam D the deleter type
     * @param position the insertion position
     * @param element the target element
     * @return the position where the element inserted
     */
    template<class U, class D>
    std::enable_if_t<std::is_convertible_v<U&, reference>, iterator>
    insert(const_iterator position, std::unique_ptr<U, D> element);

    /**
     * @brief appends the given element into the tail of this.
     * @attention the element must be created by an this container's equivalent object_creator.
     * @tparam U the value type
     * @tparam D the deleter type
     * @param element the source element
     * @return the element
     */
    template<class U, class D>
    std::enable_if_t<std::is_convertible_v<U&, reference>, reference>
    push_back(std::unique_ptr<U, D> element);

    /**
     * @brief inserts a new element.
     * @tparam U the element type to insert
     * @tparam Args the constructor parameter type of U
     * @param position the insertion position
     * @param args the constructor arguments of U
     * @return the inserted position
     */
    template<class U = T, class... Args>
    iterator emplace(const_iterator position, Args&&... args);

    /**
     * @brief appends a new element into the tail of this.
     * @tparam U the element type to insert
     * @tparam Args the constructor parameter type of U
     * @param args the constructor arguments of U
     * @return the created element
     */
    template<class U = T, class... Args>
    reference emplace_back(Args&&... args);

    /**
     * @brief replaces an existing element with the given element.
     * @attention the element must be created by an this container's equivalent object_creator.
     * @tparam U the value type
     * @tparam D the deleter type
     * @param position the target element position
     * @param element the source element
     * @return the replacement
     */
    template<class U, class D>
    std::enable_if_t<std::is_convertible_v<U&, reference>, reference>
    put(const_iterator position, std::unique_ptr<U, D> element);

    /**
     * @brief replaces an existing element with a new element.
     * @tparam U the replacement type
     * @tparam Args the constructor parameter type of U
     * @param position the target element position
     * @param args the constructor arguments of U
     * @return the replacement
     */
    template<class U = T, class... Args>
    reference replace(const_iterator position, Args&&... args);

    /**
     * @brief releases element on the given position.
     * @param position the target position
     * @return a pair of the removed element, and the next position of the released element
     */
    std::pair<util::unique_object_ptr<value_type>, iterator> release(const_iterator position) noexcept;

    /**
     * @brief releases element between the given range.
     * @tparam C the copier type
     * @param first the first position (inclusive)
     * @param last the last position (exclusive)
     * @return a pair of the removed element, and the next position of the released element
     */
    template<class C = typename util::reference_vector<value_type>::copier_type>
    std::pair<util::reference_vector<value_type, C>, iterator> release(const_iterator first, const_iterator last) noexcept;

    /**
     * @brief releases the last element.
     * @return the removed element
     * @warning undefined behavior if this is empty
     */
    util::unique_object_ptr<value_type> release_back() noexcept;

    /**
     * @brief releases all elements.
     * @tparam C the copier type
     * @return the released elements
     */
    template<class C = typename util::reference_vector<value_type>::copier_type>
    util::reference_vector<value_type, C> release_elements() noexcept;

    /**
     * @brief returns a forward iterator which points the beginning of this container.
     * @return the iterator of beginning (inclusive)
     */
    iterator begin() noexcept { return elements_.begin(); }

    /// @copydoc begin()
    const_iterator begin() const noexcept { return elements_.begin(); }

    /// @copydoc begin()
    const_iterator cbegin() const noexcept { return elements_.cbegin(); }

    /**
     * @brief returns a forward iterator which points the ending of this container.
     * @return the iterator of ending (exclusive)
     */
    iterator end() noexcept { return elements_.end(); }

    /// @copydoc end()
    const_iterator end() const noexcept { return elements_.end(); }

    /// @copydoc end()
    const_iterator cend() const noexcept { return elements_.cend(); }

    /**
     * @brief returns a backward iterator which points the reversed beginning of this container.
     * @return the reversed iterator of beginning (inclusive)
     */
    reverse_iterator rbegin() noexcept { return elements_.rbegin(); }

    /// @copydoc rbegin()
    const_reverse_iterator rbegin() const noexcept { return elements_.rbegin(); }

    /// @copydoc rbegin()
    const_reverse_iterator crbegin() const noexcept { return elements_.crbegin(); }

    /**
     * @brief returns a backward iterator which points the reversed ending of this container.
     * @return the reversed iterator of ending (exclusive)
     */
    reverse_iterator rend() noexcept { return elements_.rend(); }

    /// @copydoc rend()
    const_reverse_iterator rend() const noexcept { return elements_.rend(); }

    /// @copydoc rend()
    const_reverse_iterator crend() const noexcept { return elements_.crend(); }

    /**
     * @brief exchanges contents between this and the given container.
     * @param other the opposite container
     */
    void swap(tree_element_vector& other) noexcept { std::swap(elements_, other.elements_); }

    /**
     * @brief returns the object creator to create/delete objects in this container.
     * @return the object creator for this container
     */
    util::object_creator get_object_creator() const noexcept { return elements_.get_object_creator(); }

private:
    parent_type* parent_;
    entity_type elements_ {};

    void bless(reference element) { bless_element(*parent_, element); }
    void unbless(reference element) { unbless_element(element); }

    void bless() {
        for (auto&& e : elements_) {
            bless(e);
        }
    }

    void unbless() {
        for (auto&& e : elements_) {
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

template<class T>
tree_element_vector<T>::tree_element_vector(parent_type& parent, util::object_creator creator) noexcept
    : parent_(std::addressof(parent)), elements_(creator) {}

template<class T>
template<class U, class C>
tree_element_vector<T>::tree_element_vector(parent_type& parent, util::reference_vector<U, C> elements) noexcept
    : parent_(std::addressof(parent)), elements_(std::move(elements))
{
    bless();
}

template<class T>
template<class U, class C>
tree_element_vector<T>&
tree_element_vector<T>::operator=(util::reference_vector<U, C> elements) noexcept {
    assign(std::move(elements));
    return *this;
}

template<class T>
template<class U, class C>
inline std::enable_if_t<
        std::is_convertible_v<U&, typename tree_element_vector<T>::reference>>
tree_element_vector<T>::assign(util::reference_vector<U, C> elements) {
    elements_ = std::move(elements);
    bless();
}

template<class T>
inline typename tree_element_vector<T>::iterator
tree_element_vector<T>::erase(const_iterator position) noexcept {
    return elements_.erase(position);
}

template<class T>
inline typename tree_element_vector<T>::iterator
tree_element_vector<T>::erase(const_iterator first, const_iterator last) noexcept {
    return elements_.erase(first, last);
}

template<class T>
inline void
tree_element_vector<T>::pop_back() noexcept {
    elements_.pop_back();
}

template<class T>
template<class U, class D>
std::enable_if_t<
        std::is_convertible_v<U&, typename tree_element_vector<T>::reference>,
        typename tree_element_vector<T>::iterator>
tree_element_vector<T>::insert(const_iterator position, std::unique_ptr<U, D> element) {
    auto result = elements_.insert(position, std::move(element));
    bless(*result);
    return result;
}

template<class T>
template<class U, class D>
inline std::enable_if_t<
        std::is_convertible_v<U&, typename tree_element_vector<T>::reference>,
        typename tree_element_vector<T>::reference>
tree_element_vector<T>::push_back(std::unique_ptr<U, D> element) {
    auto&& result = elements_.push_back(std::move(element));
    bless(result);
    return result;
}

template<class T>
template<class U, class... Args>
inline typename tree_element_vector<T>::iterator
tree_element_vector<T>::emplace(const_iterator position, Args&& ... args) {
    auto result = elements_.template emplace<U>(position, std::forward<Args>(args)...);
    bless(*result);
    return result;
}

template<class T>
template<class U, class... Args>
inline typename tree_element_vector<T>::reference
tree_element_vector<T>::emplace_back(Args&& ... args) {
    auto&& result = elements_.template emplace_back<U>(std::forward<Args>(args)...);
    bless(result);
    return result;
}

template<class T>
template<class U, class D>
inline std::enable_if_t<
        std::is_convertible_v<U&, typename tree_element_vector<T>::reference>,
        typename tree_element_vector<T>::reference>
tree_element_vector<T>::put(const_iterator position, std::unique_ptr<U, D> element) {
    auto&& result = elements_.put(position, std::move(element));
    bless(result);
    return result;
}

template<class T>
template<class U, class... Args>
inline typename tree_element_vector<T>::reference
tree_element_vector<T>::replace(const_iterator position, Args&& ... args) {
    auto&& result = elements_.template replace<U>(position, std::forward<Args>(args)...);
    bless(result);
    return result;
}

template<class T>
inline std::pair<
        util::unique_object_ptr<typename tree_element_vector<T>::value_type>,
        typename tree_element_vector<T>::iterator>
tree_element_vector<T>::release(const_iterator position) noexcept {
    auto result = elements_.release(position);
    if (auto&& p = std::get<0>(result)) {
        unbless(*p);
    }
    return result;
}

template<class T>
template<class C>
std::pair<
        util::reference_vector<typename tree_element_vector<T>::value_type, C>,
        typename tree_element_vector<T>::iterator>
tree_element_vector<T>::release(const_iterator first, const_iterator last) noexcept {
    auto result = elements_.template release<C>(first, last);
    for (auto&& e : std::get<0>(result)) {
        unbless_element(e);
    }
    return result;
}

template<class T>
inline util::unique_object_ptr<typename tree_element_vector<T>::value_type>
tree_element_vector<T>::release_back() noexcept {
    auto result = elements_.release_back();
    if (result) {
        unbless(*result);
    }
    return result;
}

template<class T>
template<class C>
inline util::reference_vector<typename tree_element_vector<T>::value_type, C>
tree_element_vector<T>::release_elements() noexcept {
    unbless();
    util::reference_vector<value_type, C> result { std::move(elements_) };
    return result;
}

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
template<class U>
inline std::ostream& operator<<(std::ostream& out, tree_element_vector<U> const& value) {
    return out << value.elements_;
}

} // namespace util
