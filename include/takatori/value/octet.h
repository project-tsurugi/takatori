#pragma once

#include <ostream>
#include <string>
#include <string_view>

#include "value_kind.h"
#include "simple_value.h"

#include <takatori/util/meta_type.h>
#include <takatori/util/object_creator.h>

namespace takatori::value {

/**
 * @brief an octet sequence as polymorphic value model.
 */
class octet : public data {
public:
    /// @brief the kind of this type.
    static constexpr inline value_kind tag = value_kind::octet;

    /// @brief the allocator type
    using allocator_type = util::object_allocator<char>;

    /// @brief the entity type
    using entity_type = std::basic_string<char, std::char_traits<char>, allocator_type>;

    /// @brief the view type
    using view_type = std::string_view;

    /**
     * @brief creates a new instance.
     * @param value the octet string
     */
    explicit octet(entity_type value) noexcept;

    /**
     * @brief creates a new instance.
     * @tparam T the octet string
     * @param value the octet string
     * @param allocator the allocator
     */
    template<class T>
    explicit octet(T const& value, allocator_type const& allocator = {})
        : entity_(value, allocator)
    {}

    ~octet() override = default;
    octet(octet const& other) = delete;
    octet& operator=(octet const& other) = delete;
    octet(octet&& other) noexcept = delete;
    octet& operator=(octet&& other) noexcept = delete;

    [[nodiscard]] value_kind kind() const noexcept override;
    [[nodiscard]] octet* clone(util::object_creator creator) const& override;
    [[nodiscard]] octet* clone(util::object_creator creator) && override;

    /**
     * @brief returns the entity value.
     * @return the entity value
     */
    [[nodiscard]] view_type get() const noexcept;

    /// @copydoc get()
    [[nodiscard]] explicit operator view_type() const noexcept;

protected:
    [[nodiscard]] bool equals(data const& other) const noexcept override;
    [[nodiscard]] std::size_t hash() const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    entity_type entity_;

    friend class util::object_creator;
};

/**
 * @brief returns whether or not the two elements are equivalent.
 * @param a the first element
 * @param b the second element
 * @return true if a == b
 * @return false otherwise
 */
bool operator==(octet const& a, octet const& b) noexcept;

/**
 * @brief returns whether or not the two elements are different.
 * @param a the first element
 * @param b the second element
 * @return true if a != b
 * @return false otherwise
 */
bool operator!=(octet const& a, octet const& b) noexcept;

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
std::ostream& operator<<(std::ostream& out, octet const& value);

/**
 * @brief type_of for octet.
 */
template<> struct type_of<octet::tag> : util::meta_type<octet> {};

} // namespace takatori::value

/// @brief provides hash code of takatori::value::octet.
template<> struct std::hash<takatori::value::octet> : hash<takatori::value::data> {};
