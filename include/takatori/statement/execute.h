#pragma once

#include <ostream>

#include "statement.h"
#include "statement_kind.h"

#include <takatori/util/clone_tag.h>
#include <takatori/plan/graph.h>

namespace takatori::statement {

/**
 * @brief execute statement.
 */
class execute final : public statement {
public:
    /// @brief the kind of this statement.
    static constexpr inline statement_kind tag = statement_kind::execute;

    /**
     * @brief creates a new object with empty execution plan.
     */
    execute() noexcept = default;

    /**
     * @brief creates a new object.
     * @param execution_plan the step execution plan
     */
    explicit execute(plan::graph_type&& execution_plan) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit execute(util::clone_tag_t, execute const& other) noexcept;

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit execute(util::clone_tag_t, execute&& other) noexcept;

    [[nodiscard]] statement_kind kind() const noexcept override;
    [[nodiscard]] execute* clone() const& override;
    [[nodiscard]] execute* clone() && override;

    /**
     * @brief returns the step execution plan to execute.
     * @return the step execution plan
     */
    [[nodiscard]] plan::graph_type& execution_plan() noexcept;

    /// @copydoc execution_plan()
    [[nodiscard]] plan::graph_type const& execution_plan() const noexcept;

    /**
     * @brief returns whether or not the two elements are same.
     * @details This only compares their identity instead of execution plan body.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(execute const& a, execute const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @details This only compares their identity instead of execution plan body.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(execute const& a, execute const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, execute const& value);

protected:
    [[nodiscard]] bool equals(statement const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    plan::graph_type execution_plan_;
};

} // namespace takatori::statement
