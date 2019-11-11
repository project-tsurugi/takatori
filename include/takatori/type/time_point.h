#pragma once

#include <optional>

#include "data_type_kind.h"
#include "data_type.h"

#include "takatori/descriptor/time_zone.h"

#include "takatori/util/meta_type.h"

namespace takatori::type {

/**
 * @brief time point type.
 */
class time_point : public data_type {
public:
    /// @brief the kind of this type.
    static constexpr inline data_type_kind tag = data_type_kind::time_point;

    /**
     * @brief creates a new instance.
     * @param time_zone an optional time zone information
     */
    explicit time_point(std::optional<descriptor::time_zone> time_zone = {}) noexcept;

    ~time_point() override = default;
    time_point(time_point const& other) = delete;
    time_point& operator=(time_point const& other) = delete;
    time_point(time_point&& other) noexcept = delete;
    time_point& operator=(time_point&& other) noexcept = delete;

    data_type_kind kind() const noexcept override;
    time_point* clone(util::object_creator creator) const& override;
    time_point* clone(util::object_creator creator) && override;

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
    friend bool operator==(time_point const& a, time_point const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend inline bool operator!=(time_point const& a, time_point const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, time_point const& value);

protected:
    bool equals(data_type const& other) const noexcept override;
    std::size_t hash() const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;
    
private:
    std::optional<descriptor::time_zone> time_zone_;
};

template<> struct data_type_kind_type<time_point::tag> : util::meta_type<time_point> {};

} // namespace takatori::type

namespace std {

/// @brief provides hash code of takatori::type::time_point.
template<> struct hash<takatori::type::time_point> : hash<takatori::type::data_type> {};

} // namespace std
