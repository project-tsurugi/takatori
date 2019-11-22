#pragma once

#include <iostream>
#include <string>
#include <string_view>

#include "value_kind.h"
#include "simple_value.h"

#include "takatori/util/meta_type.h"
#include "takatori/util/object_creator.h"

namespace takatori::value {

/**
 * @brief a character sequence as polymorphic value model.
 */
class character : public data {
public:
    /// @brief the kind of this type.
    static constexpr inline value_kind tag = value_kind::character;

    /// @brief the allocator type
    using allocator_type = util::object_allocator<char>;

    /// @brief the entity type
    using entity_type = std::basic_string<char, std::char_traits<char>, allocator_type>;

    /// @brief the view type
    using view_type = std::string_view;

    /**
     * @brief creates a new instance.
     * @param value the character string
     */
    explicit character(entity_type value) noexcept;

    /**
     * @brief creates a new instance.
     * @tparam T the character like object
     * @param value the character string
     * @param allocator the allocator
     */
    template<class T>
    explicit inline character(T const& value, allocator_type const& allocator = {});

    ~character() override = default;
    character(character const& other) = delete;
    character& operator=(character const& other) = delete;
    character(character&& other) noexcept = delete;
    character& operator=(character&& other) noexcept = delete;

    value_kind kind() const noexcept override;
    character* clone(util::object_creator creator) const& override;
    character* clone(util::object_creator creator) && override;

    /**
     * @brief returns the entity value.
     * @return the entity value
     */
    view_type get() const noexcept;

    /// @copydoc get()
    explicit operator view_type() const noexcept;

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(character const& a, character const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(character const& a, character const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, character const& value);

protected:
    bool equals(data const& other) const noexcept override;
    std::size_t hash() const noexcept override;
    std::ostream& print_to(std::ostream& out) const override;

private:
    entity_type entity_;

    friend class util::object_creator;
};

template<class T>
inline
character::character(T const& value, allocator_type const& allocator)
    : entity_(value, allocator)
{}

/**
 * @brief type_of for character.
 */
template<> struct type_of<character::tag> : util::meta_type<character> {};

} // namespace takatori::value

/// @brief provides hash code of takatori::value::character.
template<> struct std::hash<takatori::value::character> : hash<takatori::value::data> {};
