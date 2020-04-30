#pragma once

#include <iostream>
#include <utility>
#include <vector>

#include "exchange.h"
#include "step_kind.h"

#include <takatori/util/object_creator.h>

namespace takatori::plan {

/**
 * @brief exchange nothing.
 */
class discard : public exchange {
public:
    /// @brief the kind of this step.
    static constexpr step_kind tag = step_kind::discard;

    /**
     * @brief creates a new object.
     */
    discard() = default;

    /**
     * @brief creates a new object.
     * @param creator the object creator for internal elements
     */
    explicit discard(util::object_creator creator) noexcept;

    /**
     * @brief creates a new object.
     * @details the created object does not have upstream processes, nor downstream processes.
     * @param other the copy source
     * @param creator the object creator
     */
    explicit discard(discard const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @details the created object does not have upstream processes, nor downstream processes.
     * @param other the move source
     * @param creator the object creator
     */
    explicit discard(discard&& other, util::object_creator creator);

    [[nodiscard]] step_kind kind() const noexcept override;
    [[nodiscard]] discard* clone(util::object_creator creator) const& override;
    [[nodiscard]] discard* clone(util::object_creator creator) && override;

    [[nodiscard]] util::sequence_view<descriptor::variable const> input_columns() const noexcept override;
    [[nodiscard]] util::sequence_view<descriptor::variable const> output_columns() const noexcept override;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @details this don't compares upstream processes nor downstream processes.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(discard const& a, discard const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @details this don't compares upstream processes nor downstream processes.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(discard const& a, discard const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, discard const& value);

protected:
    [[nodiscard]] bool equals(step const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;
};

} // namespace takatori::plan
