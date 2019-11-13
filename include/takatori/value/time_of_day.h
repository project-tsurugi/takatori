#pragma once

#include <iostream>

#include "value_kind.h"
#include "simple_value.h"

#include "takatori/chrono/time_of_day.h"

#include "takatori/util/meta_type.h"
#include "takatori/util/object_creator.h"

namespace takatori::value {

/**
 * @brief represents time of day as polymorphic value model.
 */
class time_of_day : public data {
public:
    /// @brief the kind of this type.
    static constexpr inline value_kind tag = value_kind::time_of_day;

    /// @brief the entity type
    using entity_type = chrono::time_of_day;

    /// @brief the view type
    using view_type = entity_type;

    /**
     * @brief creates a new instance.
     * @param value the time
     */
    explicit constexpr time_of_day(entity_type value) noexcept;

    // FIXME: duplicate constructor from chrono

    ~time_of_day() override = default;
    time_of_day(time_of_day const& other) = delete;
    time_of_day& operator=(time_of_day const& other) = delete;
    time_of_day(time_of_day&& other) noexcept = delete;
    time_of_day& operator=(time_of_day&& other) noexcept = delete;

    value_kind kind() const noexcept override;
    time_of_day* clone(util::object_creator creator) const& override;
    time_of_day* clone(util::object_creator creator) && override;

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
    friend constexpr bool operator==(time_of_day const& a, time_of_day const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend constexpr bool operator!=(time_of_day const& a, time_of_day const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, time_of_day const& value);

protected:
    bool equals(data const& other) const noexcept override;
    std::size_t hash() const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    entity_type entity_;

    friend class util::object_creator;
};

inline constexpr time_of_day::time_of_day(time_of_day::entity_type value) noexcept
    : entity_(value)
{}

inline constexpr time_of_day::view_type time_of_day::get() const noexcept {
    return entity_;
}

inline constexpr time_of_day::operator view_type() const noexcept {
    return get();
}

inline constexpr bool operator==(time_of_day const& a, time_of_day const& b) noexcept {
    return a.get() == b.get();
}

inline constexpr bool operator!=(time_of_day const& a, time_of_day const& b) noexcept {
    return !(a == b);
}

/**
 * @brief type_of for time_of_day.
 */
template<> struct type_of<time_of_day::tag> : util::meta_type<time_of_day> {};

} // namespace takatori::value

/// @brief provides hash code of takatori::value::time_of_day.
template<> struct std::hash<takatori::value::time_of_day> : hash<takatori::value::data> {};
