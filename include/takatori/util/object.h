#pragma once

#include <functional>
#include <iostream>

namespace takatori::util {

/**
 * @brief a root interface of polymorphic objects.
 */
class object {
public:
    /**
     * @brief destroys this object.
     */
    virtual ~object() = default;

    /**
     * @brief returns whether or not the two objects are equivalent.
     * @param a the first object
     * @param b the second object
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(object const& a, object const& b) noexcept {
        return a.equals(b);
    }

    /**
     * @brief returns whether or not the two objects are different.
     * @param a the first object
     * @param b the second object
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(object const& a, object const& b) noexcept {
        return !(a == b);
    }

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output stream
     */
    friend std::ostream& operator<<(std::ostream& out, object const& value) {
        return value.print_to(out);
    }

protected:
    /**
     * @brief creates a new instance.
     */
    object() = default;

    /**
     * @brief creates a new instance.
     * @param other the copy source
     */
    object(object const& other) = default;

    /**
     * @brief assigns the given object.
     * @param other the copy source
     * @return this
     */
    object& operator=(object const& other) = default;

    /**
     * @brief creates a new instance.
     * @param other the move source
     */
    object(object&& other) noexcept = default;

    /**
     * @brief assigns the given object.
     * @param other the move source
     * @return this
     */
    object& operator=(object&& other) noexcept = default;

    /**
     * @brief returns whether or not this object is equivalent to the target one.
     * @param other the target object
     * @return true if the both are equivalent
     * @return false otherwise
     */
    virtual bool equals(object const& other) const noexcept = 0;

    /**
     * @brief returns a hash code of this object.
     * @return the computed hash code
     */
    virtual std::size_t hash() const noexcept = 0;

    /**
     * @brief appends string representation of this object into the given output.
     * @param out the target output
     * @return the output
     */
    virtual std::ostream& print_to(std::ostream& out) const = 0;

    friend class ::std::hash<object>;
};

} // namespace takatori::util

/**
 * @brief std::hash specialization for takatori::util::object.
 */
template<>
struct std::hash<takatori::util::object> {
    /**
     * @brief compute hash of the given object.
     * @param value the target object
     * @return computed hash code
     */
    std::size_t operator()(takatori::util::object const& value) const noexcept {
        return value.hash();
    }
};
