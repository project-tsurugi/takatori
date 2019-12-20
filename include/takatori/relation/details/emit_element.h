#pragma once

#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>

#include <takatori/descriptor/variable.h>

#include <takatori/util/object_creator.h>

namespace takatori::relation::details {

/**
 * @brief represents an emit target column.
 */
class emit_element {
public:
    /// @brief the name type.
    using name_type = std::basic_string<char, std::char_traits<char>, util::object_allocator<char>>;

    /**
     * @brief creates a new instance without explicit column name.
     * @param source the emit target column on the input relation
     */
    emit_element(descriptor::variable source) noexcept; // NOLINT

    /**
     * @brief creates a new instance.
     * @param source the emit target column on the input relation
     * @param name the optional column name
     */
    emit_element(descriptor::variable source, name_type name) noexcept;

    /**
     * @brief creates a new instance.
     * @param source the emit target column on the input relation
     * @param name the optional column name
     */
    template<
            class T,
            class = std::enable_if_t<std::is_constructible_v<name_type, T const&>>>
    emit_element(descriptor::variable source, T const& name);

    /**
     * @brief returns the emit target column on the input relation.
     * @return the variable descriptor of emit target
     */
    descriptor::variable const& source() const noexcept;

    /**
     * @brief sets an emit target column on the input relation.
     * @param source the variable descriptor of emit target
     * @return this
     */
    emit_element& source(descriptor::variable source) noexcept;

    /**
     * @brief returns the column name.
     * @return the column name
     * @return optional if the column name is absent
     */
    std::optional<std::string_view> name() const noexcept;

    /**
     * @brief sets a column name.
     * @param name the column name
     * @return this
     */
    emit_element& name(name_type name) noexcept;

    /**
     * @brief sets a column name.
     * @param name the column name
     * @return this
     */
    emit_element& name(std::optional<name_type> name) noexcept;

private:
    descriptor::variable source_;
    std::optional<name_type> name_ {};
};

template<class T, class>
inline emit_element::emit_element(descriptor::variable source, T const& name)
    : source_(std::move(source))
    , name_(std::in_place, name)
{}

/**
 * @brief returns whether or not the two elements are equivalent.
 * @param a the first element
 * @param b the second element
 * @return true if a == b
 * @return false otherwise
 */
bool operator==(emit_element const& a, emit_element const& b) noexcept;

/**
 * @brief returns whether or not the two elements are different.
 * @param a the first element
 * @param b the second element
 * @return true if a != b
 * @return false otherwise
 */
bool operator!=(emit_element const& a, emit_element const& b) noexcept;

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
std::ostream& operator<<(std::ostream& out, emit_element const& value);

} // namespace takatori::relation::details

/**
 * @brief std::hash specialization for takatori::relation::details::emit_column.
 */
template<>
struct std::hash<takatori::relation::details::emit_element> {
    /**
     * @brief compute hash of the given object.
     * @param value the target object
     * @return computed hash code
     */
    std::size_t operator()(takatori::relation::details::emit_element const& value) const noexcept;
};
