#pragma once

#include <type_traits>
#include <vector>

#include "tree_fragment_traits.h"
#include "tree_element_util.h"

#include <takatori/util/assertion.h>
#include <takatori/util/object_creator.h>
#include <takatori/util/optional_ptr.h>
#include <takatori/util/rvalue_initializer_list.h>

namespace takatori::tree {

template<class T>
class tree_fragment_vector;

template<class T1, class T2>
bool operator==(tree_fragment_vector<T1> const& a, tree_fragment_vector<T2> const& b) noexcept;

template<class T1, class T2>
bool operator!=(tree_fragment_vector<T1> const& a, tree_fragment_vector<T2> const& b) noexcept;

template<class T1, class T2>
bool operator<(tree_fragment_vector<T1> const& a, tree_fragment_vector<T2> const& b) noexcept;

template<class T1, class T2>
bool operator>(tree_fragment_vector<T1> const& a, tree_fragment_vector<T2> const& b) noexcept;

template<class T1, class T2>
bool operator<=(tree_fragment_vector<T1> const& a, tree_fragment_vector<T2> const& b) noexcept;

template<class T1, class T2>
bool operator>=(tree_fragment_vector<T1> const& a, tree_fragment_vector<T2> const& b) noexcept;

template<class T1, class T2>
int compare(tree_fragment_vector<T1> const& a, tree_fragment_vector<T2> const& b) noexcept;

template<class T>
std::ostream& operator<<(std::ostream& out, tree_fragment_vector<T> const& value);

/**
 * @brief a vector of tree fragments.
 * @details As compare to tree_element_vector, this stores each object directly instead of via references.
 * @tparam T the fragment type
 */
template<class T>
class tree_fragment_vector {

    static_assert(is_tree_fragment_v<T>, "element type must be able to set its parent");

public:
    /// @brief the value type
    using value_type = T;
    /// @brief internal allocator type.
    using allocator_type = util::object_allocator<T>;
    /// @brief internal entity type
    using entity_type = std::vector<value_type, allocator_type>;

    /// @brief the size type
    using size_type = std::size_t;
    /// @brief the difference type
    using difference_type = std::ptrdiff_t;
    /// @brief the L-value reference type
    using reference = std::add_lvalue_reference_t<value_type>;
    /// @brief the const L-value reference type
    using const_reference = std::add_lvalue_reference_t<std::add_const_t<value_type>>;
    /// @brief the R-value reference type
    using rvalue_reference = std::add_rvalue_reference_t<value_type>;
    /// @brief the pointer type
    using pointer = std::add_pointer_t<value_type>;
    /// @brief the const pointer type
    using const_pointer = std::add_pointer_t<std::add_const_t<value_type>>;

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
    tree_fragment_vector(util::optional_ptr<parent_type> parent = {}, util::object_creator creator = {}) // NOLINT
        : parent_(std::move(parent))
        , elements_(creator.allocator(std::in_place_type<value_type>))
    {
        bless(elements_);
    }

    /**
     * @brief constructs a new object.
     * @param parent the parent element
     * @param elements the entity vector
     */
    tree_fragment_vector(util::optional_ptr<parent_type> parent, entity_type elements) noexcept
        : parent_(std::move(parent))
        , elements_(std::move(elements))
    {
        bless(elements_);
    }

    /**
     * @brief constructs a new object.
     * @param parent the parent element
     * @param elements the initial elements
     * @param creator the object creator
     */
    template<class Allocator>
    tree_fragment_vector(
            util::optional_ptr<parent_type> parent,
            std::vector<value_type, Allocator> elements,
            util::object_creator creator = {})
        : parent_(std::move(parent))
        , elements_(adapt_vector(std::move(elements), creator))
    {
        bless(elements_);
    }

    /**
     * @brief constructs a new object.
     * @param parent the parent element
     * @param elements the initial elements
     * @param creator the object creator
     */
    tree_fragment_vector(
            util::optional_ptr<parent_type> parent,
            std::initializer_list<T> elements,
            util::object_creator creator = {})
        : tree_fragment_vector(parent, creator)
    {
        assign(elements);
    }

    /**
     * @brief constructs a new object.
     * @param parent the parent element
     * @param elements the initial elements
     * @param creator the object creator
     */
    tree_fragment_vector(
            util::optional_ptr<parent_type> parent,
            util::rvalue_initializer_list<T> elements,
            util::object_creator creator = {})
        : tree_fragment_vector(parent, creator)
    {
        assign(std::move(elements));
    }

    /**
     * @brief constructs a new object.
     * @tparam Iter the iterator type
     * @param parent the parent element
     * @param first the first position of initial elements (inclusive)
     * @param last the last position of initial elements (exclusive)
     * @param creator the object creator
     */
    template<class Iter>
    tree_fragment_vector(
            util::optional_ptr<parent_type> parent,
            Iter first,
            Iter last,
            util::object_creator creator = {})
        : tree_fragment_vector(parent, creator)
    {
        assign(first, last);
    }

    // TODO: tune copy/move

    /**
     * @brief change its parent element.
     * @param parent the parent element
     */
    void parent_element(parent_type* parent) {
        parent_.reset(parent);
        bless(elements_);
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
     * @brief assigns copy of elements in the list.
     * @param list the source elements
     */
    void assign(std::initializer_list<T> list) {
        elements_.assign(list);
        bless(elements_);
    }

    /**
     * @brief assigns elements in the list.
     * @param list the source elements
     */
    void assign(util::rvalue_initializer_list<T> list) {
        elements_.assign(list.begin(), list.end());
        bless(elements_);
    }

    /**
     * @brief assigns copy of elements between the given span.
     * @tparam Iter the iterator type
     * @param first the starting position (inclusive)
     * @param last the ending position (exclusive)
     */
    template<class Iter>
    void assign(Iter first, Iter last) {
        elements_.assign(first, last);
        bless(elements_);
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
     * @brief inserts a copy of the given element.
     * @param position the insertion position
     * @param element the target element
     * @return the position where the element inserted
     */
    iterator insert(const_iterator position, const_reference element) {
        auto result = elements_.insert(position, element);
        bless(*result);
        return result;
    }

    /// @copydoc insert(const_iterator, const_reference)
    iterator insert(const_iterator position, rvalue_reference element) {
        auto result = elements_.insert(position, std::move(element));
        bless(*result);
        return result;
    }

    /**
     * @brief inserts copy of elements between the given list.
     * @param position the insertion position
     * @param list the source elements
     * @return the position where the first element inserted
     */
    iterator insert(const_iterator position, std::initializer_list<T> list) {
        auto before = elements_.size();
        auto result = elements_.insert(position, list);
        auto after = elements_.size();
        BOOST_ASSERT(after >= before); // NOLINT
        bless(result, result + (after - before));
        return result;
    }

    /**
     * @brief inserts elements between the given list.
     * @param position the insertion position
     * @param list the source elements
     * @return the position where the first element inserted
     */
    iterator insert(const_iterator position, util::rvalue_initializer_list<T> list) {
        auto before = elements_.size();
        auto result = elements_.insert(position, list.begin(), list.end());
        auto after = elements_.size();
        BOOST_ASSERT(after >= before); // NOLINT
        bless(result, result + (after - before));
        return result;
    }

    /**
     * @brief inserts elements between the given span.
     * @tparam Iter the iterator type
     * @param position the insertion position
     * @param first the starting position (inclusive)
     * @param last the ending position (exclusive)
     * @return the position where the first element inserted
     */
    template<class Iter>
    iterator insert(const_iterator position, Iter first, Iter last) {
        auto before = elements_.size();
        auto result = elements_.insert(position, first, last);
        auto after = elements_.size();
        BOOST_ASSERT(after >= before); // NOLINT
        bless(result, result + (after - before));
        return result;
    }

    /**
     * @brief appends a copy of element into the tail of this.
     * @param element the source element
     * @return the created copy of source
     */
    reference push_back(const_reference element) {
        elements_.push_back(element);
        auto&& result = elements_.back();
        bless(result);
        return result;
    }

    /// @copydoc push_back(const_reference)
    reference push_back(rvalue_reference element) {
        elements_.push_back(std::move(element));
        auto&& result = elements_.back();
        bless(result);
        return result;
    }

    /**
     * @brief inserts a new element.
     * @tparam Args the constructor parameter type
     * @param position the insertion position
     * @param args the constructor arguments
     * @return the inserted position
     */
    template<class... Args>
    iterator emplace(const_iterator position, Args&&... args) {
        auto result = elements_.emplace(position, std::forward<Args>(args)...);
        bless(*result);
        return result;
    }

    /**
     * @brief appends a new element into the tail of this.
     * @tparam Args the constructor parameter type
     * @param args the constructor arguments
     * @return the created element
     */
    template<class... Args>
    reference emplace_back(Args&&... args) {
        auto&& result = elements_.emplace_back(std::forward<Args>(args)...);
        bless(result);
        return result;
    }

    /**
     * @brief releases all elements.
     * @return the released elements
     */
    entity_type release_elements() noexcept {
        auto creator = get_object_creator();
        auto result = std::move(elements_);
        unbless(result);
        elements_ = entity_type(creator.allocator(std::in_place_type<value_type>));
        return result;
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
     * @details This also exchanges their util::object_creator, but object copying strategy does not.
     * @param other the opposite container
     */
    void swap(tree_fragment_vector& other) noexcept {
        std::swap(elements_, other.elements_);
    }

    /**
     * @brief returns the object creator to create/delete objects in this container.
     * @return the object creator for this container
     */
    [[nodiscard]] util::object_creator get_object_creator() const noexcept {
        return elements_.get_allocator();
    }

private:
    util::optional_ptr<parent_type> parent_;
    entity_type elements_;

    using traits = tree_fragment_traits<value_type>;

    template<class Allocator>
    static entity_type adapt_vector(
            std::vector<T, Allocator> elements,
            util::object_creator creator = {}) {
        if constexpr (std::is_same_v<decltype(elements), entity_type>) {
            if (creator.is_compatible(elements.get_allocator())) {
                return std::move(elements);
            }
            return { std::move(elements), creator.allocator() };
        }
        return {
                std::make_move_iterator(elements.begin()),
                std::make_move_iterator(elements.end()),
                creator.allocator()
        };
    }

    void bless(reference element) {
        traits::set_parent_element(element, parent_);
    }

    void unbless(reference element) {
        traits::set_parent_element(element, nullptr);
    }

    template<class Iter>
    void bless(Iter first, Iter last) {
        BOOST_ASSERT(first <= last); // NOLINT
        for (auto iter = first; iter != last; ++iter) {
            bless(*iter);
        }
    }

    void bless(entity_type& elements) {
        for (auto&& e : elements) {
            bless(e);
        }
    }

    void unbless(entity_type& elements) {
        for (auto&& e : elements) {
            unbless(e);
        }
    }

    template<class U>
    friend class tree_fragment_vector;

    template<class T1, class T2>
    friend bool operator==(tree_fragment_vector<T1> const& a, tree_fragment_vector<T2> const& b) noexcept; // NOLINT(readability-redundant-declaration)

    template<class T1, class T2>
    friend bool operator!=(tree_fragment_vector<T1> const& a, tree_fragment_vector<T2> const& b) noexcept; // NOLINT(readability-redundant-declaration)

    template<class T1, class T2>
    friend bool operator<(tree_fragment_vector<T1> const& a, tree_fragment_vector<T2> const& b) noexcept; // NOLINT(readability-redundant-declaration)

    template<class T1, class T2>
    friend bool operator>(tree_fragment_vector<T1> const& a, tree_fragment_vector<T2> const& b) noexcept; // NOLINT(readability-redundant-declaration)

    template<class T1, class T2>
    friend bool operator<=(tree_fragment_vector<T1> const& a, tree_fragment_vector<T2> const& b) noexcept; // NOLINT(readability-redundant-declaration)

    template<class T1, class T2>
    friend bool operator>=(tree_fragment_vector<T1> const& a, tree_fragment_vector<T2> const& b) noexcept; // NOLINT(readability-redundant-declaration)

    template<class T1, class T2>
    friend int compare(tree_fragment_vector<T1> const& a, tree_fragment_vector<T2> const& b) noexcept; // NOLINT(readability-redundant-declaration)

    template<class U>
    friend std::ostream& operator<<(std::ostream& out, tree_fragment_vector<U> const& value); // NOLINT(readability-redundant-declaration)
};

/// @private
template<class P, class T, class A>
tree_fragment_vector(P&&, std::vector<T, A>, util::object_creator = {})
-> tree_fragment_vector<T>;

/// @private
template<class P, class T>
tree_fragment_vector(P&&, std::initializer_list<T>, util::object_creator = {})
-> tree_fragment_vector<T>;

/// @private
template<class P, class T>
tree_fragment_vector(P&&, util::rvalue_initializer_list<T>, util::object_creator = {})
-> tree_fragment_vector<T>;

/// @private
template<class P, class Iter>
tree_fragment_vector(P&&, Iter, Iter, util::object_creator = {})
-> tree_fragment_vector<typename std::iterator_traits<Iter>::value_type>;

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
inline bool
operator==(tree_fragment_vector<T1> const& a, tree_fragment_vector<T2> const& b) noexcept {
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
inline bool
operator!=(tree_fragment_vector<T1> const& a, tree_fragment_vector<T2> const& b) noexcept {
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
inline bool
operator<(tree_fragment_vector<T1> const& a, tree_fragment_vector<T2> const& b) noexcept {
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
inline bool
operator>(tree_fragment_vector<T1> const& a, tree_fragment_vector<T2> const& b) noexcept {
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
inline bool
operator<=(tree_fragment_vector<T1> const& a, tree_fragment_vector<T2> const& b) noexcept {
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
inline bool
operator>=(tree_fragment_vector<T1> const& a, tree_fragment_vector<T2> const& b) noexcept {
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
inline int compare(tree_fragment_vector<T1> const& a, tree_fragment_vector<T2> const& b) noexcept {
    for (std::size_t i = 0, n = std::min(a.size(), b.size()); i < n; ++i) {
        if (a[i] < b[i]) return -1;
        if (b[i] < a[i]) return +1;
    }
    if (a.size() < b.size()) return -1;
    if (b.size() < a.size()) return +1;
    return 0;
}

/**
 * @brief appends string representation of the given value.
 * @tparam T the element type
 * @param out the target output
 * @param value the target value
 * @return the output
 */
template<class T>
inline std::ostream& operator<<(std::ostream& out, tree_fragment_vector<T> const& value) {
    out << "{";
    bool cont = false;
    for (auto&& element : value) {
        if (cont) out << ", ";
        cont = true;
        out << element;
    }
    out << "}";
    return out;
}

} // namespace takatori::tree
