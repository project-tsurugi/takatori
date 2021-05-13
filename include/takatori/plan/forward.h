#pragma once

#include <memory>
#include <initializer_list>
#include <ostream>
#include <optional>
#include <utility>
#include <vector>

#include "exchange.h"
#include "step_kind.h"

#include <takatori/util/clone_tag.h>

namespace takatori::plan {

/**
 * @brief exchange just the input data.
 */
class forward : public exchange {
public:
    /// @brief the size type.
    using size_type = std::size_t;

    /// @brief the kind of this step.
    static constexpr step_kind tag = step_kind::forward;

    /**
     * @brief creates a new object.
     * @note the created object does not have any exchange columns, please define them using columns()
     */
    forward() = default;

    /**
     * @brief creates a new object.
     * @param limit the max number of rows to forward
     * @note the created object does not have any exchange columns, please define them using columns()
     */
    explicit forward(
            std::optional<size_type> limit) noexcept;

    /**
     * @brief creates a new object.
     * @param columns the exchange columns
     * @param limit the max number of rows to forward
     */
    explicit forward(
            std::vector<descriptor::variable> columns,
            std::optional<size_type> limit = std::nullopt) noexcept;

    /**
     * @brief creates a new object.
     * @param columns the exchange columns
     * @param limit the max number of rows to forward
     */
    forward(
            std::initializer_list<descriptor::variable> columns,
            std::optional<size_type> limit = std::nullopt) noexcept;

    /**
     * @brief creates a new object.
     * @details the created object does not have upstream processes, nor downstream processes.
     * @param other the copy source
     */
    explicit forward(util::clone_tag_t, forward const& other);

    /**
     * @brief creates a new object.
     * @details the created object does not have upstream processes, nor downstream processes.
     * @param other the move source
     */
    explicit forward(util::clone_tag_t, forward&& other);

    [[nodiscard]] step_kind kind() const noexcept override;
    [[nodiscard]] forward* clone() const& override;
    [[nodiscard]] forward* clone() && override;

    [[nodiscard]] util::sequence_view<descriptor::variable const> input_columns() const noexcept override;
    [[nodiscard]] util::sequence_view<descriptor::variable const> output_columns() const noexcept override;

    /**
     * @brief returns the columns to exchange.
     * @return the columns to exchange
     */
    [[nodiscard]] std::vector<descriptor::variable>& columns() noexcept;

    /// @copydoc columns()
    [[nodiscard]] std::vector<descriptor::variable> const& columns() const noexcept;

    /**
     * @brief returns the max number of exchange rows.
     * @return the number of exchange rows limit
     * @return empty if it is unlimited
     */
    [[nodiscard]] std::optional<size_type> const& limit() const noexcept;

    /**
     * @brief sets the max number of exchange rows.
     * @param limit the number of exchange rows limit
     * @return this
     */
    forward& limit(std::optional<size_type> limit) noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @details this don't compares upstream processes nor downstream processes.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(forward const& a, forward const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @details this don't compares upstream processes nor downstream processes.
     * @details This operation does not consider which the input/output ports are connected to.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(forward const& a, forward const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, forward const& value);

protected:
    [[nodiscard]] bool equals(step const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    std::vector<descriptor::variable> columns_ {};
    std::optional<size_type> limit_ {};
};

} // namespace takatori::plan
