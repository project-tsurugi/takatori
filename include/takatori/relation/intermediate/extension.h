#pragma once

#include "../expression.h"
#include "../expression_kind.h"

namespace takatori::relation::intermediate {

/**
 * @brief an extension point for relation expression models.
 */
class extension : public expression {
public:
    /// @brief the kind of this expression.
    static constexpr expression_kind tag = expression_kind::extension;

    /// @brief the extension ID type.
    using extension_id_type = std::size_t;

    /// @brief the minimum extension ID for third party extensions.
    static constexpr extension_id_type minimum_user_extension_id = 10'000;

    [[nodiscard]] expression_kind kind() const noexcept final;

    [[nodiscard]] util::sequence_view<input_port_type> input_ports() noexcept override = 0;
    [[nodiscard]] util::sequence_view<input_port_type const> input_ports() const noexcept override = 0;
    [[nodiscard]] util::sequence_view<output_port_type> output_ports() noexcept override = 0;
    [[nodiscard]] util::sequence_view<output_port_type const> output_ports() const noexcept override = 0;
    [[nodiscard]] extension* clone() const& override = 0;
    [[nodiscard]] extension* clone() && override = 0;

    /**
     * @brief returns the extension ID of this type.
     * @return the extension ID
     */
    [[nodiscard]] virtual extension_id_type extension_id() const noexcept = 0;

    /**
     * @brief returns whether the two elements are equivalent.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(extension const& a, extension const& b) noexcept;

    /**
     * @brief returns whether the two elements are different.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(extension const& a, extension const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, extension const& value);

protected:
    [[nodiscard]] virtual bool equals(extension const& other) const noexcept = 0;
    [[nodiscard]] bool equals(expression const& other) const noexcept final;
    std::ostream& print_to(std::ostream& out) const override = 0;
};

} // namespace takatori::relation::intermediate

namespace takatori::relation {

/**
 * @brief type_of for extension.
 */
template<> struct type_of<intermediate::extension::tag> : util::meta_type<intermediate::extension> {};

} // namespace takatori::relation

