#pragma once

#include <optional>
#include <variant>

#include <takatori/descriptor/variable.h>

#include <takatori/util/optional_ptr.h>

#include "constant_value_kind.h"

namespace takatori::relation::details {

/**
 * @brief represents a constant value.
 * @details this value can evaluate before query execution.
 * @tparam T the value type
 */
template<class T>
class constant_value {
public:

    /// @brief the kind of
    using kind_type = constant_value_kind;

    /// @brief the value type.
    using value_type = T;

    /// @brief the variable type.
    using variable_type = descriptor::variable;

    /// @brief the entity type of this constant value.
    using entity_type = std::variant<
            std::monostate,
            value_type,
            variable_type>;

    /**
     * @brief creates a new instance that represents an absent.
     */
    constexpr constant_value() noexcept = default;

    /**
     * @brief creates a new instance that represents an absent.
     */
    constexpr constant_value(std::nullopt_t) noexcept : // NOLINT
        entity_ { std::in_place_type<std::monostate> }
    {}

    /**
     * @brief assigns absent to this constant value.
     * @return this
     */
    constant_value& operator=(std::nullopt_t) noexcept {
        entity_.template emplace<std::monostate>();
        return *this;
    }

    /**
     * @brief creates a new instance from the given value.
     * @param value the constant value
     */
    constant_value(value_type value) noexcept : // NOLINT
        entity_ { std::in_place_type<value_type>, std::move(value) }
    {}

    /**
     * @brief assigns the given value to this constant value.
     * @param value the constant value
     * @return this
     */
    constant_value& operator=(value_type value) noexcept {
        entity_.template emplace<value_type>(std::move(value));
        return *this;
    }

    /**
     * @brief creates a new instance from the given variable.
     * @param variable the variable
     */
    constant_value(variable_type variable) noexcept : // NOLINT
        entity_ { std::in_place_type<variable_type>, std::move(variable) }
    {}

    /**
     * @brief assigns the given variable to this constant value.
     * @param variable the variable
     * @return this
     */
    constant_value& operator=(variable_type variable) noexcept {
        entity_.template emplace<variable_type>(std::move(variable));
        return *this;
    }

    /**
     * @brief creates a new instance.
     * @param entity the entity
     */
    constant_value(entity_type entity) noexcept : // NOLINT
        entity_ { std::move(entity) }
    {}

    /**
     * @brief assigns the given entity to this constant value.
     * @param entity the entity
     * @return this
     */
    constant_value& operator=(entity_type entity) noexcept {
        entity_ = std::move(entity);
        return *this;
    }

    /**
     * @brief returns the kind of this constant value.
     * @return the kind
     */
    [[nodiscard]] kind_type kind() const noexcept {
        switch (entity_.index()) {
            case 0: return kind_type::absent;
            case 1: return kind_type::immediate;
            case 2: return kind_type::variable;
            default: std::abort();
        }
    }

    /**
     * @brief returns whether this constant value is absent.
     * @return true if this constant value is absent
     * @return false otherwise
     */
    [[nodiscard]] bool is_absent() const noexcept {
        return std::holds_alternative<std::monostate>(entity_);
    }

    /**
     * @brief returns whether this constant value is present.
     * @return true if this constant value is present
     * @return false otherwise
     */
    [[nodiscard]] operator bool() const noexcept { // NOLINT
        return !is_absent();
    }

    /**
     * @brief returns whether this constant value has immediate value.
     * @return true if this constant value has immediate value
     * @return false otherwise
     */
    [[nodiscard]] bool has_immediate() const noexcept {
        return std::holds_alternative<value_type>(entity_);
    }

    /**
     * @brief returns the immediate value of this constant value.
     * @return the immediate value
     * @throws std::bad_variant_access if this constant value does not have immediate value
     */
    [[nodiscard]] value_type& immediate() {
        return std::get<value_type>(entity_);
    }

    /// @copydoc immediate()
    [[nodiscard]] value_type const& immediate() const {
        return std::get<value_type>(entity_);
    }

    /**
     * @brief returns the immediate value of this constant value if exists.
     * @return the immediate value
     * @return empty if this constant value does not have immediate value
     */
    [[nodiscard]] util::optional_ptr<value_type> immediate_if() noexcept {
        auto ptr = std::get_if<value_type>(&entity_);
        return util::optional_ptr<value_type> { ptr };
    }

    /// @copydoc immediate_if()
    [[nodiscard]] util::optional_ptr<value_type const> immediate_if() const noexcept {
        auto ptr = std::get_if<value_type>(&entity_);
        return util::optional_ptr<value_type const> { ptr };
    }

    /**
     * @brief returns whether this constant value has variable.
     * @return true if this constant value has variable
     * @return false otherwise
     */
    [[nodiscard]] bool has_variable() const noexcept {
        return std::holds_alternative<variable_type>(entity_);
    }

    /**
    * @brief returns the variable of this constant value.
    * @return the variable
    * @throws std::bad_variant_access if this constant value does not have variable
    */
    [[nodiscard]] variable_type& variable() {
        return std::get<variable_type>(entity_);
    }

    /// @copydoc variable()
    [[nodiscard]] variable_type const& variable() const {
        return std::get<variable_type>(entity_);
    }

     /**
     * @brief returns the variable of this constant value if exists.
     * @return the variable
     * @return empty if this constant value does not have variable
     */
    [[nodiscard]] util::optional_ptr<variable_type> variable_if() noexcept {
        auto ptr = std::get_if<variable_type>(&entity_);
        return util::optional_ptr<variable_type> { ptr };
    }

    /// @copydoc variable_if()
    [[nodiscard]] util::optional_ptr<variable_type const> variable_if() const noexcept {
        auto ptr = std::get_if<variable_type>(&entity_);
        return util::optional_ptr<variable_type const> { ptr };
    }

    /**
     * @brief returns the entity of this constant value.
     * @return the entity
     */
    [[nodiscard]] entity_type& entity() noexcept {
        return entity_;
    }

    /// @copydoc entity()
    [[nodiscard]] entity_type const& entity() const noexcept {
        return entity_;
    }

    /**
     * @brief swaps this constant value with the given one.
     * @param other the other constant value
     */
    void swap(constant_value& other) noexcept {
        using std::swap;
        swap(entity_, other.entity_);
    }

    /**
     * @brief returns whether the two constant values are equivalent.
     * @param a the first constant value
     * @param b the second constant value
     * @return true if a == b
     * @return false otherwise
     */
    [[nodiscard]] friend bool operator==(constant_value const& a, constant_value const& b) noexcept { // NOLINT(bugprone-exception-escape)
        return a.entity_ == b.entity_;
    }

    /**
     * @brief returns whether the two constant values are different.
     * @param a the first constant value
     * @param b the second constant value
     * @return true if a != b
     * @return false otherwise
     */
    [[nodiscard]] friend bool operator!=(constant_value const& a, constant_value const& b) noexcept {
        return !(a == b);
    }

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, constant_value const& value) {
        switch (value.kind()) {
            case kind_type::absent:
                return out << "absent";
            case kind_type::immediate:
                return out << "immediate(" << value.immediate() << ")";
            case kind_type::variable:
                return out << "variable(" << value.variable() << ")";
            default:
                return out << "unknown";
        }
    }

private:
    entity_type entity_ {};
};

} // namespace takatori::relation::details
