#pragma once

#include <algorithm>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

#include "clonable.h"
#include "copier.h"
#include "object_creator.h"
#include "reference_iterator.h"

namespace takatori::util {

/// @private
namespace impl {

/// @private
template<class T, class A>
inline void delete_vector(object_creator creator, std::vector<T*, A>& elements) {
    while (!elements.empty()) {
        auto* element = elements.back();
        // broken class may have nullptr
        if (element != nullptr) {
            creator.delete_object<T>(element);
        }
        elements.pop_back();
    }
}

/// @private
template<class T>
using reference_vector_entity = std::vector<T*, object_allocator<T*>>;

/// @private
template<class T>
struct reference_vector_storage_base {
    explicit reference_vector_storage_base(object_creator creator = {}) noexcept
        : creator_(creator), elements_(creator_.allocator(std::in_place_type<T*>)) {}
    ~reference_vector_storage_base() { clear(); } // NOLINT
    reference_vector_storage_base(reference_vector_storage_base const&) = delete;
    reference_vector_storage_base& operator=(reference_vector_storage_base const&) = delete;
    reference_vector_storage_base(reference_vector_storage_base&& other) noexcept
        : creator_(other.creator_), elements_(std::move(other.elements_)) {}
    reference_vector_storage_base& operator=(reference_vector_storage_base&& other) noexcept {
        clear();
        creator_ = other.creator_;
        elements_ = reference_vector_entity<T>(std::move(other.elements_));
        return *this;
    }

    void clear() noexcept { delete_vector(creator_, elements_); }

    object_creator creator_; // NOLINT
    reference_vector_entity<T> elements_; // NOLINT
};

/// @private
template<class T, class C, bool = C::is_available>
struct reference_vector_storage;

/// @private
template<class T, class C>
struct reference_vector_storage<T, C, true> : reference_vector_storage_base<T> {
    using reference_vector_storage_base<T>::reference_vector_storage_base;

    ~reference_vector_storage() = default;

    reference_vector_storage(reference_vector_storage const& other) : reference_vector_storage_base<T>(other.creator_) {
        elements_ = other.copy_elements();
    }

    reference_vector_storage& operator=(reference_vector_storage const& other) {
        auto elements = other.copy_elements();
        clear();
        creator_ = other.creator_;
        elements_ = std::move(elements);
        return *this;
    }

    reference_vector_storage(reference_vector_storage&&) noexcept = default;
    reference_vector_storage& operator=(reference_vector_storage&&) noexcept = default;

    using reference_vector_storage_base<T>::clear;
    using reference_vector_storage_base<T>::creator_;
    using reference_vector_storage_base<T>::elements_;

    reference_vector_entity<T> copy_elements() const {
        reference_vector_entity<T> result { creator_.template allocator<T*>() };
        result.reserve(elements_.size());
        try {
            for (auto* element : elements_) {
                result.push_back(C::copy(creator_, *element));
            }
            return result;
        } catch (...) {
            delete_vector(creator_, result);
            std::rethrow_exception(std::current_exception());
        }
    }
};

/// @private
template<class T, class C>
struct reference_vector_storage<T, C, false> : reference_vector_storage_base<T> {
    using reference_vector_storage_base<T>::reference_vector_storage_base;

    ~reference_vector_storage() = default;
    reference_vector_storage(reference_vector_storage const&) = delete;
    reference_vector_storage& operator=(reference_vector_storage const&) = delete;
    reference_vector_storage(reference_vector_storage&&) noexcept = default;
    reference_vector_storage& operator=(reference_vector_storage&&) noexcept = default;

    using reference_vector_storage_base<T>::clear;
    using reference_vector_storage_base<T>::creator_;
    using reference_vector_storage_base<T>::elements_;
};
} // namespace impl

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
     * @brief constructs a new object with default object creator.
     */
    reference_vector() = default;

    /**
     * @brief constructs a new object.
     * @param creator the object creator
     */
    explicit reference_vector(object_creator creator) noexcept : storage_(creator) {}

    /**
     * @brief constructs a new object.
     * @tparam U the element type
     * @param list the initial elements
     * @param creator the object creator
     */
    template<
            class U,
            class = std::enable_if_t<std::is_convertible_v<typename std::initializer_list<U>::reference, const_reference>>>
    reference_vector(std::initializer_list<U> list, object_creator creator = {});

    /**
     * @brief constructs a new object.
     * @tparam Iter the iterator type
     * @param first the starting position (inclusive)
     * @param last the ending position (exclusive)
     * @param creator the object creator
     */
    template<
            class Iter,
            class = std::enable_if_t<std::is_convertible_v<typename std::iterator_traits<Iter>::reference, const_reference>>>
    reference_vector(Iter first, Iter last, object_creator creator = {});

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
    reference_vector(reference_vector<U, C> other) noexcept; // NOLINT

    /**
     * @brief constructs a new object.
     * @tparam C the source copier type
     * @param other the source object
     */
    template<class C>
    explicit reference_vector(reference_vector<T, C> const& other, object_creator creator);

    /**
     * @brief constructs a new object.
     * @tparam C the source copier type
     * @param other the source object
     */
    template<class C>
    explicit reference_vector(reference_vector<T, C>&& other, object_creator creator);

    /**
     * @brief returns an element at the position.
     * @param position the element index on this vector
     * @return the element on the position
     * @throws std::out_of_bound if the position is out of bound
     */
    reference at(size_type position) { return *storage_.elements_.at(position); }

    /// @copydoc at()
    const_reference at(size_type position) const { return *storage_.elements_.at(position); }

    /**
     * @brief returns an element at the position.
     * @param position the element index on this vector
     * @return the element on the position
     * @warning undefined behavior if the position is out of bound
     */
    reference operator[](size_type position) { return *storage_.elements_[position]; }

    /// @copydoc operator[]()
    const_reference operator[](size_type position) const { return *storage_.elements_[position]; }

    /**
     * @brief returns reference of the first element
     * @return reference of the first element
     * @warning undefined behavior if this is empty
     */
    reference front() { return *storage_.elements_.front(); }

    /// @copydoc front()
    const_reference front() const { return *storage_.elements_.front(); }

    /**
     * @brief returns reference of the last element
     * @return reference of the last element
     * @warning undefined behavior if this is empty
     */
    reference back() { return *storage_.elements_.back(); }

    /// @copydoc back()
    const_reference back() const { return *storage_.elements_.back(); }

    /**
     * @brief returns whether or not this is empty.
     * @return true if this is empty
     * @return false otherwise
     */
    bool empty() const noexcept { return storage_.elements_.empty(); }

    /**
     * @brief returns the number of elements in this.
     * @return the number of elements
     */
    size_type size() const noexcept { return storage_.elements_.size(); }

    /**
     * @brief returns the capacity size of this.
     * @return the max number of elements to store without expanding this container
     */
    size_type capacity() const noexcept { return storage_.elements_.capacity(); }

    /**
     * @brief reserves the capacity.
     * @param size the number of elements to store without expanding this container
     */
    void reserve(size_type size) { storage_.elements_.reserve(size); }

    /**
     * @brief may shrink capacity size to fit to the current storing elements.
     */
    void shrink_to_fit() { storage_.elements_.shrink_to_fit(); }

    /**
     * @brief removes all elements in this.
     */
    void clear() noexcept;

    /**
     * @brief assigns copy of elements in the list.
     * @tparam U the element type
     * @param list the source elements
     */
    template<class U>
    std::enable_if_t<std::is_convertible_v<typename std::initializer_list<U>::reference, const_reference>>
    assign(std::initializer_list<U> list);

    /**
     * @brief assigns copy of elements between the given span.
     * @tparam Iter the iterator type
     * @param first the starting position (inclusive)
     * @param last the ending position (exclusive)
     */
    template<class Iter>
    std::enable_if_t<std::is_convertible_v<typename std::iterator_traits<Iter>::reference, const_reference>>
    assign(Iter first, Iter last);

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
     * @attention This may create a copy if the element is not created by this container's object creator.
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
     * @brief inserts a copy of the given element.
     * @param position the insertion position
     * @param element the target element
     * @return the position where the element inserted
     */
    iterator insert(const_iterator position, const_reference element);

    /// @copydoc insert(const_iterator, const_reference)
    iterator insert(const_iterator position, rvalue_reference element);

    /**
     * @brief inserts copy of elements between the given list.
     * @tparam U the element type
     * @param position the insertion position
     * @param list the source elements
     * @return the position where the first element inserted
     */
    template<class U>
    std::enable_if_t<std::is_convertible_v<typename std::initializer_list<U>::reference, const_reference>, iterator>
    insert(const_iterator position, std::initializer_list<U> list);

    /**
     * @brief inserts copy of elements between the given span.
     * @tparam Iter the iterator type
     * @param position the insertion position
     * @param first the starting position (inclusive)
     * @param last the ending position (exclusive)
     * @return the position where the first element inserted
     */
    template<class Iter>
    std::enable_if_t<std::is_convertible_v<typename std::iterator_traits<Iter>::reference, const_reference>, iterator>
    insert(const_iterator position, Iter first, Iter last);

    /**
     * @brief appends the given element into the tail of this.
     * @attention This may create a copy if the element is not created by this container's object creator.
     * @tparam U the value type
     * @tparam D the deleter type
     * @param element the source element
     * @return the created copy of source
     */
    template<class U, class D>
    std::enable_if_t<std::is_convertible_v<U&, reference>, reference>
    push_back(std::unique_ptr<U, D> element);

    /**
     * @brief appends a copy of element into the tail of this.
     * @param element the source element
     * @return the created copy of source
     */
    reference push_back(const_reference element);

    /// @copydoc push_back(const_reference)
    reference push_back(rvalue_reference element);

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
     * @attention This may create a copy if the element is not created by this container's object creator.
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
     * @brief replaces an existing element with a copy of the given element.
     * @param position the target element position
     * @param element the source element
     * @return the replacement
     */
    reference put(const_iterator position, const_reference element);

    /// @copydoc put(const_iterator, const_reference)
    reference put(const_iterator position, rvalue_reference element);

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
    std::pair<unique_object_ptr<value_type>, iterator> release(const_iterator position) noexcept;

    /**
     * @brief releases element between the given range.
     * @tparam C the copier type
     * @param first the first position (inclusive)
     * @param last the last position (exclusive)
     * @return a pair of the removed element, and the next position of the released element
     */
    template<class C = copier_type>
    std::pair<reference_vector<value_type, C>, iterator> release(const_iterator first, const_iterator last) noexcept;

    /**
     * @brief releases the last element.
     * @return the removed element
     * @warning undefined behavior if this is empty
     */
    unique_object_ptr<value_type> release_back() noexcept;

    /**
     * @brief returns a forward iterator which points the beginning of this container.
     * @return the iterator of beginning (inclusive)
     */
    iterator begin() noexcept;

    /// @copydoc begin()
    const_iterator begin() const noexcept;

    /// @copydoc begin()
    const_iterator cbegin() const noexcept;

    /**
     * @brief returns a forward iterator which points the ending of this container.
     * @return the iterator of ending (exclusive)
     */
    iterator end() noexcept;

    /// @copydoc end()
    const_iterator end() const noexcept;

    /// @copydoc end()
    const_iterator cend() const noexcept;

    /**
     * @brief returns a backward iterator which points the reversed beginning of this container.
     * @return the reversed iterator of beginning (inclusive)
     */
    reverse_iterator rbegin() noexcept;

    /// @copydoc rbegin()
    const_reverse_iterator rbegin() const noexcept;

    /// @copydoc rbegin()
    const_reverse_iterator crbegin() const noexcept;

    /**
     * @brief returns a backward iterator which points the reversed ending of this container.
     * @return the reversed iterator of ending (exclusive)
     */
    reverse_iterator rend() noexcept;

    /// @copydoc rend()
    const_reverse_iterator rend() const noexcept;

    /// @copydoc rend()
    const_reverse_iterator crend() const noexcept;

    /**
     * @brief exchanges contents between this and the given container.
     * @attention This also exchanges their object_creator, but object copying strategy does not.
     * @param other the opposite container
     */
    void swap(reference_vector& other) noexcept { std::swap(storage_, other.storage_); }

    /**
     * @brief assigns externally allocated elements in the list.
     * @attention this operation never takes copies of inputs, that is, the input elements must be created by
     * an object_creator compatible with this container's one.
     * @tparam U the element type
     * @param list the source elements
     */
    template<class U>
    std::enable_if_t<std::is_convertible_v<typename std::initializer_list<U*>::reference, pointer>>
    unsafe_assign(std::initializer_list<U*> list);

    /**
     * @brief assigns externally allocated elements between the given span.
     * @attention this operation never takes copies of inputs, that is, the input elements must be created by
     * an object_creator compatible with this container's one.
     * @tparam Iter the iterator type
     * @param first the starting position (inclusive)
     * @param last the ending position (exclusive)
     */
    template<class Iter>
    std::enable_if_t<std::is_convertible_v<typename std::iterator_traits<Iter>::reference, pointer>>
    unsafe_assign(Iter first, Iter last);

    /**
     * @brief inserts an externally allocated element.
     * @attention this operation never takes copies of inputs, that is, the input elements must be created by
     * an object_creator compatible with this container's one.
     * @param position the insertion position
     * @param element the target element
     * @return the position where the element inserted
     */
    iterator unsafe_insert(const_iterator position, pointer element);

    /**
     * @brief inserts externally allocated elements in the list.
     * @attention this operation never takes copies of inputs, that is, the input elements must be created by
     * an object_creator compatible with this container's one.
     * @tparam U the element type
     * @param position the insertion position
     * @param list the source elements
     * @return the position where the first element inserted
     */
    template<class U>
    std::enable_if_t<std::is_convertible_v<typename std::initializer_list<U*>::reference, pointer>, iterator>
    unsafe_insert(const_iterator position, std::initializer_list<U*> list);

    /**
     * @brief inserts externally allocated elements between the given span.
     * @attention this operation never takes copies of inputs, that is, the input elements must be created by
     * an object_creator compatible with this container's one.
     * @tparam Iter the iterator type
     * @param position the insertion position
     * @param first the starting position (inclusive)
     * @param last the ending position (exclusive)
     * @return the starting position of the head of elements inserted
     */
    template<class Iter>
    std::enable_if_t<std::is_convertible_v<typename std::iterator_traits<Iter>::reference, pointer>, iterator>
    unsafe_insert(const_iterator position, Iter first, Iter last);

    /**
     * @brief appends an externally allocated of element into the tail of this.
     * @attention this operation never takes copies of an input, that is, the input elements must be created by
     * an object_creator compatible with this container's one.
     * @param element the source element
     * @return the added element
     */
    reference unsafe_push_back(pointer element);

    /**
     * @brief replaces an existing element with an externally allocated element.
     * @param position the target element position
     * @param element the source element
     * @return the replacement
     */
    reference unsafe_put(const_iterator position, pointer element);

    /**
     * @brief returns the object creator to create/delete objects in this container.
     * @return the object creator for this container
     */
    object_creator get_object_creator() const noexcept { return storage_.creator_; }

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

    const_iterator from_internal(typename decltype(storage_.elements_)::const_iterator iter) const noexcept {
        auto offset = iter - storage_.elements_.cbegin();
        return begin() + offset;
    }

    pointer copy_element(const_reference element) {
        static_assert(copier_type::is_available, "copying objects is not supported");
        return copier_type::copy(storage_.creator_, element);
    }

    pointer copy_element(rvalue_reference element) {
        static_assert(copier_type::is_available, "copying objects is not supported");
        return copier_type::copy(storage_.creator_, std::move(element));
    }

    template<class U, class D>
    pointer forward_element(std::unique_ptr<U, D>&& element) {
        if (!element) {
            throw std::invalid_argument("element must not be null");
        }
        if (storage_.creator_.is_instance(element)) {
            return element.release();
        }
        if constexpr (!copier_type::is_available) {
            throw std::invalid_argument("copying objects is not supported");
        }
        return copier_type::copy(storage_.creator_, std::move(*element));
    }

    template<class U, class... Args>
    pointer create_element(Args&&... args) {
        return storage_.creator_.template create_object<U>(std::forward<Args>(args)...);
    }

    void delete_element(pointer object) noexcept {
        if (object != nullptr) {
            storage_.creator_.template delete_object<T>(object);
        }
    }

    unique_object_ptr<value_type> wrap_unique(pointer ptr) noexcept {
        return unique_object_ptr<value_type>(ptr, object_deleter(storage_.creator_));
    }

    template<class U, class C>
    friend class reference_vector;
};

/// @private
template<class T>
reference_vector(std::initializer_list<T>) -> reference_vector<T>;

/// @private
template<class T>
reference_vector(object_creator, std::initializer_list<T>) -> reference_vector<T>;

/// @cond TEMPLATE_DEFS

template<class T, class C>
template<class U, class>
reference_vector<T, C>::reference_vector(std::initializer_list<U> list, object_creator creator)
    : storage_(creator)
{
    assign(list);
}

template<class T, class C>
template<class Iter, class>
inline reference_vector<T, C>::reference_vector(Iter first, Iter last, object_creator creator)
    : storage_(creator)
{
    assign(first, last);
}

template<class T, class C>
template<class U, class D, class>
inline reference_vector<T, C>::reference_vector(reference_vector<U, D> other) noexcept
    : storage_(other.storage_.creator_)
{
    auto&& src = other.storage_.elements_;
    unsafe_assign(src.begin(), src.end());
    src.clear();
}

template<class T, class Copier>
template<class C>
reference_vector<T, Copier>::reference_vector(reference_vector<T, C> const& other, object_creator creator)
    : storage_(creator)
{
    reserve(other.size());
    for (auto&& e : other) {
        push_back(e);
    }
}

template<class T, class Copier>
template<class C>
reference_vector<T, Copier>::reference_vector(reference_vector<T, C>&& other, object_creator creator)
    : storage_(creator)
{
    if (other.get_object_creator() == creator) {
        storage_.elements_ = std::move(other.storage_.elements_);
        other.storage_.elements_.clear(); // may be already empty
    } else {
        reserve(other.size());
        for (auto&& e : other) {
            push_back(std::move(e));
        }
        other.clear();
    }
}

template<class T, class C>
inline void
reference_vector<T, C>::clear() noexcept {
    storage_.clear();
}

template<class T, class C>
template<class U>
inline std::enable_if_t<std::is_convertible_v<typename std::initializer_list<U>::reference, typename reference_vector<T, C>::const_reference>>
reference_vector<T, C>::assign(std::initializer_list<U> list) {
    assign(list.begin(), list.end());
}

template<class T, class C>
template<class Iter>
inline std::enable_if_t<std::is_convertible_v<typename std::iterator_traits<Iter>::reference, typename reference_vector<T, C>::const_reference>>
reference_vector<T, C>::assign(Iter first, Iter last) {
    clear();
    insert(begin(), first, last);
}

template<class T, class C>
inline typename reference_vector<T, C>::iterator
reference_vector<T, C>::erase(const_iterator position) noexcept {
    auto iter = to_internal(position);
    delete_element(*iter);
    iter = storage_.elements_.erase(iter);
    return from_internal(iter);
}

template<class T, class C>
inline typename reference_vector<T, C>::iterator
reference_vector<T, C>::erase(const_iterator first, const_iterator last) noexcept {
    auto const fst = to_internal(first);
    auto const lst = to_internal(last);
    for (auto i = fst; i < lst; ++i) {
        delete_element(*i);
        *i = nullptr;
    }
    auto iter = storage_.elements_.erase(fst, lst);
    return from_internal(iter);
}

template<class T, class C>
inline void reference_vector<T, C>::pop_back() noexcept {
    delete_element(storage_.elements_.back());
    storage_.elements_.pop_back();
}

template<class T, class C>
template<class U, class D>
inline std::enable_if_t<
        std::is_convertible_v<U&, typename reference_vector<T, C>::reference>,
        typename reference_vector<T, C>::iterator>
reference_vector<T, C>::insert(const_iterator position, std::unique_ptr<U, D> element) {
    auto iter = to_internal(position);
    iter = storage_.elements_.insert(iter, nullptr);
    *iter = forward_element(std::move(element));
    return from_internal(iter);
}

template<class T, class C>
inline typename reference_vector<T, C>::iterator
reference_vector<T, C>::insert(const_iterator position, const_reference element) {
    auto iter = to_internal(position);
    iter = storage_.elements_.insert(iter, nullptr);
    *iter = copy_element(element);
    return from_internal(iter);
}

template<class T, class C>
inline typename reference_vector<T, C>::iterator
reference_vector<T, C>::insert(const_iterator position, rvalue_reference element) {
    auto iter = to_internal(position);
    iter = storage_.elements_.insert(iter, nullptr);
    *iter = copy_element(std::move(element));
    return from_internal(iter);
}

template<class T, class C>
template<class U>
inline std::enable_if_t<
        std::is_convertible_v<typename std::initializer_list<U>::reference, typename reference_vector<T, C>::const_reference>,
        typename reference_vector<T, C>::iterator>
reference_vector<T, C>::insert(const_iterator position, std::initializer_list<U> list) {
    return insert(position, list.begin(), list.end());
}

template<class T, class C>
template<class Iter>
inline std::enable_if_t<
        std::is_convertible_v<typename std::iterator_traits<Iter>::reference, typename reference_vector<T, C>::const_reference>,
        typename reference_vector<T, C>::iterator>
reference_vector<T, C>::insert(const_iterator position, Iter first, Iter last) {
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

template<class T, class C>
template<class U, class D>
inline std::enable_if_t<
        std::is_convertible_v<U&, typename reference_vector<T, C>::reference>,
        typename reference_vector<T, C>::reference>
reference_vector<T, C>::push_back(std::unique_ptr<U, D> element) {
    auto&& s = storage_.elements_;
    s.push_back(nullptr);
    auto* result = forward_element(std::move(element));
    s.back() = result;
    return *result;
}

template<class T, class C>
inline typename reference_vector<T, C>::reference
reference_vector<T, C>::push_back(const_reference element) {
    auto&& s = storage_.elements_;
    s.push_back(nullptr);
    auto* result = copy_element(element);
    s.back() = result;
    return *result;
}

template<class T, class C>
inline typename reference_vector<T, C>::reference
reference_vector<T, C>::push_back(rvalue_reference element) {
    auto&& s = storage_.elements_;
    s.push_back(nullptr);
    auto* result = copy_element(std::move(element));
    s.back() = result;
    return *result;
}

template<class T, class C>
template<class U, class... Args>
inline typename reference_vector<T, C>::iterator
reference_vector<T, C>::emplace(const_iterator position, Args&& ... args) {
    auto iter = to_internal(position);
    iter = storage_.elements_.emplace(iter, nullptr);
    *iter = create_element<U>(std::forward<Args>(args)...);
    return from_internal(iter);
}

template<class T, class C>
template<class U, class... Args>
inline typename reference_vector<T, C>::reference
reference_vector<T, C>::emplace_back(Args&& ... args) {
    storage_.elements_.push_back(nullptr);
    auto* result = create_element<U>(std::forward<Args>(args)...);
    storage_.elements_.back() = result;
    return *result;
}

template<class T, class C>
template<class U, class D>
inline std::enable_if_t<
        std::is_convertible_v<U&, typename reference_vector<T, C>::reference>,
        typename reference_vector<T, C>::reference>
reference_vector<T, C>::put(const_iterator position, std::unique_ptr<U, D> element) {
    auto iter = to_internal(position);
    auto* replacement = forward_element(std::move(element));
    delete_element(*iter);
    *iter = replacement;
    return *replacement;
}

template<class T, class C>
inline typename reference_vector<T, C>::reference
reference_vector<T, C>::put(const_iterator position, const_reference element) {
    auto iter = to_internal(position);
    auto* replacement = copy_element(element);
    delete_element(*iter);
    *iter = replacement;
    return *replacement;
}

template<class T, class C>
inline typename reference_vector<T, C>::reference
reference_vector<T, C>::put(const_iterator position, rvalue_reference element) {
    auto iter = to_internal(position);
    auto* replacement = copy_element(std::move(element));
    delete_element(*iter);
    *iter = replacement;
    return *replacement;
}

template<class T, class C>
template<class U, class... Args>
inline typename reference_vector<T, C>::reference
reference_vector<T, C>::replace(const_iterator position, Args&& ... args) {
    auto iter = to_internal(position);
    auto* replacement = create_element<U>(std::forward<Args>(args)...);
    delete_element(*iter);
    *iter = replacement;
    return *replacement;
}

template<class T, class C>
inline std::pair<
        unique_object_ptr<typename reference_vector<T, C>::value_type>,
        typename reference_vector<T, C>::iterator>
reference_vector<T, C>::release(const_iterator position) noexcept {
    auto iter = to_internal(position);
    auto released = wrap_unique(*iter);
    iter = storage_.elements_.erase(iter);
    return std::make_pair(std::move(released), from_internal(iter));
}

template<class T, class C>
template<class D>
std::pair<
        reference_vector<typename reference_vector<T, C>::value_type, D>,
        typename reference_vector<T, C>::iterator>
reference_vector<T, C>::release(const_iterator first, const_iterator last) noexcept {
    auto const fst = to_internal(first);
    auto const lst = to_internal(last);

    reference_vector<value_type, D> result { storage_.creator_ };
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

template<class T, class C>
inline unique_object_ptr<typename reference_vector<T, C>::value_type>
reference_vector<T, C>::release_back() noexcept {
    auto released = wrap_unique(storage_.elements_.back());
    storage_.elements_.pop_back();
    return released;
}

template<class T, class C>
inline typename reference_vector<T, C>::iterator
reference_vector<T, C>::begin() noexcept {
    return iterator(storage_.elements_.data());
}

template<class T, class C>
inline typename reference_vector<T, C>::const_iterator
reference_vector<T, C>::begin() const noexcept {
    return cbegin();
}

template<class T, class C>
inline typename reference_vector<T, C>::const_iterator
reference_vector<T, C>::cbegin() const noexcept {
    return const_iterator(storage_.elements_.data());
}

template<class T, class C>
inline typename reference_vector<T, C>::iterator
reference_vector<T, C>::end() noexcept {
    return begin() + storage_.elements_.size();
}

template<class T, class C>
inline typename reference_vector<T, C>::const_iterator
reference_vector<T, C>::end() const noexcept {
    return cend();
}

template<class T, class C>
inline typename reference_vector<T, C>::const_iterator
reference_vector<T, C>::cend() const noexcept {
    return cbegin() + storage_.elements_.size();
}

template<class T, class C>
inline typename reference_vector<T, C>::reverse_iterator
reference_vector<T, C>::rbegin() noexcept {
    return reverse_iterator(end());
}

template<class T, class C>
inline typename reference_vector<T, C>::const_reverse_iterator
reference_vector<T, C>::rbegin() const noexcept {
    return crbegin();
}

template<class T, class C>
inline typename reference_vector<T, C>::const_reverse_iterator
reference_vector<T, C>::crbegin() const noexcept {
    return const_reverse_iterator(cend());
}

template<class T, class C>
inline typename reference_vector<T, C>::reverse_iterator
reference_vector<T, C>::rend() noexcept {
    return reverse_iterator(begin());
}

template<class T, class C>
inline typename reference_vector<T, C>::const_reverse_iterator
reference_vector<T, C>::rend() const noexcept {
    return crend();
}

template<class T, class C>
inline typename reference_vector<T, C>::const_reverse_iterator
reference_vector<T, C>::crend() const noexcept {
    return const_reverse_iterator(cbegin());
}

template<class T, class C>
template<class U>
inline std::enable_if_t<std::is_convertible_v<typename std::initializer_list<U*>::reference, typename reference_vector<T, C>::pointer>>
reference_vector<T, C>::unsafe_assign(std::initializer_list<U*> list) {
    unsafe_assign(list.begin(), list.end());
}

template<class T, class C>
template<class Iter>
inline std::enable_if_t<std::is_convertible_v<typename std::iterator_traits<Iter>::reference, typename reference_vector<T, C>::pointer>>
reference_vector<T, C>::unsafe_assign(Iter first, Iter last) {
    clear();
    unsafe_insert(begin(), first, last);
}

template<class T, class C>
inline typename reference_vector<T, C>::iterator
reference_vector<T, C>::unsafe_insert(const_iterator position, pointer element) {
    auto iter = to_internal(position);
    iter = storage_.elements_.insert(iter, element);
    return from_internal(iter);
}

template<class T, class C>
template<class U>
inline std::enable_if_t<
        std::is_convertible_v<typename std::initializer_list<U*>::reference, typename reference_vector<T, C>::pointer>,
        typename reference_vector<T, C>::iterator>
reference_vector<T, C>::unsafe_insert(const_iterator position, std::initializer_list<U*> list) {
    return unsafe_insert(position, list.begin(), list.end());
}

template<class T, class C>
template<class Iter>
inline std::enable_if_t<
        std::is_convertible_v<typename std::iterator_traits<Iter>::reference, typename reference_vector<T, C>::pointer>,
        typename reference_vector<T, C>::iterator>
reference_vector<T, C>::unsafe_insert(const_iterator position, Iter first, Iter last) {
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

template<class T, class C>
inline typename reference_vector<T, C>::reference
reference_vector<T, C>::unsafe_push_back(pointer element) {
    storage_.elements_.push_back(element);
    return *element;
}

template<class T, class C>
inline typename reference_vector<T, C>::reference
reference_vector<T, C>::unsafe_put(const_iterator position, pointer element) {
    auto iter = to_internal(position);
    delete_element(*iter);
    *iter = element;
    return *element;
}

/// @endcond

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
