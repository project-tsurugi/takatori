#pragma once

#include <chrono>
#include <iostream>

#include "value_kind.h"
#include "simple_value.h"

#include "takatori/datetime/time_point.h"

#include "takatori/util/meta_type.h"
#include "takatori/util/object_creator.h"

namespace takatori::value {

/**
 * @brief represents time point as polymorphic value model.
 */
class time_point : public data {
public:
    /// @brief the kind of this type.
    static constexpr inline value_kind tag = value_kind::time_point;

    /// @brief the entity type
    using entity_type = datetime::time_point;

    /// @brief the view type
    using view_type = entity_type;

    /**
     * @brief creates a new instance.
     * @param value the time
     */
    explicit constexpr time_point(entity_type value) noexcept;

    /**
     * @brief creates a new instance from date and time.
     * @param date date in GMT
     * @param time time of the date
     */
    explicit time_point(datetime::date date, datetime::time_of_day time = {}) noexcept;

    /**
     * @brief creates a new instance from date and time.
     * @param date date in GMT
     * @param time time of the date
     */
    template<class Clock, class Duration>
    explicit time_point(std::chrono::time_point<Clock, Duration> time);

    ~time_point() override = default;
    time_point(time_point const& other) = delete;
    time_point& operator=(time_point const& other) = delete;
    time_point(time_point&& other) noexcept = delete;
    time_point& operator=(time_point&& other) noexcept = delete;

    value_kind kind() const noexcept override;
    time_point* clone(util::object_creator creator) const& override;
    time_point* clone(util::object_creator creator) && override;

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
    friend constexpr bool operator==(time_point const& a, time_point const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend constexpr bool operator!=(time_point const& a, time_point const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, time_point const& value);

protected:
    bool equals(data const& other) const noexcept override;
    std::size_t hash() const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    entity_type entity_;

    friend class util::object_creator;
};

inline constexpr time_point::time_point(time_point::entity_type value) noexcept
    : entity_(value)
{}

inline constexpr time_point::view_type time_point::get() const noexcept {
    return entity_;
}

inline constexpr time_point::operator view_type() const noexcept {
    return get();
}

inline constexpr bool operator==(time_point const& a, time_point const& b) noexcept {
    return a.get() == b.get();
}

inline constexpr bool operator!=(time_point const& a, time_point const& b) noexcept {
    return !(a == b);
}

template<class Clock, class Duration>
time_point::time_point(std::chrono::time_point<Clock, Duration> time)
    : entity_(time)
{}

/**
 * @brief type_of for time_point.
 */
template<> struct type_of<time_point::tag> : util::meta_type<time_point> {};

} // namespace takatori::value

/// @brief provides hash code of takatori::value::time_point.
template<> struct std::hash<takatori::value::time_point> : hash<takatori::value::data> {};
