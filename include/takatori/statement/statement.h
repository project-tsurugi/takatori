#pragma once

#include <ostream>

#include "statement_kind.h"

#include <takatori/document/region.h>

#include <takatori/tree/tree_element_base.h>

#include <takatori/util/object_creator.h>
#include <takatori/util/optional_ptr.h>

namespace takatori::statement {

/**
 * @brief a root model of procedural statements.
 */
class statement : public tree::tree_element_base {
public:
    /// @brief the parent type.
    using parent_type = tree::tree_element_base;

    /**
     * @brief creates a new instance.
     */
    statement() = default;

    /**
     * @brief destroys this object.
     */
    ~statement() override = default;

    statement(statement const& other) = delete;
    statement& operator=(statement const& other) = delete;
    statement(statement&& other) noexcept = delete;
    statement& operator=(statement&& other) noexcept = delete;

    /**
     * @brief returns the kind of this statement.
     * @return the statement kind
     */
    [[nodiscard]] virtual statement_kind kind() const noexcept = 0;

    /**
     * @brief returns a clone of this object.
     * @param creator the object creator
     * @return the created clone
     */
    [[nodiscard]] virtual statement* clone(util::object_creator creator) const& = 0;

    /// @copydoc clone()
    [[nodiscard]] virtual statement* clone(util::object_creator creator) && = 0;

    /**
     * @brief returns the parent element.
     * @return the parent element
     * @return nullptr if this is the root or orphaned element
     */
    [[nodiscard]] parent_type* parent_element() noexcept;

    /**
     * @brief returns the parent element.
     * @return the parent element
     * @return nullptr if this is the root or orphaned element
     */
    [[nodiscard]] parent_type const* parent_element() const noexcept;

    /**
     * @brief sets the parent element.
     * @param parent the parent element
     */
    void parent_element(parent_type* parent) noexcept;

    /**
     * @brief returns the parent statement
     * @return the parent statement
     * @return empty if it is not set, or the parent element is not an statement
     */
    [[nodiscard]] util::optional_ptr<statement> parent_statement() noexcept;

    /// @copydoc parent_statement()
    [[nodiscard]] util::optional_ptr<statement const> parent_statement() const noexcept;

    /**
     * @brief returns the document region of this element.
     * @return the document region
     */
    [[nodiscard]] document::region& region() noexcept;

    /// @copydoc region()
    [[nodiscard]] document::region const& region() const noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(statement const& a, statement const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(statement const& a, statement const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, statement const& value);

protected:
    /**
     * @brief returns whether or not this statements is equivalent to the target one.
     * @param other the target statement
     * @return true if the both are equivalent
     * @return false otherwise
     */
    [[nodiscard]] virtual bool equals(statement const& other) const noexcept = 0;

    /**
     * @brief appends string representation of this object into the given output.
     * @param out the target output
     * @return the output
     */
    virtual std::ostream& print_to(std::ostream& out) const = 0;

private:
    parent_type* parent_ {};
    document::region region_ {};
};

} // namespace takatori::statement
