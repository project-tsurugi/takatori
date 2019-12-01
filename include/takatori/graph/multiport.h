#pragma once

#include <algorithm>
#include <utility>

#include <boost/container/small_vector.hpp>

#include "port_direction.h"

#include "takatori/util/object_creator.h"
#include "takatori/util/print_support.h"
#include "takatori/util/reference_list_view.h"

namespace takatori::graph {

template<class T, port_direction Direction>
class multiport;

/**
 * @brief the input port type.
 */
template<class T>
using input_multiport = multiport<T, port_direction::input>;

/**
 * @brief the output port type.
 */
template<class T>
using output_multiport = multiport<T, port_direction::output>;

/**
 * @brief the view of port list.
 */
template<class Port>
using port_list_view = util::reference_list_view<util::double_pointer_extractor<Port>>;

/**
 * @brief represents an input/output port of graph nodes, which can have multiple opposites.
 * @tparam T the node type
 * @tparam Direction the port direction
 */
template<class T, port_direction Direction>
class multiport final {
public:
    /// @brief the vertex type
    using node_type = T;

    /// @brief port direction type.
    using direction_type = port_direction;

    /// @brief the port direction.
    static constexpr direction_type direction = Direction;

    /// @brief the opposite port type.
    using opposite_type = multiport<T, ~direction>;

    /// @brief the ID type of the ports.
    using id_type = std::uint64_t;

    /**
     * @brief constructs a new object.
     * @param owner the owner node
     * @param id the port ID (in node)
     * @param creator the object creator
     * @warning don't invoke this outside of owner node
     */
    explicit multiport(node_type& owner, id_type id = 0, util::object_creator creator = {}) noexcept;

    ~multiport();

    multiport(multiport const& other) = delete;
    multiport& operator=(multiport const& other) = delete;

    /**
     * @brief constructs a new object.
     * @param other the move source
     * @warning this is designed for STL like containers, developers should not use this directly
     */
    multiport(multiport&& other) noexcept;

    /**
     * @brief assigns the given object.
     * @param other the move source
     * @return this
     * @warning this is designed for STL like containers, developers should not use this directly
     */
    multiport& operator=(multiport&& other) noexcept;

    /**
     * @brief returns the port ID.
     * @return the port ID (in node).
     */
    constexpr id_type id() const noexcept;

    /**
     * @brief returns the node which owns this port.
     * @return the owner
     */
    constexpr node_type& owner() noexcept;

    /// @copydoc owner()
    constexpr node_type const& owner() const noexcept;

    /**
     * @brief returns the list of opposite ports.
     * @return the opposite ports
     * @attention the returned list was disabled after the opposite set was changed
     */
    port_list_view<opposite_type> opposites() noexcept;

    /// @copydoc opposites()
    port_list_view<opposite_type const> opposites() const noexcept;

    /**
     * @brief connects to a opposite port.
     * @param opposite the opposite port
     * @return true if successfully connected
     * @return false otherwise
     */
    bool connect_to(opposite_type& opposite);

    /**
     * @brief disconnects from the opposite port.
     * @param opposite the opposite port
     * @return true if successfully connected
     * @return false otherwise, may by not yet connected
     */
    bool disconnect_from(opposite_type& opposite);

    /**
     * @brief disconnects from the all opposite ports.
     */
    void disconnect_all();

private:
    id_type id_;
    node_type* owner_;
    static constexpr std::size_t opposite_buffer_size = 4;
    using opposite_buffer_allocator_type = util::object_allocator<opposite_type*>;
    boost::container::small_vector<opposite_type*, opposite_buffer_size, opposite_buffer_allocator_type> opposites_ {};

    bool internal_connect(opposite_type& opposite);
    bool internal_disconnect(opposite_type& opposite);
    bool internal_reconnect(opposite_type& old_opposite, opposite_type& new_opposite) noexcept;

    friend class multiport<node_type, ~direction>;
};

template<class T, port_direction Direction>
inline
multiport<T, Direction>::multiport(node_type& owner, multiport::id_type id, util::object_creator creator) noexcept
    : id_(id)
    , owner_(std::addressof(owner))
    , opposites_(typename decltype(opposites_)::allocator_type(creator.allocator<opposite_type*>()))
{}

template<class T, port_direction Direction>
inline
multiport<T, Direction>::~multiport() {
    disconnect_all();
}

template<class T, port_direction Direction>
inline
multiport<T, Direction>::multiport(multiport&& other) noexcept
    : id_(other.id_)
    , owner_(other.owner_)
    , opposites_(std::move(other.opposites_))
{
    // NOTE: boost::container::small_vector may not become empty on move
    other.opposites_.clear();
    for (auto* p : opposites_) {
        p->internal_reconnect(other, *this);
    }
}

template<class T, port_direction Direction>
inline
multiport<T, Direction>& multiport<T, Direction>::operator=(multiport&& other) noexcept {
    disconnect_all();
    id_ = other.id_;
    owner_ = other.owner_;
    opposites_ = std::move(other.opposites_);
    // NOTE: boost::container::small_vector may not become empty on move
    other.opposites_.clear();
    for (auto* p : opposites_) {
        p->internal_reconnect(other, *this);
    }
    return *this;
}

template<class T, port_direction Direction>
inline constexpr typename multiport<T, Direction>::id_type
multiport<T, Direction>::id() const noexcept {
    return id_;
}

template<class T, port_direction Direction>
inline constexpr typename multiport<T, Direction>::node_type&
multiport<T, Direction>::owner() noexcept {
    return *owner_;
}

template<class T, port_direction Direction>
inline constexpr typename multiport<T, Direction>::node_type const&
multiport<T, Direction>::owner() const noexcept {
    return *owner_;
}

template<class T, port_direction Direction>
inline port_list_view<typename multiport<T, Direction>::opposite_type>
multiport<T, Direction>::opposites() noexcept {
    return port_list_view<opposite_type> { opposites_.data(), opposites_.size() };
}

template<class T, port_direction Direction>
inline port_list_view<typename multiport<T, Direction>::opposite_type const>
multiport<T, Direction>::opposites() const noexcept {
    return port_list_view<opposite_type> { opposites_.data(), opposites_.size() };
}

template<class T, port_direction Direction>
inline bool
multiport<T, Direction>::connect_to(opposite_type& opposite) {
    bool a = internal_connect(opposite);
    bool b = opposite.internal_connect(*this);
    if (a != b) {
        throw std::domain_error("inconsistent connection");
    }
    return true;
}

template<class T, port_direction Direction>
inline bool
multiport<T, Direction>::disconnect_from(opposite_type& opposite) {
    bool a = internal_disconnect(opposite);
    bool b = opposite.internal_disconnect(*this);
    if (a != b) {
        throw std::domain_error("inconsistent connection");
    }
    return a;
}

template<class T, port_direction Direction>
inline void
multiport<T, Direction>::disconnect_all() {
    for (auto*& opposite : opposites_) {
        if (opposite != nullptr) {
            opposite->internal_disconnect(*this);
            opposite = nullptr;
        }
    }
    opposites_.clear();
}

template<class T, port_direction Direction>
inline bool
multiport<T, Direction>::internal_connect(opposite_type& opposite) {
    // NOTE: multi connections
    opposites_.emplace_back(std::addressof(opposite));
    return true;
}

template<class T, port_direction Direction>
inline bool
multiport<T, Direction>::internal_disconnect(opposite_type& opposite) {
    // NOTE: removes only 1 connection
    if (auto iter = std::find(opposites_.begin(), opposites_.end(), std::addressof(opposite));
            iter != opposites_.end()) {
        opposites_.erase(iter);
        return true;
    }
    return false;
}

template<class T, port_direction Direction>
inline bool
multiport<T, Direction>::internal_reconnect(
        opposite_type& old_opposite,
        opposite_type& new_opposite) noexcept {
    // NOTE: replaces only 1 connection
    if (auto iter = std::find(opposites_.begin(), opposites_.end(), std::addressof(old_opposite));
            iter != opposites_.end()) {
        *iter = std::addressof(new_opposite);
        return true;
    }
    return false;
}

/**
 * @brief connects downstream port from upstream.
 * @tparam T the node type
 * @param downstream the downstream port
 * @param upstream the upstream port
 * @return the downstream port
 */
template<class T>
inline input_multiport<T>& operator<<(input_multiport<T>& downstream, output_multiport<T>& upstream) {
    downstream.connect_to(upstream);
    return downstream;
}

/**
 * @brief connects upstream port into downstream.
 * @tparam T the node type
 * @param upstream the upstream port
 * @param downstream the downstream port
 * @return the upstream port
 */
template<class T>
inline output_multiport<T>& operator>>(output_multiport<T>& upstream, input_multiport<T>& downstream) {
    upstream.connect_to(downstream);
    return upstream;
}

/**
 * @brief appends string representation of the given value.
 * @tparam T the node type
 * @tparam Direction the port direction
 * @param out the target output
 * @param value the target value
 * @return the output
 */
template<class T, port_direction Direction>
inline std::ostream& operator<<(std::ostream& out, multiport<T, Direction> const& value) {
    return out << Direction << "[" << value.id() << "]"
               << "@" << util::print_support { value.owner() };
}

} // namespace takatori::graph
