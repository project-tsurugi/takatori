#pragma once

#include <functional>
#include <iostream>

#include "data_type_kind.h"

#include "takatori/util/object_creator.h"

namespace takatori::type {

/**
 * @brief a root model of data types.
 */
class data_type {
public:
    /**
     * @brief destroys this object.
     */
    virtual ~data_type() = default;

    /**
     * @brief returns the kind of this data_type.
     * @return the data_type kind
     */
    virtual data_type_kind kind() const noexcept = 0;

    /**
     * @brief returns a clone of this object.
     * @param creator the object creator
     * @return the created clone
     */
    virtual data_type* clone(util::object_creator creator) const& = 0;

    /// @copydoc clone()
    virtual data_type* clone(util::object_creator creator) && = 0;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(data_type const& a, data_type const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend inline bool operator!=(data_type const& a, data_type const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, data_type const& value);

protected:
    /**
     * @brief creates a new instance.
     */
    data_type() = default;

    /**
     * @brief creates a new instance.
     * @param other the copy source
     */
    data_type(data_type const& other) = default;

    /**
     * @brief assigns the given object.
     * @param other the copy source
     * @return this
     */
    data_type& operator=(data_type const& other) = default;

    /**
     * @brief creates a new instance.
     * @param other the move source
     */
    data_type(data_type&& other) noexcept = default;

    /**
     * @brief assigns the given object.
     * @param other the move source
     * @return this
     */
    data_type& operator=(data_type&& other) noexcept = default;

    /**
     * @brief returns whether or not this data_types is equivalent to the target one.
     * @param other the target data_type
     * @return true if the both are equivalent
     * @return false otherwise
     */
    virtual bool equals(data_type const& other) const noexcept = 0;

    /**
     * @brief returns hash code of this object.
     * The returned digest value should compute from only type specific information.
     * @return the computed hash code
     */
    virtual std::size_t hash() const noexcept = 0;

    /**
     * @brief appends string representation of this object into the given output.
     * @param out the target output
     * @return the output
     */
    virtual std::ostream& print_to(std::ostream& out) const = 0;

    friend struct ::std::hash<data_type>;
};

} // namespace takatori::type

namespace std {

/**
 * @brief provides hash code of data_type.
 */
template<>
struct hash<takatori::type::data_type> {
    /**
     * @brief returns hash code of the given object.
     * @param object the target object
     * @return the computed hash code
     */
    std::size_t operator()(takatori::type::data_type const& object) const noexcept {
        return object.hash() * 31 + static_cast<std::size_t>(object.kind());
    }
};
} // namespace std
