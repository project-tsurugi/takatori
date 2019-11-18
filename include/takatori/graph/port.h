#pragma once

#include <algorithm>
#include <utility>

#include <boost/container/small_vector.hpp>

#include "port_direction.h"

#include "takatori/util/print_support.h"
#include "takatori/util/reference_list_view.h"

namespace takatori::graph {

template<class T, port_direction Direction>
class port;

/**
 * @brief the input port type.
 */
template<class T>
using input_port = port<T, port_direction::input>;

/**
 * @brief the output port type.
 */
template<class T>
using output_port = port<T, port_direction::output>;

/**
 * @brief the view of port list.
 */
template<class Port>
using port_list_view = util::reference_list_view<util::double_pointer_extractor<Port>>;

/**
 * @brief represents an input/output port of graph nodes.
 * @tparam T the node type
 * @tparam Direction the port direction
 */
template<class T, port_direction Direction>
class port final {
public:
    /// @brief the vertex type
    using node_type = T;

    /// @brief port direction type.
    using direction_type = port_direction;

    /// @brief the port direction.
    static constexpr direction_type direction = Direction;

    /// @brief the opposite port type.
    using opposite_port_type = port<T, ~direction>;

    /// @brief the ID type of the ports.
    using id_type = std::uint64_t;

    /**
     * @brief constructs a new object.
     * @param owner the owner node
     * @param id the port ID (in node)
     * @warning don't invoke this outside of owner node
     */
    explicit constexpr port(node_type& owner, id_type id = 0) noexcept;

    ~port();

    port(port const& other) = delete;
    port& operator=(port const& other) = delete;
    port(port&& other) noexcept = delete;
    port& operator=(port&& other) noexcept = delete;

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
    port_list_view<opposite_port_type> opposites() noexcept;

    /// @copydoc opposites()
    port_list_view<opposite_port_type const> opposites() const noexcept;

    /**
     * @brief connects to a opposite port.
     * @param opposite the opposite port
     * @return true if successfully connected
     * @return false otherwise
     */
    bool connect_to(opposite_port_type& opposite);

    /**
     * @brief disconnects from the opposite port.
     * @param opposite the opposite port
     * @return true if successfully connected
     * @return false otherwise, may by not yet connected
     */
    bool disconnect_from(opposite_port_type& opposite);

    /**
     * @brief disconnects from the all opposite ports.
     */
    void disconnect_all();

private:
    id_type id_;
    node_type* owner_;
    static constexpr std::size_t opposite_buffer_size = 4;
    boost::container::small_vector<opposite_port_type*, opposite_buffer_size> opposites_ {};

    bool do_connect(opposite_port_type& opposite);
    bool do_disconnect(opposite_port_type& opposite);

    friend class port<node_type, ~direction>;
};

template<class T, port_direction Direction>
inline constexpr
port<T, Direction>::port(node_type& owner, port::id_type id) noexcept
    : id_(id)
    , owner_(std::addressof(owner))
{}

template<class T, port_direction Direction>
inline port<T, Direction>::~port() {
    disconnect_all();
}

template<class T, port_direction Direction>
inline constexpr typename port<T, Direction>::id_type
port<T, Direction>::id() const noexcept {
    return id_;
}

template<class T, port_direction Direction>
inline constexpr typename port<T, Direction>::node_type&
port<T, Direction>::owner() noexcept {
    return *owner_;
}

template<class T, port_direction Direction>
inline constexpr typename port<T, Direction>::node_type const&
port<T, Direction>::owner() const noexcept {
    return *owner_;
}

template<class T, port_direction Direction>
inline port_list_view<typename port<T, Direction>::opposite_port_type>
port<T, Direction>::opposites() noexcept {
    return port_list_view<opposite_port_type> { opposites_.data(), opposites_.size() };
}

template<class T, port_direction Direction>
inline port_list_view<typename port<T, Direction>::opposite_port_type const>
port<T, Direction>::opposites() const noexcept {
    return port_list_view<opposite_port_type> { opposites_.data(), opposites_.size() };
}

template<class T, port_direction Direction>
bool port<T, Direction>::connect_to(opposite_port_type& opposite) {
    bool a = do_connect(opposite);
    bool b = opposite.do_connect(*this);
    if (a != b) {
        throw std::domain_error("inconsistent connection");
    }
    return true;
}

template<class T, port_direction Direction>
bool port<T, Direction>::disconnect_from(opposite_port_type& opposite) {
    bool a = do_disconnect(opposite);
    bool b = opposite.do_disconnect(*this);
    if (a != b) {
        throw std::domain_error("inconsistent connection");
    }
    return a;
}

template<class T, port_direction Direction>
void port<T, Direction>::disconnect_all() {
    for (auto*& opposite : opposites_) {
        if (opposite != nullptr) {
            opposite->do_disconnect(*this);
            opposite = nullptr;
        }
    }
    opposites_.clear();
}

template<class T, port_direction Direction>
bool port<T, Direction>::do_connect(opposite_port_type& opposite) {
    // NOTE: multi connections
    opposites_.emplace_back(std::addressof(opposite));
    return true;
}

template<class T, port_direction Direction>
bool port<T, Direction>::do_disconnect(opposite_port_type& opposite) {
    // NOTE: removes only 1 connection
    if (auto iter = std::find(opposites_.begin(), opposites_.end(), std::addressof(opposite)); iter != opposites_.end()) {
        opposites_.erase(iter);
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
inline input_port<T>& operator<<(input_port<T>& downstream, output_port<T>& upstream) {
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
inline output_port<T>& operator>>(output_port<T>& upstream, input_port<T>& downstream) {
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
inline std::ostream& operator<<(std::ostream& out, port<T, Direction> const& value) {
    return out << Direction << "[" << value.id() << "]"
               << "@" << util::print_support { value.owner() };
}

} // namespace takatori::graph
