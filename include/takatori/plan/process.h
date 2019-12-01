#pragma once

#include <iostream>
#include <utility>
#include <vector>

#include "step.h"
#include "step_kind.h"

#include "takatori/relation/expression.h"

#include "takatori/graph/graph.h"

#include "takatori/util/object_creator.h"
#include "takatori/util/reference_vector.h"

namespace takatori::plan {

class exchange;

/**
 * @brief evaluates relational operator graph.
 */
class process : public step {
public:
    /// @brief the kind of this step.
    static constexpr step_kind tag = step_kind::process;
    
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
     * @brief creates a new object with empty relational operator graph.
     * @param creator the object creator for internal elements
     */
    explicit process(util::object_creator creator) noexcept;

    /**
     * @brief creates a new object.
     * @param operators the relational operator graph
     * @param creator the object creator for internal elements
     */
    explicit process(
            graph::graph<relation::expression> operators,
            util::object_creator creator = {}) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy source
     * @param creator the object creator
     */
    explicit process(process const& other, util::object_creator creator);

    /**
     * @brief creates a new object.
     * @param other the move source
     * @param creator the object creator
     */
    explicit process(process&& other, util::object_creator creator);

    step_kind kind() const noexcept override;
    process* clone(util::object_creator creator) const& override;
    process* clone(util::object_creator creator) && override;

    /**
     * @brief returns the relational operator graph of this process.
     * @return the relational operator graph
     */
    graph::graph<relation::expression>& operators() noexcept;

    /// @copydoc operators()
    graph::graph<relation::expression> const& operators() const noexcept;

    /**
     * @brief returns the view of upstream exchanges.
     * @details this process consumes input data from these exchanges.
     * @attention this will be invalidated after the upstream connections were changed
     * @return the view of upstream exchanges
     */
    step_list_view<exchange> upstreams() noexcept;
    
    /// @copydoc upstreams()
    step_list_view<exchange const> upstreams() const noexcept;

    /**
     * @brief returns the view of downstream exchanges.
     * @details this process produces output into these exchanges.
     * @attention this will be invalidated after the downstream connections were changed
     * @return the view of downstream exchanges
     */
    step_list_view<exchange> downstreams() noexcept;

    /// @copydoc downstreams()
    step_list_view<exchange const> downstreams() const noexcept;
    
    /**
     * @brief returns whether or not this obtains input data from the given exchange.
     * @param upstream the target exchange
     * @return true if the given exchange is an upstream of this
     * @return false otherwise
     */
    bool has_upstream(exchange const& upstream) const noexcept;

    /**
     * @brief adds a process input.
     * @details if the given exchange is already an upstream of this process, this operation does nothing.
     * @attention this operation may invalidate the result of predecessors(), upstreams(),
     *      successors() of the upstream, and downstreams() of the upstream.
     * @param upstream the upstream exchange
     * @return this
     */
    process& add_upstream(exchange& upstream);

    /**
     * @brief removes a process input.
     * @details if the given exchange is not an upstream of this process, this operation does nothing.
     * @attention this operation may invalidate the result of predecessors(), upstreams(),
     *      successors() of the upstream, and downstreams() of the upstream.
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
    bool has_downstream(exchange const& downstream) const noexcept;

    /**
     * @brief adds a process output.
     * @details if the given exchange is already a downstream of this process, this operation does nothing.
     * @attention this operation may invalidate the result of successors(), downstreams(),
     *      predecessors() of the downstream, and upstreams() of the downstream
     * @param downstream the downstream exchange
     * @return this
     */
    process& add_downstream(exchange& downstream);

    /**
     * @brief removes a process output.
     * @details if the given exchange is not a downstream of this process, this operation does nothing.
     * @attention this operation may invalidate the result of successors(), downstreams(),
     *      predecessors() of the downstream, and upstreams() of the downstream
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
    bool equals(step const& other) const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    graph::graph<relation::expression> operators_ {};
    std::vector<exchange*, util::object_allocator<exchange*>> upstreams_ {};
    std::vector<exchange*, util::object_allocator<exchange*>> downstreams_ {};

    void internal_add_upstream(exchange& upstream);
    void internal_remove_upstream(exchange& upstream) noexcept;
    void internal_add_downstream(exchange& downstream);
    void internal_remove_downstream(exchange& downstream) noexcept;

    friend class exchange;
};

} // namespace takatori::plan
