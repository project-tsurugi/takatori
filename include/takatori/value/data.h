#pragma once

#include <functional>
#include <iostream>

#include "value_kind.h"

#include <takatori/util/object_creator.h>

namespace takatori::value {

/**
 * @brief a root model of polymorphic values.
 */
class data {
public:
    /**
     * @brief destroys this object.
     */
    virtual ~data() = default;

    /**
     * @brief returns the kind of this value.
     * @return the value kind
     */
    [[nodiscard]] virtual value_kind kind() const noexcept = 0;

    /**
     * @brief returns a clone of this object.
     * @param creator the object creator
     * @return the created clone
     */
    [[nodiscard]] virtual data* clone(util::object_creator creator) const& = 0;

    /// @copydoc clone()
    virtual data* clone(util::object_creator creator) && = 0;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(data const& a, data const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(data const& a, data const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, data const& value);

protected:
    /**
     * @brief creates a new instance.
     */
    data() = default;

    /**
     * @brief creates a new instance.
     * @param other the copy source
     */
    data(data const& other) = default;

    /**
     * @brief assigns the given object.
     * @param other the copy source
     * @return this
     */
    data& operator=(data const& other) = default;

    /**
     * @brief creates a new instance.
     * @param other the move source
     */
    data(data&& other) noexcept = default;

    /**
     * @brief assigns the given object.
     * @param other the move source
     * @return this
     */
    data& operator=(data&& other) noexcept = default;

    /**
     * @brief returns whether or not this type is equivalent to the target one.
     * @param other the target type
     * @return true if the both are equivalent
     * @return false otherwise
     */
    [[nodiscard]] virtual bool equals(data const& other) const noexcept = 0;

    /**
     * @brief returns hash code of this object.
     * @details The returned digest value should compute from only type specific information.
     * @return the computed hash code
     */
    [[nodiscard]] virtual std::size_t hash() const noexcept = 0;

    /**
     * @brief appends string representation of this object into the given output.
     * @param out the target output
     * @return the output
     */
    virtual std::ostream& print_to(std::ostream& out) const = 0;

    friend struct std::hash<data>;
};

} // namespace takatori::value

/**
 * @brief provides hash code of values.
 */
template<>
struct std::hash<takatori::value::data> {
    /**
     * @brief returns hash code of the given object.
     * @param object the target object
     * @return the computed hash code
     */
    std::size_t operator()(takatori::value::data const& object) const noexcept {
        return object.hash() * 31 + static_cast<std::size_t>(object.kind());
    }
};
