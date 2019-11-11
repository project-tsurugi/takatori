#pragma once

#include <initializer_list>
#include <iostream>
#include <string>
#include <string_view>

#include "expression.h"
#include "expression_kind.h"
#include "expression_traits.h"

#include "takatori/tree/tree_element_vector.h"

#include "takatori/util/meta_type.h"
#include "takatori/util/object_creator.h"
#include "takatori/util/reference_vector.h"
#include "takatori/util/rvalue_reference_wrapper.h"

namespace takatori::scalar {

/**
 * @brief coalesce expression.
 */
class coalesce final : public expression {
public:
    /// @brief the kind of this expression.
    static constexpr inline expression_kind tag = expression_kind::coalesce;

    coalesce() = delete;
    ~coalesce() override = default;
    coalesce(coalesce const& other) = delete;
    coalesce& operator=(coalesce const& other) = delete;
    coalesce(coalesce&& other) noexcept = delete;
    coalesce& operator=(coalesce&& other) noexcept = delete;

    /**
     * @brief creates a new object.
     * @tparam Copier the object copying policy (don't care)
     * @param alternatives the alternative expressions
     */
    template<class Copier = util::clonable_copier<expression>>
    explicit coalesce(util::reference_vector<expression, Copier> alternatives) noexcept
        : alternatives_(*this, std::move(alternatives)) {}

    /**
     * @brief creates a new object.
     * @param alternatives the alternative expressions
     * @attention this may take copies of given expressions
     */
    coalesce(std::initializer_list<util::rvalue_reference_wrapper<expression>> alternatives = {}); // NOLINT

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    coalesce(coalesce const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    coalesce(coalesce&& other, util::object_creator creator);

    parent_type* parent_element() noexcept override;
    parent_type const* parent_element() const noexcept override;
    void parent_element(parent_type* parent) noexcept override;

    expression_kind kind() const noexcept override;
    coalesce* clone(util::object_creator creator) const& override;
    coalesce* clone(util::object_creator creator) && override;

    /**
     * @brief returns the alternative expressions.
     * @return the alternative expressions
     */
    tree::tree_element_vector<expression>& alternatives() noexcept;

    /// @copydoc alternatives()
    tree::tree_element_vector<expression> const& alternatives() const noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(coalesce const& a, coalesce const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend inline bool operator!=(coalesce const& a, coalesce const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, coalesce const& value);

protected:
    bool equals(expression const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    tree::tree_element_vector<expression> alternatives_;
    parent_type* parent_ {};
};

/**
 * @brief expression_kind_type for coalesce.
 */
template<> struct expression_kind_type<coalesce::tag> : util::meta_type<coalesce> {};

} // namespace takatori::scalar
