#pragma once

#include <initializer_list>
#include <ostream>
#include <vector>

#include <takatori/tree/tree_element_vector.h>

#include <takatori/scalar/expression.h>

#include <takatori/util/clone_tag.h>
#include <takatori/util/rvalue_reference_wrapper.h>

namespace takatori::statement {

class write;

} // namespace takatori::statement

namespace takatori::statement::details {

/**
 * @brief a tuple of values to write.
 */
class write_tuple {
public:
    /// @brief the fragment owner type.
    using parent_type = write;

    /**
     * @brief creates a new object.
     * @param elements the tuple elements
     */
    explicit write_tuple(util::reference_vector<scalar::expression> elements) noexcept;

    /**
     * @brief creates a new instance.
     * @param elements the tuple elements
     * @attention this may take copies of given expressions
     */
    write_tuple(std::initializer_list<util::rvalue_reference_wrapper<scalar::expression>> elements) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit write_tuple(util::clone_tag_t, write_tuple const& other);

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit write_tuple(util::clone_tag_t, write_tuple&& other);

    ~write_tuple() = default;

    write_tuple(write_tuple const& other) = delete;
    write_tuple& operator=(write_tuple const& other) = delete;

    /**
     * @brief creates a new object.
     * @param other the move source
     * @attention this is designed for the STL containers, developers should not call this directly
     */
    write_tuple(write_tuple&& other) noexcept = default;

    /**
     * @brief assigns the given object.
     * @param other the move source
     * @return this
     * @attention this is designed for the STL containers, developers should not call this directly
     */
    write_tuple& operator=(write_tuple&& other) noexcept = default;

    /**
     * @brief returns the owner of this fragment.
     * @return the owner
     * @return nullptr if it is absent
     */
    [[nodiscard]] parent_type* parent_element() noexcept;

    /// @copydoc parent_element()
    [[nodiscard]] parent_type const* parent_element() const noexcept;

    /**
     * @brief sets the owner of this fragment.
     * @param parent the owner
     */
    void parent_element(parent_type* parent) noexcept;

    /**
     * @brief returns the tuple elements.
     * @return the tuple elements
     */
    [[nodiscard]] tree::tree_element_vector<scalar::expression>& elements() noexcept;

    /// @copydoc elements()
    [[nodiscard]] tree::tree_element_vector<scalar::expression> const& elements() const noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, write_tuple const& value);

private:
    tree::tree_element_vector<scalar::expression> elements_;

    parent_type* parent_ {};

    friend class write;
};

/**
 * @brief returns whether or not the two elements are equivalent.
 * @param a the first element
 * @param b the second element
 * @return true if a == b
 * @return false otherwise
 */
bool operator==(write_tuple const& a, write_tuple const& b) noexcept;

/**
 * @brief returns whether or not the two elements are different.
 * @param a the first element
 * @param b the second element
 * @return true if a != b
 * @return false otherwise
 */
bool operator!=(write_tuple const& a, write_tuple const& b) noexcept;

} // namespace takatori::statement::details
