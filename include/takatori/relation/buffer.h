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
 * @details This has the "primary" output (see primary_output()), and the others are called the "secondary" outputs.
 *      By default, every output receives all rows of the input relation.
 *      If stop_with_primary() is enabled, the secondary outputs need only the rows
 *      which the downstream of the primary output requires.
 * @see primary_output()
 * @see stop_with_primary()
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
     * @param stop_with_primary whether or not this can stop when the primary output stops
     * @throws std::invalid_argument if the number of output targets is 0
     * @see stop_with_primary()
     */
    explicit buffer(size_type size, bool stop_with_primary = false);

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
     * @brief returns the primary output port.
     * @details The primary output is always the first output port (same as `output_ports()[0]`),
     *      and the others are the secondary outputs.
     *      The primary output is only meaningful if stop_with_primary() is enabled;
     *      otherwise, every output receives all rows of the input relation.
     * @return the primary output port
     * @see stop_with_primary()
     */
    [[nodiscard]] output_port_type& primary_output() noexcept;

    /// @copydoc primary_output()
    [[nodiscard]] output_port_type const& primary_output() const noexcept;

    /**
     * @brief returns whether or not this can stop when the primary output stops.
     * @details If this is enabled, this operator may stop processing the whole input,
     *      including passing rows to the secondary outputs,
     *      when the downstream of the primary output no longer requires any rows (e.g. early stop by row count).
     *      That is, the secondary outputs need only the rows which the downstream of the primary output requires,
     *      and the result does not change even if the secondary outputs receive fewer rows.
     *      For the same reason, the input relation may be narrowed to the rows
     *      which the downstream of the primary output requires.
     *      Note that, it is still correct to pass all rows to every output even if this is enabled.
     * @return true if this can stop when the primary output stops
     * @return false if every output always receives all rows of the input (default)
     * @see primary_output()
     */
    [[nodiscard]] bool stop_with_primary() const noexcept;

    /**
     * @brief sets whether or not this can stop when the primary output stops.
     * @param enabled whether or not this can stop when the primary output stops
     * @return this
     * @see stop_with_primary()
     */
    buffer& stop_with_primary(bool enabled) noexcept;

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
    bool stop_with_primary_;
};

/**
 * @brief type_of for buffer.
 */
template<> struct type_of<buffer::tag> : util::meta_type<buffer> {};

} // namespace takatori::relation
