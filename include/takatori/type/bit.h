#pragma once

#include <optional>

#include "type_kind.h"
#include "data.h"
#include "varying.h"

#include <takatori/util/meta_type.h>

namespace takatori::type {

/**
 * @brief bit sequence type.
 */
class bit : public data {
public:
    /// @brief the kind of this type.
    static constexpr inline type_kind tag = type_kind::bit;

    /// @brief the size type.
    using size_type = std::size_t;

    bit() = delete;

    /**
     * @brief creates a new instance which represents fixed-length bit sequences (a.k.a. BIT).
     * @param length the number of bits in the sequence
     */
    explicit constexpr bit(size_type length) noexcept;

    /**
     * @brief creates a new instance which represents flexible-length bit sequences (a.k.a. VARBIT).
     * @param varying the bit sequence length becomes flexible if enabled, otherwise fixed
     * @param length the max number of bits in the sequence,
     *               or empty for flexible size bit sequences
     */
    explicit constexpr bit(varying_t varying, std::optional<size_type> length = {}) noexcept;

    ~bit() override = default;
    bit(bit const& other) = delete;
    bit& operator=(bit const& other) = delete;
    bit(bit&& other) noexcept = delete;
    bit& operator=(bit&& other) noexcept = delete;

    type_kind kind() const noexcept override;
    bit* clone(util::object_creator creator) const& override;
    bit* clone(util::object_creator creator) && override;

    /**
     * @brief returns whether or not this bit sequence can vary its length.
     * @return true if this is flexible length bit sequence (a.k.a. VARBIT)
     * @return false if this is fixed length bit sequence (a.k.a. BIT)
     */
    constexpr bool varying() const noexcept;

    /**
     * @brief returns the max number of bits in the sequence.
     * @note if this object represents fixed-length bit sequence, the returned value must not be empty.
     * @return the max number of bits
     * @return empty if it is not defined
     * @see is_varying()
     */
    constexpr std::optional<size_type> length() const noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(bit const& a, bit const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(bit const& a, bit const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, bit const& value);

protected:
    bool equals(data const& other) const noexcept override;
    std::size_t hash() const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    bool varying_;
    std::optional<size_type> length_;

    friend class util::object_creator;
};

constexpr
bit::bit(bit::size_type length) noexcept
    : bit(~type::varying, length)
{}

constexpr
bit::bit(varying_t varying, std::optional<size_type> length) noexcept
    : varying_(varying.enabled())
    , length_(std::move(length))
{}

constexpr bool
bit::varying() const noexcept {
    return varying_;
}

constexpr std::optional<bit::size_type>
bit::length() const noexcept {
    return length_;
}

template<> struct type_of<bit::tag> : util::meta_type<bit> {};

} // namespace takatori::type

/// @brief provides hash code of takatori::type::bit.
template<> struct std::hash<takatori::type::bit> : hash<takatori::type::data> {};
