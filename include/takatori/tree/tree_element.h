#pragma once

namespace takatori::tree {

/**
 * @brief a generic tree node.
 * @see tree_element_traits
 */
class tree_element {
public:
    /**
     * @brief destroys this object.
     */
    virtual ~tree_element() = default;

    /**
     * @brief returns the parent element.
     * @return the parent element
     * @return nullptr if this is the root or orphaned element
     */
    virtual tree_element* parent_element() noexcept = 0;

    /**
     * @brief returns the parent element.
     * @return the parent element
     * @return nullptr if this is the root or orphaned element
     */
    virtual tree_element const* parent_element() const noexcept = 0;

protected:
    /**
     * @brief creates a new instance.
     */
    tree_element() = default;

    /**
     * @brief creates a new instance.
     * @param other the copy source
     */
    tree_element(tree_element const& other) = default;

    /**
     * @brief assigns the given object.
     * @param other the copy source
     * @return this
     */
    tree_element& operator=(tree_element const& other) = default;

    /**
     * @brief creates a new instance.
     * @param other the move source
     */
    tree_element(tree_element&& other) noexcept = default;

    /**
     * @brief assigns the given object.
     * @param other the move source
     * @return this
     */
    tree_element& operator=(tree_element&& other) noexcept = default;
};

} // namespace takatori::tree
