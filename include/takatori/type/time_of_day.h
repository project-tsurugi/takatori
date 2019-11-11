#pragma once

#include <optional>

#include "data_type_kind.h"
#include "data_type.h"

#include "takatori/descriptor/time_zone.h"

#include "takatori/util/meta_type.h"

namespace takatori::type {

/**
 * @brief time of day type.
 */
class time_of_day : public data_type {
public:
    /// @brief the kind of this type.
    static constexpr inline data_type_kind tag = data_type_kind::time_of_day;

    /**
     * @brief creates a new instance.
     * @param time_zone an optional time zone information
     */
    explicit time_of_day(std::optional<descriptor::time_zone> time_zone = {}) noexcept;

    ~time_of_day() override = default;
    time_of_day(time_of_day const& other) = delete;
    time_of_day& operator=(time_of_day const& other) = delete;
    time_of_day(time_of_day&& other) noexcept = delete;
    time_of_day& operator=(time_of_day&& other) noexcept = delete;

    data_type_kind kind() const noexcept override;
    time_of_day* clone(util::object_creator creator) const& override;
    time_of_day* clone(util::object_creator creator) && override;

    /**
     * @brief returns time zone information.
     * @return time zone information if this object represents the zoned time
     * @return empty if this object does not have time zone information
     */
    std::optional<descriptor::time_zone> const& time_zone() const noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(time_of_day const& a, time_of_day const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend inline bool operator!=(time_of_day const& a, time_of_day const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, time_of_day const& value);

protected:
    bool equals(data_type const& other) const noexcept override;
    std::size_t hash() const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;
    
private:
    std::optional<descriptor::time_zone> time_zone_;
};

template<> struct data_type_kind_type<time_of_day::tag> : util::meta_type<time_of_day> {};

} // namespace takatori::type

namespace std {

/// @brief provides hash code of takatori::type::time_of_day.
template<> struct hash<takatori::type::time_of_day> : hash<takatori::type::data_type> {};

} // namespace std
