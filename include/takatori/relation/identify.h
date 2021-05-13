#pragma once

#include <memory>

#include "expression.h"
#include "expression_kind.h"

#include <takatori/descriptor/variable.h>
#include <takatori/type/row_id.h>

#include <takatori/util/clone_tag.h>
#include <takatori/util/meta_type.h>

namespace takatori::relation {

/**
 * @brief adds a row ID column into relations.
 */
class identify : public expression {
public:
    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::identify;

    /**
     * @brief creates a new object.
     * @param variable a descriptor of the target column variable
     * @param type the corresponding column type
     */
    explicit identify(
            descriptor::variable variable,
            std::shared_ptr<type::row_id const> type) noexcept;

    /**
     * @brief creates a new object.
     * @param variable a descriptor of the target column variable
     * @param type the corresponding column type
     * @attention this may take copies of the given type
     */
    explicit identify(
            descriptor::variable variable,
            type::row_id&& type);

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit identify(util::clone_tag_t, identify const& other);

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit identify(util::clone_tag_t, identify&& other);

    [[nodiscard]] expression_kind kind() const noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type> input_ports() noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type const> input_ports() const noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type> output_ports() noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type const> output_ports() const noexcept override;
    [[nodiscard]] identify* clone() const& override;
    [[nodiscard]] identify* clone() && override;

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
     * @brief returns the descriptor of target variable.
     * @return the target variable
     */
    [[nodiscard]] descriptor::variable& variable() noexcept;

    /// @copydoc variable()
    [[nodiscard]] descriptor::variable const& variable() const noexcept;

    /**
     * @brief returns the value type.
     * @return the value type
     * @warning undefined behavior if the type is absent
     */
    [[nodiscard]] type::row_id const& type() const noexcept;

    /**
     * @brief returns the value type.
     * @return the value type
     * @return empty if the type is absent
     */
    [[nodiscard]] util::optional_ptr<type::row_id const> optional_type() const noexcept;

    /**
     * @brief returns the value type for share its type.
     * @return the value type for sharing
     * @return empty if the type is absent
     */
    [[nodiscard]] std::shared_ptr<type::row_id const> shared_type() const noexcept;

    /**
     * @brief sets a value type.
     * @param type the value type
     * @return this
     */
    identify& type(std::shared_ptr<type::row_id const> type) noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(identify const& a, identify const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(identify const& a, identify const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, identify const& value);

protected:
    [[nodiscard]] bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    input_port_type input_;
    output_port_type output_;
    descriptor::variable variable_;
    std::shared_ptr<type::row_id const> type_;
};

/**
 * @brief type_of for where.
 */
template<> struct type_of<identify::tag> : util::meta_type<identify> {};

} // namespace takatori::relation
