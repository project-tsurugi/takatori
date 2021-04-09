#pragma once

#include <utility>

#include "descriptor_kind.h"

namespace takatori::descriptor {

/**
 * @brief binding information from each program element descriptor.
 * @tparam Kind the element kind
 */
template<descriptor_kind Kind>
class binding_info {
public:
    /// @brief the descriptor kind.
    static constexpr inline descriptor_kind tag = Kind;

    /**
     * @brief destroys this object.
     */
    virtual ~binding_info() = default;

    /**
     * @brief returns the derived class ID.
     * @note this is to avoid `dynamic_cast` for performance reason.
     * Each different derived class must return a different ID, and the same class must return the same ID.
     * That is, `a == b` implies `a.class_id() == b.class_id()`, and `a.class_id() != b.class_id()` implies `a != b`.
     * @return the derived class ID
     */
    [[nodiscard]] virtual std::size_t class_id() const noexcept = 0;

    /**
     * @brief returns whether or not the two objects are equivalent.
     * @param a the first object
     * @param b the second object
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(binding_info const& a, binding_info const& b) noexcept {
        return a.equals(b);
    }

    /**
     * @brief returns whether or not the two objects are different.
     * @param a the first object
     * @param b the second object
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(binding_info const& a, binding_info const& b) noexcept {
        return !a.equals(b);
    }

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output stream
     */
    friend std::ostream& operator<<(std::ostream& out, binding_info const& value) {
        return value.print_to(out);
    }

protected:
    /**
     * @brief creates a new instance.
     */
    binding_info() = default;

    /**
     * @brief creates a new instance.
     * @param other the copy source
     */
    binding_info(binding_info const& other) = default;

    /**
     * @brief assigns the given object.
     * @param other the copy source
     * @return this
     */
    binding_info& operator=(binding_info const& other) = default;

    /**
     * @brief creates a new instance.
     * @param other the move source
     */
    binding_info(binding_info&& other) noexcept = default;

    /**
     * @brief assigns the given object.
     * @param other the move source
     * @return this
     */
    binding_info& operator=(binding_info&& other) noexcept = default;

    /**
     * @brief returns whether or not this object is equivalent to the target one.
     * @param other the target object
     * @return true if the both are equivalent
     * @return false otherwise
     */
    [[nodiscard]] virtual bool equals(binding_info const& other) const noexcept = 0;

    /**
     * @brief returns a hash code of this object.
     * @return the computed hash code
     */
    [[nodiscard]] virtual std::size_t hash() const noexcept = 0;

    /**
     * @brief appends string representation of this object into the given output.
     * @param out the target output
     * @return the output
     */
    virtual std::ostream& print_to(std::ostream& out) const = 0;

    friend class ::std::hash<binding_info<Kind>>;
};

} // namespace takatori::descriptor

/**
 * @brief std::hash specialization for takatori::descriptor::binding_info.
 * @tparam Kind the element kind
 */
template<takatori::descriptor::descriptor_kind Kind>
struct std::hash<takatori::descriptor::binding_info<Kind>> {
    /**
     * @brief compute hash of the given object.
     * @param value the target object
     * @return computed hash code
     */
    std::size_t operator()(takatori::descriptor::binding_info<Kind> const& value) const noexcept {
        return value.hash();
    }
};
