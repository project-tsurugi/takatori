#pragma once

#include <iostream>
#include <utility>
#include <vector>

#include "step.h"
#include "step_kind.h"

#include <takatori/descriptor/variable.h>

#include <takatori/util/object_creator.h>
#include <takatori/util/reference_vector.h>
#include <takatori/util/sequence_view.h>

namespace takatori::plan {

class process;

/**
 * @brief an abstract implementation of exchange step, which exchanges data between processes.
 */
class exchange : public step {
public:
    step_kind kind() const noexcept override = 0;

    ~exchange() override;
    exchange(exchange const& other) = delete;
    exchange& operator=(exchange const& other) = delete;
    exchange(exchange&& other) noexcept = delete;
    exchange& operator=(exchange&& other) noexcept = delete;

    exchange* clone(util::object_creator creator) const& override = 0;
    exchange* clone(util::object_creator creator) && override = 0;

    /**
     * @brief returns the columns which this exchange accepts from the upstream processes.
     * @return the input columns
     * @note these columns also may appear in output_columns()
     */
    virtual util::sequence_view<descriptor::variable const> input_columns() const noexcept = 0;

    /**
     * @brief returns the columns which this exchange provides to the downstream processes.
     * @return the output columns
     * @note these columns also may appear in input_columns()
     */
    virtual util::sequence_view<descriptor::variable const> output_columns() const noexcept = 0;

    /**
     * @brief returns the view of upstream processes.
     * @details this exchange consumes input data from these processes.
     * @attention this will be invalidated after the upstream connections were changed
     * @return the view of upstream processes
     */
    step_list_view<process> upstreams() noexcept;
    
    /// @copydoc upstreams()
    step_list_view<process const> upstreams() const noexcept;

    /**
     * @brief returns the view of downstream processes.
     * @details this exchange produces output into these processes.
     * @attention this will be invalidated after the downstream connections were changed
     * @return the view of downstream processes
     */
    step_list_view<process> downstreams() noexcept;

    /// @copydoc downstreams()
    step_list_view<process const> downstreams() const noexcept;

    /**
     * @brief returns whether or not this obtains input data from the given process.
     * @param upstream the target process
     * @return true if the given process is an upstream of this
     * @return false otherwise
     */
    bool has_upstream(process const& upstream) const noexcept;

    /**
     * @brief adds a exchange input.
     * @details if the given process is already an upstream of this exchange, this operation does nothing.
     * @attention this operation may invalidate the result of predecessors(), upstreams(),
     *      successors() of the upstream, and downstreams() of the upstream.
     * @param upstream the upstream process
     * @return this
     */
    exchange& add_upstream(process& upstream);

    /**
     * @brief removes a exchange input.
     * @details if the given process is not an upstream of this exchange, this operation does nothing.
     * @attention this operation may invalidate the result of predecessors(), upstreams(),
     *      successors() of the upstream, and downstreams() of the upstream.
     * @param upstream the upstream process
     * @return this
     */
    exchange& remove_upstream(process& upstream);

    /**
     * @brief returns whether or not this offers output data into the given process.
     * @param downstream the target process
     * @return true if the given process is a downstream of this
     * @return false otherwise
     */
    bool has_downstream(process const& downstream) const noexcept;

    /**
     * @brief adds a exchange output.
     * @details if the given process is already a downstream of this exchange, this operation does nothing.
     * @attention this operation may invalidate the result of successors(), downstreams(),
     *      predecessors() of the downstream, and upstreams() of the downstream
     * @param downstream the downstream process
     * @return this
     */
    exchange& add_downstream(process& downstream);

    /**
     * @brief removes a exchange output.
     * @details if the given process is not a downstream of this exchange, this operation does nothing.
     * @attention this operation may invalidate the result of successors(), downstreams(),
     *      predecessors() of the downstream, and upstreams() of the downstream
     * @param downstream the downstream process
     * @return this
     */
    exchange& remove_downstream(process& downstream);

    /**
     * @brief adds an upstream process to the exchange.
     * @param downstream the exchange
     * @param upstream the upstream process
     * @return the exchange
     */
    friend exchange& operator<<(exchange& downstream, process& upstream);

    /**
     * @brief adds a downstream process to the exchange.
     * @param upstream the exchange
     * @param downstream the downstream process
     * @return the exchange
     */
    friend exchange& operator>>(exchange& upstream, process& downstream);

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(exchange const& a, exchange const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(exchange const& a, exchange const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, exchange const& value);

protected:
    /**
     * @brief creates a new object.
     * @param creator the object creator for internal elements
     */
    explicit exchange(util::object_creator creator = {}) noexcept;

    bool equals(step const& other) const noexcept override = 0;
    std::ostream& print_to(std::ostream& out) const override = 0;

private:
    std::vector<process*, util::object_allocator<process*>> upstreams_;
    std::vector<process*, util::object_allocator<process*>> downstreams_;

    void internal_add_upstream(process& upstream);
    void internal_remove_upstream(process& upstream) noexcept;
    void internal_add_downstream(process& downstream);
    void internal_remove_downstream(process& downstream) noexcept;

    friend class process;
};

} // namespace takatori::plan
