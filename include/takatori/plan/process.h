#pragma once

#include <memory>
#include <ostream>
#include <utility>
#include <vector>

#include "step.h"
#include "step_kind.h"

#include <takatori/relation/expression.h>

#include <takatori/graph/graph.h>

#include <takatori/util/clone_tag.h>
#include <takatori/util/reference_vector.h>

namespace takatori::plan {

class exchange;

/**
 * @brief evaluates relational operator graph.
 */
class process : public step {
public:
    /// @brief the kind of this step.
    static constexpr step_kind tag = step_kind::process;

    /// @brief the adjacent step type.
    using adjacent_type = exchange;
    
    ~process() override;
    process(process const& other) = delete;
    process& operator=(process const& other) = delete;
    process(process&& other) noexcept = delete;
    process& operator=(process&& other) noexcept = delete;

    /**
     * @brief creates a new object with empty relational operator graph.
     */
    process() = default;

    /**
     * @brief creates a new object.
     * @param operators the relational operator graph
     */
    explicit process(graph::graph<relation::expression> operators) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit process(util::clone_tag_t, process const& other);

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit process(util::clone_tag_t, process&& other);

    step_kind kind() const noexcept override;
    process* clone() const& override;
    process* clone() && override;

    /**
     * @brief returns the relational operator graph of this process.
     * @return the relational operator graph
     */
    [[nodiscard]] graph::graph<relation::expression>& operators() noexcept;

    /// @copydoc operators()
    [[nodiscard]] graph::graph<relation::expression> const& operators() const noexcept;

    /**
     * @brief returns the view of upstream exchanges.
     * @details this process consumes input data from these exchanges.
     * @attention this will be invalidated after the upstream connections were changed
     * @return the view of upstream exchanges
     */
    [[nodiscard]] step_list_view<exchange> upstreams() noexcept;
    
    /// @copydoc upstreams()
    [[nodiscard]] step_list_view<exchange const> upstreams() const noexcept;

    /**
     * @brief returns the view of downstream exchanges.
     * @details this process produces output into these exchanges.
     * @attention this will be invalidated after the downstream connections were changed
     * @return the view of downstream exchanges
     */
    [[nodiscard]] step_list_view<exchange> downstreams() noexcept;

    /// @copydoc downstreams()
    [[nodiscard]] step_list_view<exchange const> downstreams() const noexcept;
    
    /**
     * @brief returns whether or not this obtains input data from the given exchange.
     * @param upstream the target exchange
     * @return true if the given exchange is an upstream of this
     * @return false otherwise
     */
    [[nodiscard]] bool has_upstream(exchange const& upstream) const noexcept;

    /**
     * @brief adds a process input.
     * @details if the given exchange is already an upstream of this process, this operation does nothing.
     * @attention this operation may invalidate the result of upstreams(), and upstreams' downstreams().
     * @param upstream the upstream exchange
     * @return this
     */
    process& add_upstream(exchange& upstream);

    /**
     * @brief removes a process input.
     * @details if the given exchange is not an upstream of this process, this operation does nothing.
     * @attention this operation may invalidate the result of upstreams(), and upstreams' downstreams().
     * @param upstream the upstream exchange
     * @return this
     */
    process& remove_upstream(exchange& upstream);

    /**
     * @brief returns whether or not this offers output data into the given exchange.
     * @param downstream the target exchange
     * @return true if the given exchange is a downstream of this
     * @return false otherwise
     */
    [[nodiscard]] bool has_downstream(exchange const& downstream) const noexcept;

    /**
     * @brief adds a process output.
     * @details if the given exchange is already a downstream of this process, this operation does nothing.
     * @attention this operation may invalidate the result of downstreams(), and downstreams' upstreams()
     * @param downstream the downstream exchange
     * @return this
     */
    process& add_downstream(exchange& downstream);

    /**
     * @brief removes a process output.
     * @details if the given exchange is not a downstream of this process, this operation does nothing.
     * @attention this operation may invalidate the result of downstreams(), and downstreams' upstreams()
     * @param downstream the downstream exchange
     * @return this
     */
    process& remove_downstream(exchange& downstream);

    /**
     * @brief adds an upstream exchange to the process.
     * @param downstream the process
     * @param upstream the upstream exchange
     * @return the process
     */
    friend process& operator<<(process& downstream, exchange& upstream);

    /**
     * @brief adds a downstream exchange to the process.
     * @param upstream the process
     * @param downstream the downstream exchange
     * @return the process
     */
    friend process& operator>>(process& upstream, exchange& downstream);

    /**
     * @brief returns whether or not the two elements are identical.
     * @details This just compares each identity.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(process const& a, process const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @details This just compares each identity.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(process const& a, process const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, process const& value);

protected:
    [[nodiscard]] bool equals(step const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    graph::graph<relation::expression> operators_ {};
    std::vector<exchange*> upstreams_ {};
    std::vector<exchange*> downstreams_ {};

    void internal_add_upstream(exchange& upstream);
    void internal_remove_upstream(exchange& upstream) noexcept;
    void internal_add_downstream(exchange& downstream);
    void internal_remove_downstream(exchange& downstream) noexcept;

    friend class exchange;
};

} // namespace takatori::plan
