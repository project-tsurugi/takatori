#pragma once

#include <memory>

#include "expression.h"
#include "expression_kind.h"

#include <takatori/scalar/expression.h>

#include <takatori/util/clone_tag.h>
#include <takatori/util/meta_type.h>
#include <takatori/util/ownership_reference.h>

namespace takatori::relation {

/**
 * @brief removes rows from relation.
 */
class filter : public expression {
public:
    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::filter;

    /**
     * @brief creates a new object.
     * @param condition the condition expression
     */
    explicit filter(
            std::unique_ptr<scalar::expression> condition) noexcept;

    /**
     * @brief creates a new object.
     * @param condition the condition expression
     * @attention this may take copies of the given expression
     */
    explicit filter(scalar::expression&& condition) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit filter(util::clone_tag_t, filter const& other);

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit filter(util::clone_tag_t, filter&& other);

    [[nodiscard]] expression_kind kind() const noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type> input_ports() noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type const> input_ports() const noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type> output_ports() noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type const> output_ports() const noexcept override;
    [[nodiscard]] filter* clone() const& override;
    [[nodiscard]] filter* clone() && override;

    /**
     * @brief returns the input port.
     * @return the input port
     */
    [[nodiscard]] input_port_type& input() noexcept;

    /// @copydoc input()
    [[nodiscard]] input_port_type const& input() const noexcept;

    /**
     * @brief returns the output port.
     * @return the output port
     */
    [[nodiscard]] output_port_type& output() noexcept;

    /// @copydoc output()
    [[nodiscard]] output_port_type const& output() const noexcept;

    /**
     * @brief returns the condition expression.
     * @return the condition expression
     * @warning undefined behavior if the expression is absent
     */
    [[nodiscard]] scalar::expression& condition() noexcept;

    /**
     * @brief returns the condition expression.
     * @return the condition expression
     * @warning undefined behavior if the expression is absent
     */
    [[nodiscard]] scalar::expression const& condition() const noexcept;

    /**
     * @brief returns the condition expression.
     * @return the condition expression
     * @return empty if the expression is absent
     */
    [[nodiscard]] util::optional_ptr<scalar::expression> optional_condition() noexcept;

    /// @copydoc optional_condition()
    [[nodiscard]] util::optional_ptr<scalar::expression const> optional_condition() const noexcept;

    /**
     * @brief releases the condition expression.
     * @return the condition expression
     * @return empty if the expression is absent
     */
    [[nodiscard]] std::unique_ptr<scalar::expression> release_condition() noexcept;

    /**
     * @brief sets the condition expression.
     * @param condition the replacement
     * @return this
     */
    filter& condition(std::unique_ptr<scalar::expression> condition) noexcept;

    /**
     * @brief returns the ownership of the condition expression.
     * @return the condition expression
     */
    [[nodiscard]] util::ownership_reference<scalar::expression> ownership_condition();

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(filter const& a, filter const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(filter const& a, filter const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, filter const& value);

protected:
    [[nodiscard]] bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    input_port_type input_;
    output_port_type output_;
    std::unique_ptr<scalar::expression> condition_;
};

/**
 * @brief type_of for where.
 */
template<> struct type_of<filter::tag> : util::meta_type<filter> {};

} // namespace takatori::relation
