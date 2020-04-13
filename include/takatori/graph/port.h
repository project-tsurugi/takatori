#pragma once

#include <algorithm>
#include <utility>

#include "port_direction.h"

#include <takatori/util/object_creator.h>
#include <takatori/util/optional_ptr.h>
#include <takatori/util/print_support.h>

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
 * @brief represents an input/output port of graph nodes, which only can have upto one opposite.
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
    using opposite_type = port<T, ~direction>;

    /// @brief the port index type.
    using index_type = std::size_t;

    /**
     * @brief constructs a new object.
     * @param owner the owner node
     * @param index the index where this port on the container
     * @warning don't invoke this outside of owner node
     */
    explicit constexpr port(node_type& owner, index_type index = 0) noexcept
        : index_(index)
        , owner_(std::addressof(owner))
    {}

    /**
     * @brief constructs a new object.
     * @param owner the owner node
     * @param index the index where this port on the container
     * @param creator the object creator (ignored)
     * @warning don't invoke this outside of owner node
     * @note This is designed to keep operational compatibility with multiport
     */
    explicit constexpr port(node_type& owner, index_type index, [[maybe_unused]] util::object_creator creator) noexcept
        : index_(index)
        , owner_(std::addressof(owner))
    {}

    ~port() {
        internal_disconnect();
    }

    port(port const& other) = delete;
    port& operator=(port const& other) = delete;

    /**
     * @brief constructs a new object.
     * @param other the move source
     * @warning this is designed for STL like containers, developers should not use this directly
     */
    constexpr port(port&& other) noexcept
        : index_(other.index_)
        , owner_(other.owner_)
        , opposite_(other.opposite_)
    {
        other.opposite_ = nullptr;
        if (opposite_ != nullptr) {
            opposite_->internal_reconnect(other, *this);
        }
    }

    /**
     * @brief assigns the given object.
     * @param other the move source
     * @return this
     * @warning this is designed for STL like containers, developers should not use this directly
     */
    constexpr port& operator=(port&& other) noexcept {
        // NOTE: std::exchange() is not constexpr until C++20
        internal_disconnect();
        index_ = other.index_;
        owner_ = other.owner_;
        opposite_ = other.opposite_;
        other.owner_ = nullptr;
        other.opposite_ = nullptr;
        if (opposite_ != nullptr) {
            opposite_->internal_reconnect(other, *this);
        }
        return *this;
    }

    /**
     * @brief returns the index where this port on the parent container.
     * @return the port index (0-origin).
     */
    [[nodiscard]] constexpr index_type index() const noexcept {
        return index_;
    }

    /**
     * @brief returns the node which owns this port.
     * @return the owner
     */
    constexpr node_type& owner() noexcept {
        return *owner_;
    }

    /// @copydoc owner()
    [[nodiscard]] constexpr node_type const& owner() const noexcept {
        return *owner_;
    }

    /**
     * @brief returns the opposite port.
     * @return the opposite port
     * @return empty if this port is not connected
     */
    constexpr util::optional_ptr<opposite_type> opposite() noexcept {
        return util::optional_ptr { opposite_ };
    }

    /// @copydoc opposite()
    [[nodiscard]] constexpr util::optional_ptr<opposite_type const> opposite() const noexcept {
        return util::optional_ptr { opposite_ };
    }

    /**
     * @brief returns whether or not the target port is connected to this.
     * @param port the target port
     * @return true if it is connected to this
     * @return false otherwise
     */
    [[nodiscard]] constexpr bool is_connected(opposite_type const& port) const noexcept {
        return opposite_ == std::addressof(port);
    }

    /**
     * @brief connects to a opposite port.
     * @param opposite the opposite port
     * @return true if successfully connected
     * @return false if this and the given port are already connected to each other
     * @throw std::domain_error if this or the opposite port already connected to other ports
     */
    constexpr bool connect_to(opposite_type& opposite) {
        auto* that = std::addressof(opposite);
        if (this->opposite_ == nullptr && that->opposite_ == nullptr) {
            this->opposite_ = that;
            that->opposite_ = this;
            return true;
        }
        if (this->opposite_ == that && that->opposite_ == this) {
            return false;
        }
        throw std::domain_error("already connected");
    }

    /**
     * @brief disconnects from the opposite port.
     * @param opposite the opposite port
     * @return true if successfully connected
     * @return false otherwise, may by not yet connected
     */
    constexpr bool disconnect_from(opposite_type& opposite) {
        if (this->opposite_ == nullptr) {
            return false;
        }
        auto* that = std::addressof(opposite);
        if (this->opposite_ == that && that->opposite_ == this) {
            this->opposite_ = nullptr;
            that->opposite_ = nullptr;
            return true;
        }
        throw std::domain_error("inconsistent connection");
    }

    /**
     * @brief disconnects from the any opposite ports.
     */
    constexpr void disconnect_all() {
        if (opposite_ != nullptr) {
            disconnect_from(*opposite_);
        }
    }

private:
    index_type index_;
    node_type* owner_;
    opposite_type* opposite_ {};

    constexpr void internal_reconnect(opposite_type& old_opposite, opposite_type& new_opposite) noexcept {
        if (opposite_ == std::addressof(old_opposite)) {
            opposite_ = std::addressof(new_opposite);
        }
        // broken
    }

    constexpr void internal_disconnect() noexcept {
        if (opposite_ != nullptr) {
            if (opposite_->opposite_ == this) {
                opposite_->opposite_ = nullptr;
            }
            opposite_ = nullptr;
        }
    }

    friend class port<node_type, ~direction>;
};

/**
 * @brief returns whether or not the downstream port is connected from the upstream one.
 * @tparam T the node type
 * @param downstream the downstream port
 * @param upstream the upstream port
 * @return true if they are connected
 * @return false otherwise
 */
template<class T>
[[nodiscard]] inline constexpr bool operator<(input_port<T> const& downstream, output_port<T> const& upstream) noexcept {
    return downstream.is_connected(upstream);
}

/**
 * @brief returns whether or not the upstream port is connected to the downstream one.
 * @tparam T the node type
 * @param downstream the downstream port
 * @param upstream the upstream port
 * @return true if they are connected
 * @return false otherwise
 */
template<class T>
[[nodiscard]] inline constexpr bool operator>(output_port<T> const& upstream, input_port<T> const& downstream) noexcept {
    return downstream < upstream;
}

/**
 * @brief connects downstream port from upstream.
 * @tparam T the node type
 * @param downstream the downstream port
 * @param upstream the upstream port
 * @return the downstream port
 */
template<class T>
inline constexpr input_port<T>& operator<<(input_port<T>& downstream, output_port<T>& upstream) {
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
inline constexpr output_port<T>& operator>>(output_port<T>& upstream, input_port<T>& downstream) {
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
    return out << Direction << "[" << value.index() << "]"
               << "@" << util::print_support { value.owner() };
}

} // namespace takatori::graph
