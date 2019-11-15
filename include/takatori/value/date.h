#pragma once

#include <iostream>

#include "value_kind.h"
#include "simple_value.h"

#include "takatori/datetime/date.h"

#include "takatori/util/meta_type.h"
#include "takatori/util/object_creator.h"

namespace takatori::value {

/**
 * @brief represents date as polymorphic value model.
 */
class date : public data {
public:
    /// @brief the kind of this type.
    static constexpr inline value_kind tag = value_kind::date;

    /// @brief the entity type
    using entity_type = datetime::date;

    /// @brief the view type
    using view_type = entity_type;

    /**
     * @brief creates a new instance.
     * @param value the date
     */
    explicit constexpr date(entity_type value) noexcept;

    /**
     * @brief creates a new instance from year. month, and day triple.
     * @param year the year (1900-)
     * @param month the month number (1-12) of year
     * @param day the day (1-31) of month
     */
    explicit date(std::uint32_t year, std::uint32_t month, std::uint32_t day) noexcept;

    ~date() override = default;
    date(date const& other) = delete;
    date& operator=(date const& other) = delete;
    date(date&& other) noexcept = delete;
    date& operator=(date&& other) noexcept = delete;

    value_kind kind() const noexcept override;
    date* clone(util::object_creator creator) const& override;
    date* clone(util::object_creator creator) && override;

    /**
     * @brief returns the entity value.
     * @return the entity value
     */
    constexpr view_type get() const noexcept;

    /// @copydoc get()
    explicit constexpr operator view_type() const noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend constexpr bool operator==(date const& a, date const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend constexpr bool operator!=(date const& a, date const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, date const& value);

protected:
    bool equals(data const& other) const noexcept override;
    std::size_t hash() const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    entity_type entity_;

    friend class util::object_creator;
};

inline constexpr date::date(date::entity_type value) noexcept
    : entity_(value)
{}

inline constexpr date::view_type date::get() const noexcept {
    return entity_;
}

inline constexpr date::operator view_type() const noexcept {
    return get();
}

inline constexpr bool operator==(date const& a, date const& b) noexcept {
    return a.get() == b.get();
}

inline constexpr bool operator!=(date const& a, date const& b) noexcept {
    return !(a == b);
}

/**
 * @brief type_of for date.
 */
template<> struct type_of<date::tag> : util::meta_type<date> {};

} // namespace takatori::value

/// @brief provides hash code of takatori::value::date.
template<> struct std::hash<takatori::value::date> : hash<takatori::value::data> {};
