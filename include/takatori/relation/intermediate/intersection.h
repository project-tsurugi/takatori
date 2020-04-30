#pragma once

#include <array>
#include <initializer_list>
#include <vector>
#include <utility>

#include "../expression.h"
#include "../expression_kind.h"
#include "../set_quantifier.h"
#include "../details/key_pair_element.h"

#include <takatori/descriptor/variable.h>

#include <takatori/util/meta_type.h>
#include <takatori/util/object_creator.h>

namespace takatori::relation::intermediate {

/**
 * @brief obtains a multi-set intersection of two input relations.
 */
class intersection : public expression {
public:
    /// @brief the key pair type
    using group_key_pair = details::key_pair_element;

    /// @brief the set quantifier type.
    using quantifier_kind = set_quantifier;

    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::intersection_relation;

    /**
     * @brief creates a new object.
     * @param quantifier the set quantifier of this operation
     * @param group_key_pairs the key element pairs:
     *      this operation distinguishes individual rows by these keys
     * @param creator the object creator for internal elements
     */
    explicit intersection(
            quantifier_kind quantifier,
            std::vector<group_key_pair, util::object_allocator<group_key_pair>> group_key_pairs,
            util::object_creator creator = {}) noexcept;

    /**
     * @brief creates a new object.
     * @param group_key_pairs the key element pairs:
     *      this operation distinguishes individual rows by these keys
     * @param quantifier the set quantifier of this operation
     */
    intersection(
            std::initializer_list<group_key_pair> group_key_pairs,
            quantifier_kind quantifier = quantifier_kind::all);

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    explicit intersection(intersection const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    explicit intersection(intersection&& other, util::object_creator creator);

    [[nodiscard]] expression_kind kind() const noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type> input_ports() noexcept override;
    [[nodiscard]] util::sequence_view<input_port_type const> input_ports() const noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type> output_ports() noexcept override;
    [[nodiscard]] util::sequence_view<output_port_type const> output_ports() const noexcept override;
    [[nodiscard]] intersection* clone(util::object_creator creator) const& override;
    [[nodiscard]] intersection* clone(util::object_creator creator) && override;

    /**
     * @brief returns the left input port.
     * @return the left input port
     */
    [[nodiscard]] input_port_type& left() noexcept;

    /// @copydoc left()
    [[nodiscard]] input_port_type const& left() const noexcept;

    /**
     * @brief returns the right input port.
     * @return the right input port
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
    intersection& quantifier(quantifier_kind quantifier) noexcept;

    /**
     * @brief returns the key columns on the input relations.
     * @return the key columns
     */
    [[nodiscard]] std::vector<group_key_pair, util::object_allocator<group_key_pair>>& group_key_pairs() noexcept;

    /// @copydoc group_key_pairs()
    [[nodiscard]] std::vector<group_key_pair, util::object_allocator<group_key_pair>> const& group_key_pairs() const noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(intersection const& a, intersection const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(intersection const& a, intersection const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, intersection const& value);

protected:
    [[nodiscard]] bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    std::array<input_port_type, 2> inputs_;
    output_port_type output_;
    quantifier_kind quantifier_;
    std::vector<group_key_pair, util::object_allocator<group_key_pair>> group_key_pairs_;

    static inline constexpr std::size_t left_index = 0;
    static inline constexpr std::size_t right_index = left_index + 1;
};

} // namespace takatori::relation::intermediate

namespace takatori::relation {

/**
 * @brief type_of for intersect.
 */
template<> struct type_of<intermediate::intersection::tag> : util::meta_type<intermediate::intersection> {};

} // namespace takatori::relation

