#pragma once

namespace takatori::tree {

/**
 * @brief a generic tree node.
 */
class tree_element_base {
public:
    /**
     * @brief destroys this object.
     */
    virtual ~tree_element_base() = default;

protected:
    /**
     * @brief creates a new instance.
     */
    tree_element_base() = default;

    /**
     * @brief creates a new instance.
     * @param other the copy source
     */
    tree_element_base(tree_element_base const& other) = default;

    /**
     * @brief assigns the given object.
     * @param other the copy source
     * @return this
     */
    tree_element_base& operator=(tree_element_base const& other) = default;

    /**
     * @brief creates a new instance.
     * @param other the move source
     */
    tree_element_base(tree_element_base&& other) noexcept = default;

    /**
     * @brief assigns the given object.
     * @param other the move source
     * @return this
     */
    tree_element_base& operator=(tree_element_base&& other) noexcept = default;
};

} // namespace takatori::tree
