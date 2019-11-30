#pragma once

#include <initializer_list>
#include <vector>
#include <utility>

#include "../expression.h"
#include "../expression_kind.h"
#include "../details/mapping_element.h"

#include "takatori/descriptor/variable.h"

#include "takatori/util/meta_type.h"
#include "takatori/util/object_creator.h"

namespace takatori::relation::intermediate {

/**
 * @brief re-defines all column identifiers in input relations.
 */
class escape : public expression {
public:
    /// @brief the escape mapping.
    using mapping = details::mapping_element;

    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::escape;

    /**
     * @brief creates a new object.
     * @param mappings the variable mappings:
     *      each source represents the input column,
     *      and destination represents the output column
     * @param creator the object creator for internal elements
     */
    explicit escape(
            std::vector<mapping, util::object_allocator<mapping>> mappings,
            util::object_creator creator = {}) noexcept;

    /**
     * @brief creates a new object.
     * @param mappings the variable mappings:
     *      each source represents the input column,
     *      and destination represents the output column
     */
    escape(std::initializer_list<mapping> mappings);

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    explicit escape(escape const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    explicit escape(escape&& other, util::object_creator creator);

    expression_kind kind() const noexcept override;
    util::sequence_view<input_port_type> input_ports() noexcept override;
    util::sequence_view<input_port_type const> input_ports() const noexcept override;
    util::sequence_view<output_port_type> output_ports() noexcept override;
    util::sequence_view<output_port_type const> output_ports() const noexcept override;
    escape* clone(util::object_creator creator) const& override;
    escape* clone(util::object_creator creator) && override;

    /**
     * @brief returns the input port.
     * @return the input port
     */
    input_port_type& input() noexcept;

    /// @copydoc input()
    input_port_type const& input() const noexcept;

    /**
     * @brief returns the output port.
     * @return the output port
     */
    output_port_type& output() noexcept;

    /// @copydoc output()
    output_port_type const& output() const noexcept;

    /**
     * @brief returns the variable mappings.
     *      each source represents the input column,
     *      and destination represents the output column
     * @return the key columns
     */
    std::vector<mapping, util::object_allocator<mapping>>& mappings() noexcept;

    /// @copydoc mappings()
    std::vector<mapping, util::object_allocator<mapping>> const& mappings() const noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(escape const& a, escape const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(escape const& a, escape const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, escape const& value);

protected:
    bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    input_port_type input_;
    output_port_type output_;
    std::vector<mapping, util::object_allocator<mapping>> mappings_;
};

} // namespace takatori::relation::intermediate

namespace takatori::relation {

/**
 * @brief type_of for escape.
 */
template<> struct type_of<intermediate::escape::tag> : util::meta_type<intermediate::escape> {};

} // namespace takatori::relation

