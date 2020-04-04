#include <takatori/tree/tree_element.h>
#include <takatori/tree/tree_element_traits.h>
#include <takatori/tree/tree_fragment_traits.h>

#include <type_traits>

#include <gtest/gtest.h>

#include "generic_tree.h"

namespace takatori::tree {

class tree_element_test : public ::testing::Test {};

static_assert(is_tree_element_v<tree_element>);
static_assert(is_tree_element_v<node<int>>);
static_assert(is_tree_element_v<branch<int>>);
static_assert(is_tree_element_v<leaf<int>>);

using traits = tree_element_traits<tree_element>;
using node_traits = tree_element_traits<node<int>>;
using branch_traits = tree_element_traits<branch<int>>;
using leaf_traits = tree_element_traits<leaf<int>>;

using const_node_traits = tree_element_traits<node<int> const>;

static_assert(std::is_same_v<typename traits::element_type, tree_element>);
static_assert(std::is_same_v<typename traits::parent_type, tree_element>);
static_assert(traits::can_get_mutable_parent);
static_assert(!is_tree_fragment_v<tree_element>);

static_assert(std::is_same_v<typename node_traits::element_type, node<int>>);
static_assert(std::is_same_v<typename node_traits::parent_type, node<int>>);
static_assert(node_traits::can_get_mutable_parent);
static_assert(is_tree_fragment_v<node<int>>);

static_assert(std::is_same_v<typename branch_traits::element_type, branch<int>>);
static_assert(std::is_same_v<typename branch_traits::parent_type, node<int>>);
static_assert(branch_traits::can_get_mutable_parent);
static_assert(is_tree_fragment_v<branch<int>>);

static_assert(std::is_same_v<typename leaf_traits::element_type, leaf<int>>);
static_assert(std::is_same_v<typename leaf_traits::parent_type, node<int>>);
static_assert(leaf_traits::can_get_mutable_parent);
static_assert(is_tree_fragment_v<leaf<int>>);

static_assert(std::is_same_v<typename const_node_traits::element_type, node<int> const>);
static_assert(std::is_same_v<typename const_node_traits::parent_type, node<int>>);
static_assert(!const_node_traits::can_get_mutable_parent);
static_assert(!is_tree_fragment_v<node<int> const>);

using fragment_traits = tree_fragment_traits<node<int>>;

TEST_F(tree_element_test, parents) {
    branch<int> root {
            std::make_unique<leaf<int>>(1),
            std::make_unique<leaf<int>>(2),
    };
    fragment_traits::set_parent_element(*root.left(), root);
    fragment_traits::set_parent_element(*root.right(), root);

    EXPECT_EQ(node_traits::get_parent_element(*root.left()).get(), &root);
    EXPECT_EQ(node_traits::get_parent_element(*root.right()).get(), &root);
}

TEST_F(tree_element_test, parents_const) {
    branch<int> root {
            std::make_unique<leaf<int>>(1),
            std::make_unique<leaf<int>>(2),
    };
    fragment_traits::set_parent_element(*root.left(), root);
    fragment_traits::set_parent_element(*root.right(), root);

    auto& croot = std::as_const(root);

    EXPECT_EQ(node_traits::get_parent_element(*root.left()).get(), &croot);
    EXPECT_EQ(node_traits::get_parent_element(*root.right()).get(), &croot);
}

} // namespace takatori::tree
