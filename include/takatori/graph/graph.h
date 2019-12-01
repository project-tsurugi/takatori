#pragma once

#include <functional>
#include <stdexcept>
#include <type_traits>
#include <unordered_set>
#include <utility>

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

    /// @brief the set entry type.
    using entry_type = element_type*;

    /// @brief the entity type
    using entity_type = std::unordered_set<
            entry_type,
            std::hash<entry_type>,
            std::equal_to<>,
            util::object_allocator<entry_type>>;

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

    /**
     * @brief creates a new object with default object creator.
     */
    graph() = default;

    ~graph();
    graph(graph const& other) = delete;
    graph& operator=(graph const& other) = delete;

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    graph(graph&& other) noexcept;

    /**
     * @brief assigns the given object.
     * @param other the move source
     * @return this
     */
    graph& operator=(graph&& other) noexcept;

    /**
     * @brief creates a new object.
     * @param creator the object creator
     */
    explicit graph(util::object_creator creator) noexcept;

    /**
     * @brief returns whether or not this graph contains the given element.
     * @param element the target element
     * @return true if there is such the element
     * @return false otherwise
     */
    bool contains(const_reference element) const;

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
     * @brief removes an element.
     * @param element the target element
     * @return true if successfully removed
     * @return false otherwise (may be no such the element)
     */
    bool erase(const_reference element);

    /**
     * @brief removes an element on the given iterator.
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
    U& emplace(Args&&... args);

    /**
     * @brief releases the element on this graph.
     * @param element the target element
     * @return the released element
     */
    util::unique_object_ptr<value_type> release(const_reference element) noexcept;

    /**
     * @brief removes an element on the given iterator.
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
     * @details This also exchanges their object_creator.
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

    static constexpr entry_type entry_key(const_reference element) noexcept {
        return const_cast<entry_type>(std::addressof(element));  // NOLINT
    }

    reference bless_element(pointer element) noexcept {
        graph_element_traits<element_type>::join(*element, *this);
        return *element;
    }

    void unbless_element(pointer element) noexcept {
        if (element != nullptr) {
            graph_element_traits<element_type>::leave(*element);
        }
    }

    template<class U>
    U* insert_element(util::unique_object_ptr<U> element) {
        auto [iter, success] = vertices_.emplace(element.get());
        if (!success) {
            throw std::logic_error("conflict element ID");
        }
        (void) iter;
        return element.release();
    }

    template<class U, class... Args>
    util::unique_object_ptr<U> create_element(Args&&... args) {
        return get_object_creator().template create_unique<U>(std::forward<Args>(args)...);
    }

    void delete_element(pointer element) noexcept {
        if (element != nullptr) {
            get_object_creator().delete_object(element);
        }
    }
};

template<class T>
graph<T>::graph(graph&& other) noexcept
    : vertices_(std::move(other.vertices_))
{
    for (auto* e : vertices_) {
        bless_element(e);
    }
}

template<class T>
graph<T>& graph<T>::operator=(graph&& other) noexcept {
    clear();
    vertices_ = std::move(other.vertices_);
    for (auto* e : vertices_) {
        bless_element(e);
    }
    return *this;
}

template<class T>
inline graph<T>::graph(util::object_creator creator) noexcept
    : vertices_(creator.allocator<entry_type>())
{}

template<class T>
inline graph<T>::~graph() {
    for (auto&& entry : vertices_) {
        delete_element(entry);
    }
}

template<class T>
inline bool
graph<T>::contains(const_reference element) const {
    auto iter = vertices_.find(entry_key(element));
    return iter != vertices_.end();
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
    vertices_.reserve(size);
}

template<class T>
inline void
graph<T>::clear() noexcept {
    for (auto&& entry : vertices_) {
        delete_element(entry);
    }
    vertices_.clear();
}

template<class T>
inline bool
graph<T>::erase(const_reference element) {
    if (auto iter = vertices_.find(entry_key(element)); iter != vertices_.end()) {
        erase(const_iterator(iter));
        return true;
    }
    return false;
}

template<class T>
inline typename graph<T>::iterator
graph<T>::erase(graph::const_iterator position) {
    auto iter = position.unwrap();
    delete_element(*iter);
    auto next = vertices_.erase(iter);
    return iterator(next);
}

template<class T>
inline typename graph<T>::reference
graph<T>::insert(const_reference element) {
    static_assert(util::is_clonable_v<element_type>);
    auto entry = insert_element(util::clone_unique(element, get_object_creator()));
    return bless_element(entry);
}

template<class T>
inline typename graph<T>::reference
graph<T>::insert(rvalue_reference element) {
    static_assert(util::is_clonable_v<element_type>);
    auto entry = insert_element(util::clone_unique(std::move(element), get_object_creator()));
    return bless_element(entry);
}

template<class T>
template<class U, class... Args>
inline U&
graph<T>::emplace(Args&& ... args) {
    static_assert(util::is_clonable_v<element_type>);
    auto entry = insert_element(create_element<U>(std::forward<Args>(args)...));
    bless_element(entry);
    return *entry;
}

template<class T>
inline util::unique_object_ptr<typename graph<T>::value_type>
graph<T>::release(const_reference element) noexcept {
    if (auto iter = vertices_.find(entry_key(element)); iter != vertices_.end()) {
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
    auto result = get_object_creator().wrap_unique(*iter);
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
}

template<class T>
inline util::object_creator
graph<T>::get_object_creator() const noexcept {
    return util::object_creator { vertices_.get_allocator() };
}

} // namespace takatori::graph
