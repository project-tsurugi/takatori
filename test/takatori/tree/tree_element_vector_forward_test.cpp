#include <takatori/tree/tree_element_vector_forward.h>

#include "simple_clonable.h"

#include <gtest/gtest.h>

namespace takatori::tree {

class tree_element_vector_forward_test : public ::testing::Test {};

TEST_F(tree_element_vector_forward_test, copy) {
    int parent = 0;
    tree_element_vector<clonable> v { parent };
    v.emplace_back(100);
    v.emplace_back(200);
    v.emplace_back(300);

    auto c = forward(v);
    ASSERT_EQ(c.size(), 3);
    EXPECT_EQ(c[0].count(), 200);
    EXPECT_EQ(c[1].count(), 400);
    EXPECT_EQ(c[2].count(), 600);
}

TEST_F(tree_element_vector_forward_test, move) {
    int parent = 0;
    tree_element_vector<clonable> v { parent };
    v.emplace_back(100);
    v.emplace_back(200);
    v.emplace_back(300);

    auto c = forward(std::move(v));
    ASSERT_EQ(c.size(), 3);
    EXPECT_EQ(c[0].count(), 100);
    EXPECT_EQ(c[1].count(), 200);
    EXPECT_EQ(c[2].count(), 300);
}

} // namespace takatori::tree
