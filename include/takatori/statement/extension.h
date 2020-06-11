#pragma once

#include <ostream>

#include "statement.h"
#include "statement_kind.h"

#include <takatori/util/meta_type.h>

namespace takatori::statement {

/**
 * @brief an extension point for scalar statement models.
 */
class extension : public statement {
public:
    /// @brief the extension ID type.
    using extension_id_type = std::size_t;

    /// @brief the minimum extension ID for third party extensions.
    static constexpr extension_id_type minimum_user_extension_id = 10'000;

    /// @brief the kind of this statement.
    static constexpr inline statement_kind tag = statement_kind::extension;

    [[nodiscard]] statement_kind kind() const noexcept final;
    [[nodiscard]] extension* clone(util::object_creator creator) const& override = 0;
    [[nodiscard]] extension* clone(util::object_creator creator) && override = 0;

    /**
     * @brief returns the extension ID of this type.
     * @return the extension ID
     */
    [[nodiscard]] virtual extension_id_type extension_id() const noexcept = 0;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(extension const& a, extension const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
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
    /**
     * @brief returns whether or not this extension is equivalent to the target one.
     * @param other the target extension
     * @return true if the both are equivalent
     * @return false otherwise
     */
    [[nodiscard]] virtual bool equals(extension const& other) const noexcept = 0;
    [[nodiscard]] bool equals(statement const& other) const noexcept final;
    std::ostream& print_to(std::ostream& out) const override = 0;
};

} // namespace takatori::statement
