#pragma once

#include <functional>
#include <stdexcept>
#include <type_traits>
#include <unordered_set>
#include <utility>

#include "graph_element_traits.h"
#include "graph_iterator.h"

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>
#include <takatori/util/object_creator.h>
#include <takatori/util/optional_ptr.h>

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

    ~graph() {
        for (auto&& entry : vertices_) {
            delete_element(entry);
        }
    }

    graph(graph const& other) = delete;
    graph& operator=(graph const& other) = delete;

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    graph(graph&& other) noexcept
        : vertices_(std::move(other.vertices_))
    {
        for (auto* e : vertices_) {
            bless_element(e);
        }
    }

    /**
     * @brief assigns the given object.
     * @param other the move source
     * @return this
     */
    graph& operator=(graph&& other) noexcept {
        clear();
        vertices_ = std::move(other.vertices_);
        for (auto* e : vertices_) {
            bless_element(e);
        }
        return *this;
    }

    /**
     * @brief creates a new object.
     * @param creator the object creator
     */
    explicit graph(util::object_creator creator) noexcept
        : vertices_(creator.allocator<entry_type>())
    {}

    /**
     * @brief returns whether or not this graph contains the given element.
     * @param element the target element
     * @return true if there is such the element
     * @return false otherwise
     */
    bool contains(const_reference element) const {
        auto iter = vertices_.find(entry_key(element));
        return iter != vertices_.end();
    }

    /**
     * @brief returns whether or not this is empty.
     * @return true if this is empty
     * @return false otherwise
     */
    bool empty() const noexcept {
        return vertices_.empty();
    }

    /**
     * @brief returns the number of elements in this.
     * @return the number of elements
     */
    size_type size() const noexcept {
        return vertices_.size();
    }

    /**
     * @brief reserves the capacity.
     * @param size the number of elements to store without expanding this container
     * @attention this is an optional feature
     */
    void reserve(size_type size) {
        vertices_.reserve(size);
    }

    /**
     * @brief removes all elements in this.
     */
    void clear() noexcept {
        for (auto&& entry : vertices_) {
            delete_element(entry);
        }
        vertices_.clear();
    }

    /**
     * @brief find for an element.
     * @param element the target element
     * @return the element position
     * @return end() if it is not found
     */
    iterator find(const_reference element) {
        auto iter = vertices_.find(entry_key(element));
        return iterator(iter);
    }

    /// @copydoc find()
    const_iterator find(const_reference element) const {
        auto iter = vertices_.find(entry_key(element));
        return const_iterator(iter);
    }

    /**
     * @brief removes an element.
     * @param element the target element
     * @return true if successfully removed
     * @return false otherwise (may be no such the element)
     */
    bool erase(const_reference element) {
        if (auto iter = vertices_.find(entry_key(element)); iter != vertices_.end()) {
            erase(const_iterator(iter));
            return true;
        }
        return false;
    }

    /**
     * @brief removes an element on the given iterator.
     * @param position the target element position
     * @return the next position of the erased element
     */
    iterator erase(const_iterator position) {
        auto iter = position.unwrap();
        delete_element(*iter);
        auto next = vertices_.erase(iter);
        return iterator(next);
    }

    /**
     * @brief inserts a copy of the given element into this graph.
     * @tparam U the element type
     * @param element the element
     * @return the added element
     */
    template<
            class U,
            class = std::enable_if_t<
                    std::is_convertible_v<U, const_reference>>>
    U& insert(U&& element) {
        static_assert(util::is_clonable_v<element_type>);
        auto entry = insert_element(util::clone_unique(std::forward<U>(element), get_object_creator()));
        bless_element(entry);
        return *entry;
    }

    /**
     * @brief inserts the given element into this graph.
     * @details if the allocator of the element is not compatible to this graph,
     *      this inserts a copy of the element.
     * @tparam U the element type
     * @tparam D the deleter type
     * @param element the element
     * @return the added element
     * @throw if the element is absent
     * @see get_object_creator()
     * @see util::object_creator::is_instance()
     */
    template<
            class U,
            class D,
            class = std::enable_if_t<
                    std::is_convertible_v<U, const_reference>>>
    U& insert(std::unique_ptr<U, D> element) {
        if (!element) {
            throw std::invalid_argument("element must not be absent");
        }
        if (!get_object_creator().is_instance(element)) {
            return insert(util::clone_unique(std::move(*element), get_object_creator()));
        }
        auto entry = insert_element(std::move(element));
        bless_element(entry);
        return *entry;
    }

    /**
     * @brief creates a new element and inserts it into this graph.
     * @tparam T the element type
     * @tparam Args the constructor parameter types of the element
     * @param args the constructor arguments
     * @return the inserted element
     */
    template<class U = element_type, class... Args>
    U& emplace(Args&&... args) {
        static_assert(util::is_clonable_v<element_type>);
        auto entry = insert_element(create_element<U>(std::forward<Args>(args)...));
        bless_element(entry);
        return *entry;
    }

    /**
     * @brief releases the element on this graph.
     * @param element the target element
     * @return the released element
     */
    util::unique_object_ptr<value_type> release(const_reference element) noexcept {
        if (auto iter = vertices_.find(entry_key(element)); iter != vertices_.end()) {
            auto kv = release(const_iterator(iter));
            return std::get<0>(std::move(kv));
        }
        return {};
    }

    /**
     * @brief removes an element on the given iterator.
     * @param position the target position
     * @return a pair of the removed element, and the next position of the released element
     */
    std::pair<util::unique_object_ptr<value_type>, iterator> release(const_iterator position) {
        auto iter = position.unwrap();
        auto result = get_object_creator().wrap_unique(*iter);
        unbless_element(result.get());
        auto next = vertices_.erase(iter);
        return std::make_pair(std::move(result), iterator { next });
    }

    /**
     * @brief merges vertices and their connections into this graph.
     * @param other the source graph
     * @throws std::invalid_argument if get_object_creator() != other.get_object_creator()
     */
    void merge(graph&& other) {
        if (get_object_creator() != other.get_object_creator()) {
            throw std::invalid_argument("inconsistent allocator");
        }
        for (auto* v : other.vertices_) {
            bless_element(v);
        }
        vertices_.merge(other.vertices_);
        other.clear();
    }

    /**
     * @brief returns a forward iterator which points the beginning of this container.
     * @return the iterator of beginning (inclusive)
     */
    iterator begin() noexcept {
        return iterator { vertices_.begin() };
    }

    /// @copydoc begin()
    const_iterator begin() const noexcept {
        return cbegin();
    }

    /// @copydoc begin()
    const_iterator cbegin() const noexcept {
        return const_iterator { vertices_.cbegin() };
    }

    /**
     * @brief returns a forward iterator which points the ending of this container.
     * @return the iterator of ending (exclusive)
     */
    iterator end() noexcept {
        return iterator { vertices_.end() };
    }

    /// @copydoc end()
    const_iterator end() const noexcept {
        return cend();
    }

    /// @copydoc end()
    const_iterator cend() const noexcept {
        return const_iterator { vertices_.cend() };
    }

    /**
     * @brief returns a backward iterator which points the reversed beginning of this container.
     * @return the reversed iterator of beginning (inclusive)
     */
    std::reverse_iterator<iterator> rbegin() noexcept {
        return std::make_reverse_iterator(end());
    }

    /// @copydoc rbegin()
    std::reverse_iterator<const_iterator> rbegin() const noexcept {
        return crbegin();
    }

    /// @copydoc rbegin()
    std::reverse_iterator<const_iterator> crbegin() const noexcept {
        return std::make_reverse_iterator(cend());
    }

    /**
     * @brief returns a backward iterator which points the reversed ending of this container.
     * @return the reversed iterator of ending (exclusive)
     */
    std::reverse_iterator<iterator> rend() noexcept {
        return std::make_reverse_iterator(begin());
    }

    /// @copydoc rend()
    std::reverse_iterator<const_iterator> rend() const noexcept {
        return crend();
    }

    /// @copydoc rend()
    std::reverse_iterator<const_iterator> crend() const noexcept {
        return std::make_reverse_iterator(cbegin());
    }

    /**
     * @brief exchanges contents between this and the given graph.
     * @details This also exchanges their object_creator.
     * @param other the target graph
     */
    void swap(graph& other) noexcept {
        std::swap(vertices_, other.vertices_);
        for (auto* v : vertices_) {
            bless_element(v);
        }
        for (auto* v : other.vertices_) {
            other.bless_element(v);
        }
    }

    /**
     * @brief returns the object creator to create/delete objects in this container.
     * @return the object creator for this container
     */
    util::object_creator get_object_creator() const noexcept {
        return util::object_creator { vertices_.get_allocator() };
    }

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

} // namespace takatori::graph
