#pragma once

#include <ostream>

#include <takatori/util/clone_tag.h>

#include "statement.h"
#include "statement_kind.h"

namespace takatori::statement {

/**
 * @brief empty statement that does nothing.
 */
class empty final : public statement {
public:
    /// @brief the kind of this statement.
    static constexpr inline statement_kind tag { statement_kind::empty };

    /**
     * @brief creates a new instance.
     */
    explicit empty() noexcept = default;

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit empty(util::clone_tag_t, empty const& other) noexcept;

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit empty(util::clone_tag_t, empty&& other) noexcept;

    [[nodiscard]] statement_kind kind() const noexcept override;
    [[nodiscard]] empty* clone() const& override;
    [[nodiscard]] empty* clone() && override;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(empty const& a, empty const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(empty const& a, empty const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, empty const& value);

protected:
    [[nodiscard]] bool equals(statement const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;
};

} // namespace takatori::statement