#pragma once

#include <functional>
#include <type_traits>
#include <unordered_set>
#include <utility>

#include "graph_element_traits.h"
#include "graph_iterator.h"

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>
#include <takatori/util/exception.h>
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
    using entry_type = std::add_pointer_t<element_type>;

    /// @brief the entity type
    using entity_type = std::unordered_set<
            entry_type,
            std::hash<entry_type>,
            std::equal_to<>>;

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
     * @brief creates a new object.
     */
    graph() = default;

    graph(graph const& other) = delete;
    graph& operator=(graph const& other) = delete;

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    graph(graph&& other) noexcept :
        vertices_ { std::move(other.vertices_) }
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
     * @brief destructs this object.
     */
    ~graph() {
        for (auto&& entry : vertices_) {
            delete_element(entry);
        }
    }

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
    [[nodiscard]] bool empty() const noexcept {
        return vertices_.empty();
    }

    /**
     * @brief returns the number of elements in this.
     * @return the number of elements
     */
    [[nodiscard]] size_type size() const noexcept {
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
    [[nodiscard]] iterator find(const_reference element) {
        auto iter = vertices_.find(entry_key(element));
        return iterator(iter);
    }

    /// @copydoc find()
    [[nodiscard]] const_iterator find(const_reference element) const {
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
                    std::is_convertible_v<U, const_reference>>> // NOLINT(modernize-type-traits)
    U& insert(U&& element) {
        static_assert(util::is_clonable_v<element_type>);
        auto* entry = insert_element(util::clone_unique(std::forward<U>(element)));
        bless_element(entry);
        return *entry;
    }

    /**
     * @brief inserts the given element into this graph.
     * @tparam U the element type
     * @param element the element
     * @return the added element
     * @throws if the element is absent
     */
    template<
            class U,
            class = std::enable_if_t<
                    std::is_convertible_v<U, const_reference>>>
    U& insert(std::unique_ptr<U> element) {
        if (!element) {
            util::throw_exception(std::invalid_argument("element must not be absent"));
        }
        auto* entry = insert_element(std::move(element));
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
        static_assert(std::is_base_of_v<value_type, U>);
        auto* entry = insert_element(create_element<U>(std::forward<Args>(args)...));
        bless_element(entry);
        return *entry;
    }

    /**
     * @brief releases the element on this graph.
     * @param element the target element
     * @return the released element
     */
    std::unique_ptr<value_type> release(const_reference element) noexcept {
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
    std::pair<std::unique_ptr<value_type>, iterator> release(const_iterator position) {
        auto iter = position.unwrap();
        std::unique_ptr<value_type> result { *iter };
        unbless_element(result.get());
        auto next = vertices_.erase(iter);
        return std::make_pair(std::move(result), iterator { next });
    }

    /**
     * @brief merges vertices and their connections into this graph.
     * @param other the source graph
     */
    void merge(graph&& other) {
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
    [[nodiscard]] iterator begin() noexcept {
        return iterator { vertices_.begin() };
    }

    /// @copydoc begin()
    [[nodiscard]] const_iterator begin() const noexcept {
        return cbegin();
    }

    /// @copydoc begin()
    [[nodiscard]] const_iterator cbegin() const noexcept {
        return const_iterator { vertices_.cbegin() };
    }

    /**
     * @brief returns a forward iterator which points the ending of this container.
     * @return the iterator of ending (exclusive)
     */
    [[nodiscard]] iterator end() noexcept {
        return iterator { vertices_.end() };
    }

    /// @copydoc end()
    [[nodiscard]] const_iterator end() const noexcept {
        return cend();
    }

    /// @copydoc end()
    [[nodiscard]] const_iterator cend() const noexcept {
        return const_iterator { vertices_.cend() };
    }

    /**
     * @brief returns a backward iterator which points the reversed beginning of this container.
     * @return the reversed iterator of beginning (inclusive)
     */
    [[nodiscard]] std::reverse_iterator<iterator> rbegin() noexcept {
        return std::make_reverse_iterator(end());
    }

    /// @copydoc rbegin()
    [[nodiscard]] std::reverse_iterator<const_iterator> rbegin() const noexcept {
        return crbegin();
    }

    /// @copydoc rbegin()
    [[nodiscard]] std::reverse_iterator<const_iterator> crbegin() const noexcept {
        return std::make_reverse_iterator(cend());
    }

    /**
     * @brief returns a backward iterator which points the reversed ending of this container.
     * @return the reversed iterator of ending (exclusive)
     */
    [[nodiscard]] std::reverse_iterator<iterator> rend() noexcept {
        return std::make_reverse_iterator(begin());
    }

    /// @copydoc rend()
    [[nodiscard]] std::reverse_iterator<const_iterator> rend() const noexcept {
        return crend();
    }

    /// @copydoc rend()
    [[nodiscard]] std::reverse_iterator<const_iterator> crend() const noexcept {
        return std::make_reverse_iterator(cbegin());
    }

    /**
     * @brief exchanges contents between this and the given graph.
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

private:
    entity_type vertices_;

    [[nodiscard]] static constexpr entry_type entry_key(const_reference element) noexcept {
        return const_cast<entry_type>(std::addressof(element));  // NOLINT
    }

    reference bless_element(entry_type element) noexcept {
        graph_element_traits<element_type>::join(*element, *this);
        return *element;
    }

    void unbless_element(entry_type element) noexcept {
        if (element != nullptr) {
            graph_element_traits<element_type>::leave(*element);
        }
    }

    template<class U>
    U* insert_element(std::unique_ptr<U> element) {
        auto [iter, success] = vertices_.emplace(element.get());
        if (!success) {
            util::throw_exception(std::logic_error("conflict element ID"));
        }
        (void) iter;
        return element.release();
    }

    template<class U, class... Args>
    [[nodiscard]] std::unique_ptr<U> create_element(Args&&... args) {
        return std::make_unique<U>(std::forward<Args>(args)...);
    }

    static void delete_element(pointer element) noexcept {
        delete element; // NOLINT
    }
};

} // namespace takatori::graph
