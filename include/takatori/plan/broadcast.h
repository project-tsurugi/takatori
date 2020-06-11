#pragma once

#include <initializer_list>
#include <ostream>
#include <optional>
#include <utility>
#include <vector>

#include "exchange.h"
#include "step_kind.h"

#include <takatori/util/object_creator.h>

namespace takatori::plan {

/**
 * @brief exchange just the input data.
 */
class broadcast : public exchange {
public:
    /// @brief the kind of this step.
    static constexpr step_kind tag = step_kind::broadcast;

    /**
     * @brief creates a new object.
     * @note the created object does not have any exchange columns, please define them using columns()
     */
    broadcast() = default;

    /**
     * @brief creates a new object.
     * @param creator the object creator for internal elements
     * @note the created object does not have any exchange columns, please define them using columns()
     */
    explicit broadcast(util::object_creator creator) noexcept;

    /**
     * @brief creates a new object.
     * @param columns the exchange columns
     * @param creator the object creator for internal elements
     */
    explicit broadcast(
            std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> columns,
            util::object_creator creator = {}) noexcept;

    /**
     * @brief creates a new object.
     * @param columns the exchange columns
     */
    broadcast(std::initializer_list<descriptor::variable> columns);

    /**
     * @brief creates a new object.
     * @details the created object does not have upstream processes, nor downstream processes.
     * @param other the copy source
     * @param creator the object creator
     */
    explicit broadcast(broadcast const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @details the created object does not have upstream processes, nor downstream processes.
     * @param other the move source
     * @param creator the object creator
     */
    explicit broadcast(broadcast&& other, util::object_creator creator);

    [[nodiscard]] step_kind kind() const noexcept override;
    [[nodiscard]] broadcast* clone(util::object_creator creator) const& override;
    [[nodiscard]] broadcast* clone(util::object_creator creator) && override;

    [[nodiscard]] util::sequence_view<descriptor::variable const> input_columns() const noexcept override;
    [[nodiscard]] util::sequence_view<descriptor::variable const> output_columns() const noexcept override;

    /**
     * @brief returns the columns to exchange.
     * @return the columns to exchange
     */
    [[nodiscard]] std::vector<descriptor::variable, util::object_allocator<descriptor::variable>>& columns() noexcept;

    /// @copydoc columns()
    [[nodiscard]] std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> const& columns() const noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @details this don't compares upstream processes nor downstream processes.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(broadcast const& a, broadcast const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @details this don't compares upstream processes nor downstream processes.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(broadcast const& a, broadcast const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, broadcast const& value);

protected:
    [[nodiscard]] bool equals(step const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    std::vector<descriptor::variable, util::object_allocator<descriptor::variable>> columns_ {};
};

} // namespace takatori::plan
