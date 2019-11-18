#pragma once

#include <map>
#include <stdexcept>
#include <type_traits>

#include "graph_element_traits.h"
#include "graph_iterator.h"

#include "takatori/util/clonable.h"
#include "takatori/util/object_creator.h"
#include "takatori/util/optional_ptr.h"

namespace takatori::graph {

/**
 * @brief represents a graph.
 * @tparam T the graph element type
 */
template<class T>
class graph {

    static_assert(is_graph_element_v<T>);

public:
    /// @brief the graph element type.
    using element_type = T;

    /// @brief the element ID type.
    using id_type = typename graph_element_traits<element_type>::id_type;

    /// @brief the map entry type.
    using entry_type = std::pair<id_type const, element_type*>;

    /// @brief the entity type
    using entity_type = std::map<
            id_type,
            element_type*,
            std::less<id_type>,
            util::object_creator::allocator_type<entry_type>>;

    /// @brief the value type
    using value_type = element_type;
    /// @brief the size type
    using size_type = std::size_t;
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
    using iterator = graph_iterator<typename entity_type::iterator>;
    /// @brief the const iterator type
    using const_iterator = graph_iterator<typename entity_type::const_iterator>;

    ~graph();
    graph(graph const& other) = delete;
    graph& operator=(graph const& other) = delete;
    graph(graph&& other) noexcept = delete;
    graph& operator=(graph&& other) noexcept = delete;

    /**
     * @brief creates a new object with default object creator.
     */
    graph() = default;

    /**
     * @brief creates a new object.
     * @param creator the object creator
     */
    explicit graph(util::object_creator creator) noexcept;

    /**
     * @brief returns the element which has the given ID.
     * @param id the element ID
     * @return the corresponded element
     * @throws std::domain_error if there is no such the element
     */
    reference at(id_type id);

    /// @copydoc operator[]()
    const_reference at(id_type id) const;

    /**
     * @brief returns the element which has the given ID.
     * @param id the element ID
     * @return the corresponded element
     * @warning undefined behavior if there is no such the element
     */
    reference operator[](id_type id);

    /// @copydoc operator[]()
    const_reference operator[](id_type id) const;

    /**
     * @brief returns the element which has the given ID.
     * @param id the element ID
     * @return the corresponded element
     * @return empty if there is no such the element
     */
    util::optional_ptr<value_type> find(id_type id);

    /// @copydoc find()
    util::optional_ptr<value_type const> find(id_type id) const;

    /**
     * @brief returns whether or not this graph contains the given element.
     * @param id the element ID
     * @return true if there is such the element
     * @return false otherwise
     */
    bool contains(id_type id) const;

    /**
     * @brief returns whether or not this is empty.
     * @return true if this is empty
     * @return false otherwise
     */
    bool empty() const noexcept;

    /**
     * @brief returns the number of elements in this.
     * @return the number of elements
     */
    size_type size() const noexcept;

    /**
     * @brief reserves the capacity.
     * @param size the number of elements to store without expanding this container
     * @attention this is an optional feature
     */
    void reserve(size_type size);

    /**
     * @brief removes all elements in this.
     */
    void clear() noexcept;

    /**
     * @brief removes a element.
     * @param id the target element ID
     * @return true if successfully removed
     * @return false otherwise (may be no such the element)
     */
    bool erase(id_type id);

    /**
     * @brief removes a element on the given iterator.
     * @param position the target element position
     * @return the next position of the erased element
     */
    iterator erase(const_iterator position);

    /**
     * @brief inserts a copy of the given element into this graph.
     * @param element the element
     * @return the added element
     */
    reference insert(const_reference element);

    /// @copydoc insert()
    reference insert(rvalue_reference element);

    /**
     * @brief creates a new element and inserts it into this graph.
     * @tparam T the element type
     * @tparam Args the constructor parameter types of the element
     * @param args the constructor arguments
     * @return the inserted element
     */
    template<class U = element_type, class... Args>
    reference emplace(Args&&... args);

    /**
     * @brief releases a element from this graph.
     * @param id the target element ID
     * @return the released element
     */
    util::unique_object_ptr<value_type> release(id_type id) noexcept;

    /**
     * @brief removes a element on the given iterator.
     * @param position the target position
     * @return a pair of the removed element, and the next position of the released element
     */
    std::pair<util::unique_object_ptr<value_type>, iterator> release(const_iterator position);

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
    std::reverse_iterator<iterator> rbegin() noexcept;

    /// @copydoc rbegin()
    std::reverse_iterator<const_iterator> rbegin() const noexcept;

    /// @copydoc rbegin()
    std::reverse_iterator<const_iterator> crbegin() const noexcept;

    /**
     * @brief returns a backward iterator which points the reversed ending of this container.
     * @return the reversed iterator of ending (exclusive)
     */
    std::reverse_iterator<iterator> rend() noexcept;

    /// @copydoc rend()
    std::reverse_iterator<const_iterator> rend() const noexcept;

    /// @copydoc rend()
    std::reverse_iterator<const_iterator> crend() const noexcept;

    /**
     * @brief exchanges contents between this and the given graph.
     * This also exchanges their object_creator.
     * @param other the target graph
     */
    void swap(graph& other) noexcept;

    /**
     * @brief returns the object creator to create/delete objects in this container.
     * @return the object creator for this container
     */
    util::object_creator get_object_creator() const noexcept;

private:
    entity_type vertices_;
    mutable id_type next_id_ { 1 };

    reference bless_element(entry_type& entry) noexcept;

    void unbless_element(pointer element) noexcept;

    entry_type& prepare_element();

    template<class U, class... Args>
    pointer create_element(Args&&... args);

    void delete_element(pointer element);
};

template<class T>
inline graph<T>::graph(util::object_creator creator) noexcept
    : vertices_(creator.allocator<entry_type>())
{}

template<class T>
inline graph<T>::~graph() {
    for (auto&& kv : vertices_) {
        if (auto*& v = std::get<1>(kv)) {
            delete_element(v);
            v = nullptr;
        }
    }
}

template<class T>
inline typename graph<T>::reference
graph<T>::at(id_type id) {
    if (auto v = find(id)) return *v;
    throw std::domain_error("invalid element");
}

template<class T>
inline typename graph<T>::const_reference
graph<T>::at(id_type id) const {
    if (auto v = find(id)) return *v;
    throw std::domain_error("invalid element");
}

template<class T>
inline typename graph<T>::reference
graph<T>::operator[](id_type id) {
    return *find(id);
}

template<class T>
inline typename graph<T>::const_reference
graph<T>::operator[](id_type id) const {
    return *find(id);
}

template<class T>
inline util::optional_ptr<typename graph<T>::value_type>
graph<T>::find(id_type id) {
    if (auto iter = vertices_.find(id); iter != vertices_.end()) {
        return util::optional_ptr { iter->second };
    }
    return {};
}

template<class T>
inline util::optional_ptr<typename graph<T>::value_type const>
graph<T>::find(id_type id) const {
    if (auto iter = vertices_.find(id); iter != vertices_.end()) {
        return util::optional_ptr { iter->second };
    }
    return {};
}

template<class T>
inline bool
graph<T>::contains(id_type id) const {
    return find(id).has_value();
}

template<class T>
bool graph<T>::empty() const noexcept {
    return vertices_.empty();
}

template<class T>
inline typename graph<T>::size_type
graph<T>::size() const noexcept {
    return vertices_.size();
}

template<class T>
inline void
graph<T>::reserve(graph::size_type size) {
    // nothing to do.
    (void) size;
}

template<class T>
inline void
graph<T>::clear() noexcept {
    for (auto&& kv : vertices_) {
        if (auto*& v = std::get<1>(kv)) {
            delete_element(v);
            v = nullptr;
        }
    }
    vertices_.clear();
}

template<class T>
inline bool
graph<T>::erase(id_type id) {
    if (auto iter = vertices_.find(id); iter != vertices_.end()) {
        erase(const_iterator(iter));
        return true;
    }
    return false;
}

template<class T>
inline typename graph<T>::iterator
graph<T>::erase(graph::const_iterator position) {
    auto iter = position.unwrap();
    delete_element(iter->second);
    auto next = vertices_.erase(iter);
    return iterator(next);
}

template<class T>
inline typename graph<T>::reference
graph<T>::insert(const_reference element) {
    static_assert(util::is_clonable_v<element_type>);
    auto&& pair = prepare_element();
    std::get<1>(pair) = util::clone(element, get_object_creator());
    return bless_element(pair);
}

template<class T>
inline typename graph<T>::reference
graph<T>::insert(rvalue_reference element) {
    static_assert(util::is_clonable_v<element_type>);
    auto&& pair = prepare_element();
    std::get<1>(pair) = util::clone(std::move(element), get_object_creator());
    return bless_element(pair);
}

template<class T>
template<class U, class... Args>
inline typename graph<T>::reference
graph<T>::emplace(Args&& ... args) {
    static_assert(util::is_clonable_v<element_type>);
    auto&& pair = prepare_element();
    std::get<1>(pair) = create_element<U>(std::forward<Args>(args)...);
    return bless_element(pair);
}

template<class T>
util::unique_object_ptr<typename graph<T>::value_type>
graph<T>::release(id_type id) noexcept {
    if (auto iter = vertices_.find(id); iter != vertices_.end()) {
        auto kv = release(const_iterator(iter));
        return std::get<0>(std::move(kv));
    }
    return {};
}

template<class T>
inline std::pair<
        util::unique_object_ptr<typename graph<T>::value_type>,
        typename graph<T>::iterator>
graph<T>::release(graph::const_iterator position) {
    auto iter = position.unwrap();
    auto result = get_object_creator().wrap_unique(iter->second);
    unbless_element(result.get());
    auto next = vertices_.erase(iter);
    return std::make_pair(std::move(result), iterator { next });
}

template<class T>
inline typename graph<T>::iterator
graph<T>::begin() noexcept {
    return iterator { vertices_.begin() };
}

template<class T>
inline typename graph<T>::const_iterator
graph<T>::begin() const noexcept {
    return cbegin();
}

template<class T>
inline typename graph<T>::const_iterator
graph<T>::cbegin() const noexcept {
    return iterator { vertices_.cbegin() };
}

template<class T>
inline typename graph<T>::iterator
graph<T>::end() noexcept {
    return iterator { vertices_.end() };
}

template<class T>
inline typename graph<T>::const_iterator
graph<T>::end() const noexcept {
    return cend();
}

template<class T>
inline typename graph<T>::const_iterator
graph<T>::cend() const noexcept {
    return iterator { vertices_.cend() };
}

template<class T>
inline std::reverse_iterator<typename graph<T>::iterator>
graph<T>::rbegin() noexcept {
    return std::make_reverse_iterator(end());
}

template<class T>
inline std::reverse_iterator<typename graph<T>::const_iterator>
graph<T>::rbegin() const noexcept {
    return crbegin();
}

template<class T>
inline std::reverse_iterator<typename graph<T>::const_iterator>
graph<T>::crbegin() const noexcept {
    return std::make_reverse_iterator(cend());
}

template<class T>
inline std::reverse_iterator<typename graph<T>::iterator>
graph<T>::rend() noexcept {
    return std::make_reverse_iterator(begin());
}

template<class T>
inline std::reverse_iterator<typename graph<T>::const_iterator>
graph<T>::rend() const noexcept {
    return crend();
}

template<class T>
inline std::reverse_iterator<typename graph<T>::const_iterator>
graph<T>::crend() const noexcept {
    return std::make_reverse_iterator(cbegin());
}

template<class T>
inline void
graph<T>::swap(graph& other) noexcept {
    std::swap(vertices_, other.vertices_);
    std::swap(next_id_, other.next_id_);
}

template<class T>
inline util::object_creator
graph<T>::get_object_creator() const noexcept {
    return util::object_creator { vertices_.get_allocator() };
}

template<class T>
inline typename graph<T>::reference
graph<T>::bless_element(entry_type& entry) noexcept {
    auto id = std::get<0>(entry);
    auto* element = std::get<1>(entry);
    graph_element_traits<element_type>::join(*element, *this, id);
    return *element;
}

template<class T>
inline void
graph<T>::unbless_element(pointer element) noexcept {
    if (element != nullptr) {
        graph_element_traits<element_type>::leave(*element);
    }
}

template<class T>
inline typename graph<T>::entry_type&
graph<T>::prepare_element() {
    auto id = next_id_++;
    auto [iter, success] = vertices_.emplace(id, nullptr);
    if (!success) {
        throw std::logic_error("conflict element ID");
    }
    return *iter;
}

template<class T>
template<class U, class... Args>
inline typename graph<T>::pointer
graph<T>::create_element(Args&& ... args) {
    return get_object_creator().template create_object<U>(std::forward<Args>(args)...);
}

template<class T>
inline void
graph<T>::delete_element(pointer element) {
    if (element != nullptr) {
        get_object_creator().delete_object(element);
    }
}

} // namespace takatori::graph
