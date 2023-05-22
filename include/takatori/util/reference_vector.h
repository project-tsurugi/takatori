#pragma once

#include <algorithm>
#include <memory>
#include <sstream>
#include <type_traits>
#include <utility>
#include <vector>

#include "clonable.h"
#include "copier.h"
#include "exception.h"
#include "ownership_reference.h"
#include "reference_iterator.h"

#include <takatori/workaround/gcc11.h>

namespace takatori::util {

/// @cond IMPL_DEFS
namespace impl {

template<class T, class A>
inline void delete_vector(std::vector<T*, A>& elements) {
    while (!elements.empty()) {
        auto* element = elements.back();
        delete element; // NOLINT
        elements.pop_back();
    }
}

template<class T>
using reference_vector_entity = std::vector<T*>;

template<class T>
struct reference_vector_storage_base {
    explicit reference_vector_storage_base() noexcept = default;
    ~reference_vector_storage_base() { clear(); } // NOLINT
    reference_vector_storage_base(reference_vector_storage_base const&) = delete;
    reference_vector_storage_base& operator=(reference_vector_storage_base const&) = delete;
    reference_vector_storage_base(reference_vector_storage_base&& other) noexcept :
        elements_ { std::move(other.elements_) }
    {}
    reference_vector_storage_base& operator=(reference_vector_storage_base&& other) noexcept {
        clear();
        elements_ = reference_vector_entity<T>(std::move(other.elements_));
        return *this;
    }

    void clear() noexcept { delete_vector(elements_); }

    reference_vector_entity<T> elements_; // NOLINT
};

template<class T, class C, bool = C::is_available>
struct reference_vector_storage;

template<class T, class C>
struct reference_vector_storage<T, C, true> : reference_vector_storage_base<T> {
    using reference_vector_storage_base<T>::reference_vector_storage_base;
    reference_vector_storage() noexcept = default;

    ~reference_vector_storage() = default;

    reference_vector_storage(reference_vector_storage const& other)
        : reference_vector_storage_base<T> {}
    {
        elements_ = other.copy_elements();
    }

    reference_vector_storage& operator=(reference_vector_storage const& other) {
        auto elements = other.copy_elements();
        clear();
        elements_ = std::move(elements);
        return *this;
    }

    reference_vector_storage(reference_vector_storage&&) noexcept = default;
    reference_vector_storage& operator=(reference_vector_storage&&) noexcept = default;

    using reference_vector_storage_base<T>::clear;
    using reference_vector_storage_base<T>::elements_;

    [[nodiscard]] reference_vector_entity<T> copy_elements() const {
        reference_vector_entity<T> result {};
        result.reserve(elements_.size());
        try {
            for (auto* element : elements_) {
                result.push_back(C::copy(*element).release());
            }
            return result;
        } catch (...) {
            delete_vector(result);
            std::rethrow_exception(std::current_exception());
        }
    }
};

template<class T, class C>
struct reference_vector_storage<T, C, false> : reference_vector_storage_base<T> {
    using reference_vector_storage_base<T>::reference_vector_storage_base;
    reference_vector_storage() noexcept = default;
    ~reference_vector_storage() = default;
    reference_vector_storage(reference_vector_storage const&) = delete;
    reference_vector_storage& operator=(reference_vector_storage const&) = delete;
    reference_vector_storage(reference_vector_storage&&) noexcept = default;
    reference_vector_storage& operator=(reference_vector_storage&&) noexcept = default;

    using reference_vector_storage_base<T>::clear;
    using reference_vector_storage_base<T>::elements_;
};

} // namespace impl
/// @endcond

/**
 * @brief a vector of references.
 * @tparam T the element type
 * @tparam Copier the object copying strategy
 */
template<class T, class Copier = std::disjunction<clonable_copier<T>, standard_copier<T>, null_copier<T>>>
class reference_vector {
public:
    /// @brief the value type
    using value_type = T;
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
    using iterator = reference_iterator<double_pointer_extractor<value_type>>;
    /// @brief the const iterator type
    using const_iterator = reference_iterator<double_pointer_extractor<std::add_const_t<value_type>>>;
    /// @brief the reverse iterator type
    using reverse_iterator = std::reverse_iterator<iterator>;
    /// @brief the const reverse iterator type
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    /// @brief the object copier type
    using copier_type = Copier;

    /**
     * @brief constructs a new empty object.
     */
    reference_vector() = default;

    /**
     * @brief constructs a new object.
     * @tparam U the element type
     * @param list the initial elements
     */
    template<
            class U,
            class = std::enable_if_t<std::is_convertible_v<typename std::initializer_list<U>::reference, const_reference>>>
    reference_vector(std::initializer_list<U> list)
    {
        assign(list);
    }

    /**
     * @brief constructs a new object.
     * @tparam Iter the iterator type
     * @param first the starting position (inclusive)
     * @param last the ending position (exclusive)
     */
    template<
            class Iter,
            class = workaround::gcc11::enable_if_t<std::is_convertible_v<typename std::iterator_traits<Iter>::reference, const_reference>>>
    reference_vector(Iter first, Iter last)
    {
        assign(first, last);
    }

    /**
     * @brief constructs a new object.
     * @tparam U the source value type
     * @tparam C the source copier type
     * @param other the source object
     */
    template<
            class U,
            class C,
            class = std::enable_if_t<std::is_convertible_v<typename reference_vector<U, C>::pointer, pointer>>>
    reference_vector(reference_vector<U, C> other) noexcept // NOLINT
    {
        auto&& src = other.storage_.elements_;
        unsafe_assign(src.begin(), src.end());
        src.clear();
    }

    /**
     * @brief constructs a new object.
     * @tparam C the source copier type
     * @param other the source object
     */
    template<class C>
    explicit reference_vector(reference_vector<T, C> const& other)
    {
        reserve(other.size());
        for (auto&& e : other) {
            push_back(e);
        }
    }

    /**
     * @brief constructs a new object.
     * @tparam C the source copier type
     * @param other the source object
     */
    template<class C>
    explicit reference_vector(reference_vector<T, C>&& other)
    {
        storage_.elements_ = std::move(other.storage_.elements_);
        other.storage_.elements_.clear(); // may be already empty
    }

    /**
     * @brief returns an element at the position.
     * @param position the element index on this vector
     * @return the element on the position
     * @throws std::out_of_bound if the position is out of bound
     */
    [[nodiscard]] reference at(size_type position) {
        return *storage_.elements_.at(position);
    }

    /// @copydoc at()
    [[nodiscard]] const_reference at(size_type position) const {
        return *storage_.elements_.at(position);
    }

    /**
     * @brief returns an element at the position.
     * @param position the element index on this vector
     * @return the element on the position
     * @warning undefined behavior if the position is out of bound
     */
    [[nodiscard]] reference operator[](size_type position) {
        return *storage_.elements_[position];
    }

    /// @copydoc operator[]()
    [[nodiscard]] const_reference operator[](size_type position) const {
        return *storage_.elements_[position];
    }

    /**
     * @brief returns reference of the first element
     * @return reference of the first element
     * @warning undefined behavior if this is empty
     */
    [[nodiscard]] reference front() {
        return *storage_.elements_.front();
    }

    /// @copydoc front()
    [[nodiscard]] const_reference front() const {
        return *storage_.elements_.front();
    }

    /**
     * @brief returns reference of the last element
     * @return reference of the last element
     * @warning undefined behavior if this is empty
     */
    [[nodiscard]] reference back() {
        return *storage_.elements_.back();
    }

    /// @copydoc back()
    [[nodiscard]] const_reference back() const {
        return *storage_.elements_.back();
    }

    /**
     * @brief returns whether or not this is empty.
     * @return true if this is empty
     * @return false otherwise
     */
    [[nodiscard]] bool empty() const noexcept {
        return storage_.elements_.empty();
    }

    /**
     * @brief returns the number of elements in this.
     * @return the number of elements
     */
    [[nodiscard]] size_type size() const noexcept {
        return storage_.elements_.size();
    }

    /**
     * @brief returns the capacity size of this.
     * @return the max number of elements to store without expanding this container
     */
    [[nodiscard]] size_type capacity() const noexcept {
        return storage_.elements_.capacity();
    }

    /**
     * @brief reserves the capacity.
     * @param size the number of elements to store without expanding this container
     */
    void reserve(size_type size) {
        storage_.elements_.reserve(size);
    }

    /**
     * @brief may shrink capacity size to fit to the current storing elements.
     */
    void shrink_to_fit() {
        storage_.elements_.shrink_to_fit();
    }

    /**
     * @brief removes all elements in this.
     */
    void clear() noexcept {
        storage_.clear();
    }

    /**
     * @brief assigns copy of elements in the list.
     * @tparam U the element type
     * @param list the source elements
     */
    template<class U>
    std::enable_if_t<std::is_convertible_v<typename std::initializer_list<U>::reference, const_reference>>
    assign(std::initializer_list<U> list) {
        assign(list.begin(), list.end());
    }

    /**
     * @brief assigns copy of elements between the given span.
     * @tparam Iter the iterator type
     * @param first the starting position (inclusive)
     * @param last the ending position (exclusive)
     */
    template<class Iter>
    workaround::gcc11::enable_if_t<std::is_convertible_v<typename std::iterator_traits<Iter>::reference, const_reference>>
    assign(Iter first, Iter last) {
        clear();
        insert(begin(), first, last);
    }

    /**
     * @brief removes the element on the given position.
     * @param position the target element position
     * @return the next position of the erased element
     */
    iterator erase(const_iterator position) noexcept {
        auto iter = to_internal(position);
        delete_element(*iter);
        iter = storage_.elements_.erase(iter);
        return from_internal(iter);
    }

    /**
     * @brief removes the elements between the span.
     * @param first the starting position (inclusive)
     * @param last the ending position (exclusive)
     * @return the next position of the "last"
     */
    iterator erase(const_iterator first, const_iterator last) noexcept {
        auto const fst = to_internal(first);
        auto const lst = to_internal(last);
        for (auto i = fst; i < lst; ++i) {
            delete_element(*i);
            *i = nullptr;
        }
        auto iter = storage_.elements_.erase(fst, lst);
        return from_internal(iter);
    }

    /**
     * @brief removes the last element.
     * @warning undefined behavior if this is empty
     */
    void pop_back() noexcept {
        delete_element(storage_.elements_.back());
        storage_.elements_.pop_back();
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
        auto iter = to_internal(position);
        iter = storage_.elements_.insert(iter, nullptr);
        *iter = forward_element(std::move(element));
        return from_internal(iter);
    }

    /**
     * @brief inserts a copy of the given element.
     * @param position the insertion position
     * @param element the target element
     * @return the position where the element inserted
     */
    iterator insert(const_iterator position, const_reference element) {
        auto iter = to_internal(position);
        iter = storage_.elements_.insert(iter, nullptr);
        *iter = copy_element(element);
        return from_internal(iter);
    }

    /// @copydoc insert(const_iterator, const_reference)
    iterator insert(const_iterator position, rvalue_reference element) {
        auto iter = to_internal(position);
        iter = storage_.elements_.insert(iter, nullptr);
        *iter = copy_element(std::move(element));
        return from_internal(iter);
    }

    /**
     * @brief inserts copy of elements between the given list.
     * @tparam U the element type
     * @param position the insertion position
     * @param list the source elements
     * @return the position where the first element inserted
     */
    template<class U>
    std::enable_if_t<std::is_convertible_v<typename std::initializer_list<U>::reference, const_reference>, iterator>
    insert(const_iterator position, std::initializer_list<U> list) {
        return insert(position, list.begin(), list.end());
    }

    /**
     * @brief inserts copy of elements between the given span.
     * @tparam Iter the iterator type
     * @param position the insertion position
     * @param first the starting position (inclusive)
     * @param last the ending position (exclusive)
     * @return the position where the first element inserted
     */
    template<class Iter>
    workaround::gcc11::enable_if_t<std::is_convertible_v<typename std::iterator_traits<Iter>::reference, const_reference>, iterator>
    insert(const_iterator position, Iter first, Iter last) {
        auto&& es = storage_.elements_;
        auto index = to_internal(position) - es.begin();
        if (auto count = std::distance(first, last); count > 0) {
            es.reserve(es.size() + static_cast<std::size_t>(count));
        }
        decltype(index) count = 0;
        for (auto iter = first; iter != last; ++iter) {
            es.push_back(copy_element(*iter));
            ++count;
        }
        if (count != 0 && static_cast<std::size_t>(index + count) != es.size()) {
            auto from = es.begin() + index;
            auto to = es.end() - count;
            std::rotate(from, to, es.end());
        }
        return begin() + index;
    }

    /**
     * @brief appends the given element into the tail of this.
     * @tparam U the value type
     * @tparam D the deleter type
     * @param element the source element
     * @return the created copy of source
     */
    template<class U, class D>
    std::enable_if_t<std::is_convertible_v<U&, reference>, reference>
    push_back(std::unique_ptr<U, D> element) {
        auto&& s = storage_.elements_;
        s.push_back(nullptr);
        auto* result = forward_element(std::move(element));
        s.back() = result;
        return *result;
    }

    /**
     * @brief appends a copy of element into the tail of this.
     * @param element the source element
     * @return the created copy of source
     */
    reference push_back(const_reference element) {
        auto&& s = storage_.elements_;
        s.push_back(nullptr);
        auto* result = copy_element(element);
        s.back() = result;
        return *result;
    }

    /// @copydoc push_back(const_reference)
    reference push_back(rvalue_reference element) {
        auto&& s = storage_.elements_;
        s.push_back(nullptr);
        auto* result = copy_element(std::move(element));
        s.back() = result;
        return *result;
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
        auto iter = to_internal(position);
        iter = storage_.elements_.emplace(iter, nullptr);
        *iter = create_element<U>(std::forward<Args>(args)...);
        return from_internal(iter);
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
        storage_.elements_.push_back(nullptr);
        auto* result = create_element<U>(std::forward<Args>(args)...);
        storage_.elements_.back() = result;
        return *result;
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
        auto iter = to_internal(position);
        auto* replacement = forward_element(std::move(element));
        delete_element(*iter);
        *iter = replacement;
        return *replacement;
    }

    /**
     * @brief replaces an existing element with a copy of the given element.
     * @param position the target element position
     * @param element the source element
     * @return the replacement
     */
    reference put(const_iterator position, const_reference element) {
        auto iter = to_internal(position);
        auto* replacement = copy_element(element);
        delete_element(*iter);
        *iter = replacement;
        return *replacement;
    }

    /// @copydoc put(const_iterator, const_reference)
    reference put(const_iterator position, rvalue_reference element) {
        auto iter = to_internal(position);
        auto* replacement = copy_element(std::move(element));
        delete_element(*iter);
        *iter = replacement;
        return *replacement;
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
    [[nodiscard]] reference replace(const_iterator position, Args&&... args) {
        auto iter = to_internal(position);
        auto* replacement = create_element<U>(std::forward<Args>(args)...);
        delete_element(*iter);
        *iter = replacement;
        return *replacement;
    }

    /**
     * @brief releases element on the given position.
     * @param position the target position
     * @return a pair of the removed element, and the next position of the released element
     */
    [[nodiscard]] std::pair<std::unique_ptr<value_type>, iterator> release(const_iterator position) noexcept {
        auto iter = to_internal(position);
        auto released = wrap_unique(*iter);
        iter = storage_.elements_.erase(iter);
        return std::make_pair(std::move(released), from_internal(iter));
    }

    /**
     * @brief releases element between the given range.
     * @tparam C the copier type
     * @param first the first position (inclusive)
     * @param last the last position (exclusive)
     * @return a pair of the removed element, and the next position of the released element
     */
    template<class C = copier_type>
    [[nodiscard]] std::pair<reference_vector<value_type, C>, iterator> release(const_iterator first, const_iterator last) noexcept {
        auto const fst = to_internal(first);
        auto const lst = to_internal(last);

        reference_vector<value_type, C> result {};
        if (auto distance = std::distance(fst, lst); distance > 0) {
            result.reserve(static_cast<std::size_t>(distance));
        }
        for (auto i = fst; i < lst; ++i) {
            result.unsafe_push_back(*i);
            *i = nullptr;
        }
        auto iter = storage_.elements_.erase(fst, lst);
        return std::make_pair(std::move(result), from_internal(iter));
    }

    /**
     * @brief releases the last element.
     * @return the removed element
     * @warning undefined behavior if this is empty
     */
    [[nodiscard]] std::unique_ptr<value_type> release_back() noexcept {
        auto released = wrap_unique(storage_.elements_.back());
        storage_.elements_.pop_back();
        return released;
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
    exchange(const_iterator position, std::unique_ptr<U, D> element) {
        auto iter = to_internal(position);
        auto* rep = forward_element(std::move(element));
        auto released = wrap_unique(*iter);
        *iter = rep;
        return released;
    }

    /**
     * @brief returns ownership reference of the element on the given position.
     * @details If the entry was modified via the returned ownership, it behaves like as if
     *      put(const_iterator, std::unique_ptr<U, D>) was called.
     * @attention the ownership reference will be invalidated after some elements are added or removed.
     * @param position the target position
     * @return the element on the given position
     */
    [[nodiscard]] ownership_reference<value_type> ownership(const_iterator position) {
        using ownership_ref = ownership_reference<value_type>;
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
        return iterator(storage_.elements_.data());
    }

    /// @copydoc begin()
    [[nodiscard]] const_iterator begin() const noexcept {
        return cbegin();
    }

    /// @copydoc begin()
    [[nodiscard]] const_iterator cbegin() const noexcept {
        return const_iterator(storage_.elements_.data());
    }

    /**
     * @brief returns a forward iterator which points the ending of this container.
     * @return the iterator of ending (exclusive)
     */
    [[nodiscard]] iterator end() noexcept {
        return begin() + storage_.elements_.size();
    }

    /// @copydoc end()
    [[nodiscard]] const_iterator end() const noexcept {
        return cend();
    }

    /// @copydoc end()
    [[nodiscard]] const_iterator cend() const noexcept {
        return cbegin() + storage_.elements_.size();
    }

    /**
     * @brief returns a backward iterator which points the reversed beginning of this container.
     * @return the reversed iterator of beginning (inclusive)
     */
    [[nodiscard]] reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }

    /// @copydoc rbegin()
    [[nodiscard]] const_reverse_iterator rbegin() const noexcept {
        return crbegin();
    }

    /// @copydoc rbegin()
    [[nodiscard]] const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(cend());
    }

    /**
     * @brief returns a backward iterator which points the reversed ending of this container.
     * @return the reversed iterator of ending (exclusive)
     */
    [[nodiscard]] reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }

    /// @copydoc rend()
    [[nodiscard]] const_reverse_iterator rend() const noexcept {
        return crend();
    }

    /// @copydoc rend()
    [[nodiscard]] const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator(cbegin());
    }

    /**
     * @brief exchanges contents between this and the given container.
     * @param other the opposite container
     */
    void swap(reference_vector& other) noexcept {
        std::swap(storage_, other.storage_);
    }

    /**
     * @brief assigns externally allocated elements in the list.
     * @attention this operation never takes copies of inputs.
     * @tparam U the element type
     * @param list the source elements
     */
    template<class U>
    std::enable_if_t<std::is_convertible_v<typename std::initializer_list<U*>::reference, pointer>>
    unsafe_assign(std::initializer_list<U*> list) {
        unsafe_assign(list.begin(), list.end());
    }

    /**
     * @brief assigns externally allocated elements between the given span.
     * @attention this operation never takes copies of inputs.
     * @tparam Iter the iterator type
     * @param first the starting position (inclusive)
     * @param last the ending position (exclusive)
     */
    template<class Iter>
    std::enable_if_t<std::is_convertible_v<typename std::iterator_traits<Iter>::reference, pointer>>
    unsafe_assign(Iter first, Iter last) {
        clear();
        unsafe_insert(begin(), first, last);
    }

    /**
     * @brief inserts an externally allocated element.
     * @attention this operation never takes copies of inputs.
     * @param position the insertion position
     * @param element the target element
     * @return the position where the element inserted
     */
    iterator unsafe_insert(const_iterator position, pointer element) {
        auto iter = to_internal(position);
        iter = storage_.elements_.insert(iter, element);
        return from_internal(iter);
    }

    /**
     * @brief inserts externally allocated elements in the list.
     * @attention this operation never takes copies of inputs.
     * @tparam U the element type
     * @param position the insertion position
     * @param list the source elements
     * @return the position where the first element inserted
     */
    template<class U>
    std::enable_if_t<std::is_convertible_v<typename std::initializer_list<U*>::reference, pointer>, iterator>
    unsafe_insert(const_iterator position, std::initializer_list<U*> list) {
        return unsafe_insert(position, list.begin(), list.end());
    }

    /**
     * @brief inserts externally allocated elements between the given span.
     * @attention this operation never takes copies of inputs.
     * @tparam Iter the iterator type
     * @param position the insertion position
     * @param first the starting position (inclusive)
     * @param last the ending position (exclusive)
     * @return the starting position of the head of elements inserted
     */
    template<class Iter>
    std::enable_if_t<std::is_convertible_v<typename std::iterator_traits<Iter>::reference, pointer>, iterator>
    unsafe_insert(const_iterator position, Iter first, Iter last) {
        auto&& es = storage_.elements_;
        auto index = to_internal(position) - es.begin();
        if (auto count = std::distance(first, last); count > 0) {
            es.reserve(es.size() + static_cast<std::size_t>(count));
        }
        decltype(index) count = 0;
        for (auto iter = first; iter != last; ++iter) {
            pointer element = *iter;
            es.push_back(element);
            ++count;
        }
        if (count != 0 && static_cast<std::size_t>(index + count) != es.size()) {
            auto from = es.begin() + index;
            auto to = es.end() - count;
            std::rotate(from, to, es.end());
        }
        return begin() + index;
    }

    /**
     * @brief appends an externally allocated of element into the tail of this.
     * @attention this operation never takes copies of an input.
     * @param element the source element
     * @return the added element
     */
    reference unsafe_push_back(pointer element) {
        storage_.elements_.push_back(element);
        return *element;
    }

    /**
     * @brief replaces an existing element with an externally allocated element.
     * @param position the target element position
     * @param element the source element
     * @return the replacement
     */
    reference unsafe_put(const_iterator position, pointer element) {
        auto iter = to_internal(position);
        delete_element(*iter);
        *iter = element;
        return *element;
    }

private:
    impl::reference_vector_storage<T, copier_type> storage_ {};

    using mutable_pointer = std::add_pointer_t<std::remove_const_t<value_type>>;

    typename decltype(storage_.elements_)::iterator to_internal(const_iterator iter) noexcept {
        auto offset = iter - cbegin();
        return storage_.elements_.begin() + offset;
    }

    iterator from_internal(typename decltype(storage_.elements_)::const_iterator iter) noexcept {
        auto offset = iter - storage_.elements_.cbegin();
        return begin() + offset;
    }

    [[nodiscard]] const_iterator from_internal(typename decltype(storage_.elements_)::const_iterator iter) const noexcept {
        auto offset = iter - storage_.elements_.cbegin();
        return begin() + offset;
    }

    [[nodiscard]] pointer copy_element(const_reference element) {
        static_assert(copier_type::is_available, "copying objects is not supported");
        auto copy = copier_type::copy(element);
        return copy.release();
    }

    [[nodiscard]] pointer copy_element(rvalue_reference element) {
        static_assert(copier_type::is_available, "copying objects is not supported");
        auto copy = copier_type::copy(std::move(element));
        return copy.release();
    }

    template<class U, class D>
    [[nodiscard]] pointer forward_element(std::unique_ptr<U, D>&& element) {
        if (!element) {
            throw_exception(std::invalid_argument("element must not be null"));
        }
        return element.release();
    }

    template<class U, class... Args>
    [[nodiscard]] pointer create_element(Args&&... args) {
        return new U(std::forward<Args>(args)...); // NOLINT
    }

    void delete_element(pointer object) noexcept {
        if (object != nullptr) {
            delete object; // NOLINT
        }
    }

    std::unique_ptr<value_type> wrap_unique(pointer ptr) noexcept {
        return std::unique_ptr<value_type> { ptr };
    }

    template<class U, class C>
    friend class reference_vector;
};

/// @private
template<class T>
reference_vector(std::initializer_list<T>) -> reference_vector<T>;

/**
 * @brief returns whether or not the both vectors have equivalent elements.
 * @tparam T1 element type of the first vector
 * @tparam C1 copier type of the first vector
 * @tparam T2 element type of the second vector
 * @tparam C2 copier type of the first vector
 * @param a the first vector
 * @param b the second vector
 * @return true if a = b
 * @return false other wise
 */
template<class T1, class C1, class T2, class C2>
inline bool operator==(reference_vector<T1, C1> const& a, reference_vector<T2, C2> const& b) noexcept {
    if (a.size() != b.size()) return false;
    for (std::size_t i = 0, n = a.size(); i < n; ++i) {
        if (!(a[i] == b[i])) return false;
    }
    return true;
}

/**
 * @brief returns whether or not the both vectors have different elements.
 * @tparam T1 element type of the first vector
 * @tparam C1 copier type of the first vector
 * @tparam T2 element type of the second vector
 * @tparam C2 copier type of the first vector
 * @param a the first vector
 * @param b the second vector
 * @return true if a != b
 * @return false other wise
 */
template<class T1, class C1, class T2, class C2>
inline bool operator!=(reference_vector<T1, C1> const& a, reference_vector<T2, C2> const& b) noexcept {
    return !(a == b);
}

template<class T1, class C1, class T2, class C2>
inline int compare(reference_vector<T1, C1> const& a, reference_vector<T2, C2> const& b) noexcept;

/**
 * @brief returns whether or not the first vector is less than the second one, in lexicographic order.
 * @tparam T1 element type of the first vector
 * @tparam C1 copier type of the first vector
 * @tparam T2 element type of the second vector
 * @tparam C2 copier type of the first vector
 * @param a the first vector
 * @param b the second vector
 * @return true if a < b
 * @return false other wise
 */
template<class T1, class C1, class T2, class C2>
inline bool operator<(reference_vector<T1, C1> const& a, reference_vector<T2, C2> const& b) noexcept {
    return compare(a, b) < 0;
}

/**
 * @brief returns whether or not the first vector is greater than the second one, in lexicographic order.
 * @tparam T1 element type of the first vector
 * @tparam C1 copier type of the first vector
 * @tparam T2 element type of the second vector
 * @tparam C2 copier type of the first vector
 * @param a the first vector
 * @param b the second vector
 * @return true if a > b
 * @return false other wise
 */
template<class T1, class C1, class T2, class C2>
inline bool operator>(reference_vector<T1, C1> const& a, reference_vector<T2, C2> const& b) noexcept {
    return compare(a, b) > 0;
}

/**
 * @brief returns whether or not the first vector is less than or equal to the second one, in lexicographic order.
 * @tparam T1 element type of the first vector
 * @tparam C1 copier type of the first vector
 * @tparam T2 element type of the second vector
 * @tparam C2 copier type of the first vector
 * @param a the first vector
 * @param b the second vector
 * @return true if a <= b
 * @return false other wise
 */
template<class T1, class C1, class T2, class C2>
inline bool operator<=(reference_vector<T1, C1> const& a, reference_vector<T2, C2> const& b) noexcept {
    return compare(a, b) <= 0;
}

/**
 * @brief returns whether or not the first vector is greater than or equal to the second one, in lexicographic order.
 * @tparam T1 element type of the first vector
 * @tparam C1 copier type of the first vector
 * @tparam T2 element type of the second vector
 * @tparam C2 copier type of the first vector
 * @param a the first vector
 * @param b the second vector
 * @return true if a >= b
 * @return false other wise
 */
template<class T1, class C1, class T2, class C2>
inline bool operator>=(reference_vector<T1, C1> const& a, reference_vector<T2, C2> const& b) noexcept {
    return compare(a, b) >= 0;
}

/**
 * @brief compares elements in the given vectors.
 * @tparam T1 element type of the first vector
 * @tparam C1 copier type of the first vector
 * @tparam T2 element type of the second vector
 * @tparam C2 copier type of the first vector
 * @param a the first vector
 * @param b the second vector
 * @return < 0 if a < b
 * @return = 0 if a = b
 * @return > 0 if a > b
 */
template<class T1, class C1, class T2, class C2>
inline int compare(reference_vector<T1, C1> const& a, reference_vector<T2, C2> const& b) noexcept {
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
 * @tparam C the copier type
 * @param out the target output
 * @param value the target value
 * @return the output
 */
template<class T, class C>
inline std::ostream& operator<<(std::ostream& out, reference_vector<T, C> const& value) {
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

} // namespace takatori::util
