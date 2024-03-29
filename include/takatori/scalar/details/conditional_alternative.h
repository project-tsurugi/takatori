#pragma once

#include <ostream>
#include <string>
#include <string_view>
#include <vector>

#include <takatori/scalar/expression.h>

#include <takatori/util/clone_tag.h>
#include <takatori/util/optional_ptr.h>
#include <takatori/util/ownership_reference.h>

namespace takatori::scalar {

class conditional;

} // namespace takatori::scalar

namespace takatori::scalar::details {

/**
 * @brief an alternative clause in conditional expression.
 */
class conditional_alternative {
public:
    /// @brief the fragment owner type.
    using parent_type = conditional;

    /**
     * @brief creates a new object.
     * @param other the move source
     * @attention this is designed for the STL containers, developers should not call this directly
     */
    conditional_alternative(conditional_alternative&& other) noexcept = default;

    /**
     * @brief assigns the given object.
     * @param other the move source
     * @return this
     * @attention this is designed for the STL containers, developers should not call this directly
     */
    conditional_alternative& operator=(conditional_alternative&& other) noexcept = default;

    /**
     * @brief creates a new instance.
     * @param condition the condition expression
     * @param body the body expression
     */
    explicit conditional_alternative(
            std::unique_ptr<expression> condition,
            std::unique_ptr<expression> body) noexcept;

    /**
     * @brief creates a new instance.
     * @param condition the condition expression
     * @param body the body expression
     * @attention this may take copies of given expressions
     */
    explicit conditional_alternative(
            expression&& condition,
            expression&& body) noexcept;

    /**
     * @brief creates a new object.
     * @param other the copy source
     */
    explicit conditional_alternative(util::clone_tag_t, conditional_alternative const& other);

    /**
     * @brief creates a new object.
     * @param other the move source
     */
    explicit conditional_alternative(util::clone_tag_t, conditional_alternative&& other);

    ~conditional_alternative() = default;

    conditional_alternative(conditional_alternative const& other) = delete;
    conditional_alternative& operator=(conditional_alternative const& other) = delete;

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
     * @brief returns the condition expression.
     * @return the condition expression
     */
    [[nodiscard]] expression& condition() noexcept;
    
    /// @copydoc condition
    [[nodiscard]] expression const& condition() const noexcept;

    /**
     * @brief returns the condition expression.
     * @return the condition expression
     * @return empty if the condition expression is absent
     */
    [[nodiscard]] util::optional_ptr<expression> optional_condition() noexcept;
    
    /// @copydoc optional_condition()
    [[nodiscard]] util::optional_ptr<expression const> optional_condition() const noexcept;
    
    /**
     * @brief sets the condition expression.
     * @param condition the condition expression
     * @return this
     */
    conditional_alternative& condition(std::unique_ptr<expression> condition) noexcept;

    /**
     * @brief releases the condition expression.
     * @return the released expression
     */
    [[nodiscard]] std::unique_ptr<expression> release_condition() noexcept;

    /**
     * @brief returns ownership reference of the condition expression.
     * @return the condition expression
     */
    [[nodiscard]] util::ownership_reference<expression> ownership_condition();

    /**
     * @brief returns the body expression.
     * @return the body expression
     */
    [[nodiscard]] expression& body() noexcept;

    /// @copydoc body
    [[nodiscard]] expression const& body() const noexcept;

    /**
     * @brief returns the body expression.
     * @return the body expression
     * @return empty if the body expression is absent
     */
    [[nodiscard]] util::optional_ptr<expression> optional_body() noexcept;

    /// @copydoc optional_body()
    [[nodiscard]] util::optional_ptr<expression const> optional_body() const noexcept;

    /**
     * @brief sets the body expression.
     * @param body the body expression
     * @return this
     */
    conditional_alternative& body(std::unique_ptr<expression> body) noexcept;

    /**
     * @brief releases the body expression.
     * @return the released expression
     */
    [[nodiscard]] std::unique_ptr<expression> release_body() noexcept;

    /**
     * @brief returns ownership reference of the body expression.
     * @return the body expression
     */
    [[nodiscard]] util::ownership_reference<expression> ownership_body();

    /**
     * @brief returns whether or not the two elements are equivalent.
     * @param a the first element
     * @param b the second element
     * @return true if a == b
     * @return false otherwise
     */
    friend bool operator==(conditional_alternative const& a, conditional_alternative const& b) noexcept;

    /**
     * @brief returns whether or not the two elements are different.
     * @param a the first element
     * @param b the second element
     * @return true if a != b
     * @return false otherwise
     */
    friend bool operator!=(conditional_alternative const& a, conditional_alternative const& b) noexcept;

    /**
     * @brief appends string representation of the given value.
     * @param out the target output
     * @param value the target value
     * @return the output
     */
    friend std::ostream& operator<<(std::ostream& out, conditional_alternative const& value);

private:
    std::unique_ptr<expression> condition_;
    std::unique_ptr<expression> body_;

    parent_type* parent_ {};

    friend class conditional;
};

} // namespace takatori::scalar::details
