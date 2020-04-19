#pragma once

#include <takatori/descriptor/variable.h>
#include <takatori/descriptor/relation.h>
#include <takatori/descriptor/function.h>
#include <takatori/descriptor/aggregate_function.h>
#include <takatori/descriptor/declared_type.h>

#include <takatori/value/data.h>
#include <takatori/type/data.h>
#include <takatori/scalar/expression.h>
#include <takatori/relation/expression.h>
#include <takatori/plan/step.h>

#include "object_acceptor.h"

namespace takatori::serializer {

/**
 * @brief inspects structure of objects and pass their elements into object_acceptor.
 */
class object_scanner {
public:
    /**
     * @brief creates a new instance, which does not inspect verbose information.
     */
    constexpr object_scanner() = default;

    /**
     * @brief creates a new instance.
     * @param verbose whether or not inspects verbose information
     */
    explicit constexpr object_scanner(bool verbose) noexcept
        : verbose_(verbose)
    {}

    /**
     * @brief destroys this instance.
     */
    virtual ~object_scanner() = default;

    /**
     * @brief creates a new instance.
     * @param other the copy source
     */
    object_scanner(object_scanner const& other) = default;

    /**
     * @brief assigns the given object into this.
     * @param other the copy source
     * @return this
     */
    object_scanner& operator=(object_scanner const& other) = default;

    /**
     * @brief creates a new instance.
     * @param other the move source
     */
    object_scanner(object_scanner&& other) noexcept = default;

    /**
     * @brief assigns the given object into this.
     * @param other the move source
     * @return this
     */
    object_scanner& operator=(object_scanner&& other) noexcept = default;

    /**
     * @brief returns whether or not this inspects verbose information.
     * @return true if this also inspects verbose information
     * @return false if this only inspects simple information
     */
    [[nodiscard]] constexpr bool verbose() const noexcept {
        return verbose_;
    }

    /**
     * @brief sets whether or not inspects verbose information.
     * @param verbose whether or not enable verbose information
     * @return this
     */
    constexpr object_scanner& verbose(bool verbose) noexcept {
        verbose_ = verbose;
        return *this;
    }

    /**
     * @brief scans an element and pass its structural elements into the given acceptor.
     * @param element the target element
     * @param acceptor the acceptor
     */
    template<class T>
    void operator()(T const& element, object_acceptor&& acceptor) {
        (*this)(element, acceptor);
    }

    /**
     * @brief scans a descriptor and pass its structural elements into the given acceptor.
     * @param element the target element
     * @param acceptor the acceptor
     */
    virtual void operator()(descriptor::variable const& element, object_acceptor& acceptor);

    /// @copydoc operator()(descriptor::variable const&, object_acceptor&)
    virtual void operator()(descriptor::relation const& element, object_acceptor& acceptor);

    /// @copydoc operator()(descriptor::variable const&, object_acceptor&)
    virtual void operator()(descriptor::function const& element, object_acceptor& acceptor);

    /// @copydoc operator()(descriptor::variable const&, object_acceptor&)
    virtual void operator()(descriptor::aggregate_function const& element, object_acceptor& acceptor);

    /// @copydoc operator()(descriptor::variable const&, object_acceptor&)
    virtual void operator()(descriptor::declared_type const& element, object_acceptor& acceptor);

    /**
     * @brief scans a value and pass its structural elements into the given acceptor.
     * @param element the target element
     * @param acceptor the acceptor
     */
    virtual void operator()(value::data const& element, object_acceptor& acceptor);

    /**
     * @brief scans a type and pass its structural elements into the given acceptor.
     * @param element the target element
     * @param acceptor the acceptor
     */
    virtual void operator()(type::data const& element, object_acceptor& acceptor);

    /**
     * @brief scans a scalar expression and pass its structural elements into the given acceptor.
     * @param element the target element
     * @param acceptor the acceptor
     */
    virtual void operator()(scalar::expression const& element, object_acceptor& acceptor);

    /**
     * @brief scans a relation expression and pass its structural elements into the given acceptor.
     * @param element the target element
     * @param acceptor the acceptor
     */
    virtual void operator()(relation::expression const& element, object_acceptor& acceptor);

    /**
     * @brief scans a relation expression graph and pass its structural elements into the given acceptor.
     * @param element the target element
     * @param acceptor the acceptor
     */
    virtual void operator()(relation::expression::graph_type const& element, object_acceptor& acceptor);

    /**
     * @brief scans a step pass its structural elements into the given acceptor.
     * @param element the target element
     * @param acceptor the acceptor
     */
    virtual void operator()(plan::step const& element, object_acceptor& acceptor);

    /**
     * @brief scans a step graph and pass its structural elements into the given acceptor.
     * @param element the target element
     * @param acceptor the acceptor
     */
    virtual void operator()(plan::step::graph_type const& element, object_acceptor& acceptor);

protected:
    /**
     * @brief scans properties of the given descriptor.
     * @param element the target element
     * @param acceptor the acceptor
     * @note This may be called only if verbose() is enabled.
     */
    virtual void properties(descriptor::variable const& element, object_acceptor& acceptor);

    /// @copydoc properties(descriptor::variable const&, object_acceptor&)
    virtual void properties(descriptor::relation const& element, object_acceptor& acceptor);

    /// @copydoc properties(descriptor::variable const&, object_acceptor&)
    virtual void properties(descriptor::function const& element, object_acceptor& acceptor);

    /// @copydoc properties(descriptor::variable const&, object_acceptor&)
    virtual void properties(descriptor::aggregate_function const& element, object_acceptor& acceptor);

    /// @copydoc properties(descriptor::variable const&, object_acceptor&)
    virtual void properties(descriptor::declared_type const& element, object_acceptor& acceptor);

    /**
     * @brief scans properties of the given value.
     * @param element the target element
     * @param acceptor the acceptor
     * @note This may be called only if verbose() is enabled.
     */
    virtual void properties(value::data const& element, object_acceptor& acceptor);

    /**
     * @brief scans properties of the given type.
     * @param element the target element
     * @param acceptor the acceptor
     * @note This may be called only if verbose() is enabled.
     */
    virtual void properties(type::data const& element, object_acceptor& acceptor);

    /**
     * @brief scans properties of the given expression.
     * @param element the target element
     * @param acceptor the acceptor
     */
    virtual void properties(scalar::expression const& element, object_acceptor& acceptor);

    /**
     * @brief scans properties of the relation expression.
     * @param element the target element
     * @param acceptor the acceptor
     */
    virtual void properties(relation::expression const& element, object_acceptor& acceptor);

    /**
     * @brief scans properties of the given step.
     * @param element the target element
     * @param acceptor the acceptor
     */
    virtual void properties(plan::step const& element, object_acceptor& acceptor);

private:
    bool verbose_ {};

    template<class T>
    void process_default(T const& element, object_acceptor& acceptor);

    template<class T>
    void process_envelope(T const& element, object_acceptor& acceptor);
};

} // namespace takatori::serializer
