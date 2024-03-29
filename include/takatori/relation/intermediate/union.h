#pragma once

#include <array>
#include <initializer_list>
#include <memory>
#include <vector>
#include <utility>

#include "../expression.h"
#include "../expression_kind.h"
#include "../set_quantifier.h"
#include "../details/union_element.h"

#include <takatori/descriptor/variable.h>

#include <takatori/util/clone_tag.h>
#include <takatori/util/meta_type.h>

namespace takatori::relation::intermediate {

/**
 * @brief obtains a multi-set union between two input relations.
 */
class union_ : public expression {
public:
    /// @brief the column mapping from the right relation into left relation.
    using mapping = details::union_element;

    /// @brief the set quantifier type.
    using quantifier_kind = set_quantifier;

    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::union_relation;

    /**
     * @brief creates a new object.
     * @param quantifier the set quantifier of this operation
     * @param mappings the column mappings on input/output relations:
     *      the succeeding operators only can refer the destination columns declared in here.
     *      If `quantifier=distinct`, each input column must not be absent
     */
    explicit union_(
            quantifier_kind quantifier,
            std::vector<mapping> mappings) noexcept;

    /**
     * @brief creates a new object.
     * @param mappings the column mappings on input/output relations:
     *      the succeeding operators only can refer the destination columns declared in here.
     *      If `quantifier=distinct`, each input column must not be absent
     * @param quantifier the set quantifier of this operation
     */
    union_(
            std::initializer_list<mapping> mappings,
            quantifier_kind quantifier = quantifier_kind::all);

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit union_(util::clone_tag_t, union_ const& other);

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit union_(util::clone_tag_t, union_&& other);

    [[nodiscard]] expression_kind kind() const noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type> input_ports() noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type const> input_ports() const noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type> output_ports() noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type const> output_ports() const noexcept override;
    [[nodiscard]] union_* clone() const& override;
    [[nodiscard]] union_* clone() && override;

    /**
     * @brief returns the first input port.
     * @return the first input port
     */
    [[nodiscard]] input_port_type& left() noexcept;

    /// @copydoc left()
    [[nodiscard]] input_port_type const& left() const noexcept;

    /**
     * @brief returns the second input port.
     * @return the second input port
     */
    [[nodiscard]] input_port_type& right() noexcept;

    /// @copydoc right()
    [[nodiscard]] input_port_type const& right() const noexcept;

    /**
     * @brief returns the output port.
     * @return the output port
     */
    [[nodiscard]] output_port_type& output() noexcept;

    /// @copydoc output()
    [[nodiscard]] output_port_type const& output() const noexcept;

    /**
     * @brief returns the set quantifier kind of this operation.
     * @return the set quantifier kind
     */
    [[nodiscard]] quantifier_kind quantifier() const noexcept;

    /**
     * @brief sets the set quantifier kind of this operation.
     * @param quantifier the quantifier kind
     * @return this
     */
    union_& quantifier(quantifier_kind quantifier) noexcept;

    /**
     * @brief returns the column mappings on input/output relations:
     *      the succeeding operators only can refer the destination columns declared in here
     * @return the column mappings
     */
    [[nodiscard]] std::vector<mapping>& mappings() noexcept;

    /// @copydoc mappings()
    [[nodiscard]] std::vector<mapping> const& mappings() const noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(union_ const& a, union_ const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(union_ const& a, union_ const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, union_ const& value);

protected:
    [[nodiscard]] bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    std::array<input_port_type, 2> inputs_;
    output_port_type output_;
    quantifier_kind quantifier_;
    std::vector<mapping> mappings_;

    static inline constexpr std::size_t left_index = 0;
    static inline constexpr std::size_t right_index = left_index + 1;
};

} // namespace takatori::relation::intermediate

namespace takatori::relation {

/**
 * @brief type_of for union_.
 */
template<> struct type_of<intermediate::union_::tag> : util::meta_type<intermediate::union_> {};

} // namespace takatori::relation

