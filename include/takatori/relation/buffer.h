#pragma once

#include "expression.h"
#include "expression_kind.h"

#include <boost/container/small_vector.hpp>

#include <takatori/scalar/expression.h>

#include <takatori/util/meta_type.h>
#include <takatori/util/object_creator.h>

namespace takatori::relation {

/**
 * @brief pass the input relation into the multiple successors.
 */
class buffer : public expression {
public:
    /// @brief the size type.
    using size_type = std::size_t;

    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::buffer;

    /**
     * @brief creates a new object.
     * @param size the number of output targets
     * @param creator the object creator for internal elements
     */
    explicit buffer(size_type size, util::object_creator creator = {}) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    explicit buffer(buffer const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    explicit buffer(buffer&& other, util::object_creator creator);

    [[nodiscard]] expression_kind kind() const noexcept override;
    util::sequence_view<input_port_type> input_ports() noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type const> input_ports() const noexcept override;
    util::sequence_view<output_port_type> output_ports() noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type const> output_ports() const noexcept override;
    [[nodiscard]] buffer* clone(util::object_creator creator) const& override;
    buffer* clone(util::object_creator creator) && override;

    /**
     * @brief returns the input port.
     * @return the input port
     */
    input_port_type& input() noexcept;

    /// @copydoc input()
    [[nodiscard]] input_port_type const& input() const noexcept;

    /**
     * @brief returns the number of output targets in this buffer.
     * @return the number of output targets
     */
    [[nodiscard]] size_type size() const noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(buffer const& a, buffer const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(buffer const& a, buffer const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, buffer const& value);

protected:
    [[nodiscard]] bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    input_port_type input_;
    boost::container::small_vector<output_port_type, 4, util::object_allocator<output_port_type>> outputs_;
};

/**
 * @brief type_of for buffer.
 */
template<> struct type_of<buffer::tag> : util::meta_type<buffer> {};

} // namespace takatori::relation
