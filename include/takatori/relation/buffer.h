#pragma once

#include <memory>

#include "expression.h"
#include "expression_kind.h"

#include <boost/container/small_vector.hpp>

#include <takatori/scalar/expression.h>

#include <takatori/util/clone_tag.h>
#include <takatori/util/meta_type.h>

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
     */
    explicit buffer(size_type size);

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit buffer(util::clone_tag_t, buffer const& other);

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit buffer(util::clone_tag_t, buffer&& other);

    [[nodiscard]] expression_kind kind() const noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type> input_ports() noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type const> input_ports() const noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type> output_ports() noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type const> output_ports() const noexcept override;
    [[nodiscard]] buffer* clone() const& override;
    [[nodiscard]] buffer* clone() && override;

    /**
     * @brief returns the input port.
     * @return the input port
     */
    [[nodiscard]] input_port_type& input() noexcept;

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
    boost::container::small_vector<output_port_type, 4> outputs_;
};

/**
 * @brief type_of for buffer.
 */
template<> struct type_of<buffer::tag> : util::meta_type<buffer> {};

} // namespace takatori::relation
