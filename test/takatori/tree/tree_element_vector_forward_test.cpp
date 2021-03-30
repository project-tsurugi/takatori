#include <takatori/tree/tree_element_vector_forward.h>

#include "simple_clonable.h"

#include <gtest/gtest.h>

namespace takatori::tree {

class tree_element_vector_forward_test : public ::testing::Test {
protected:
    util::pmr::monotonic_buffer_resource resource {};
    util::object_creator custom { &resource };
};

TEST_F(tree_element_vector_forward_test, copy) {
    int parent = 0;
    tree_element_vector<clonable> v { parent };
    v.emplace_back(100);
    v.emplace_back(200);
    v.emplace_back(300);

    auto c = forward(custom, v);
    ASSERT_EQ(c.size(), 3);
    EXPECT_EQ(c[0].count(), 200);
    EXPECT_EQ(c[1].count(), 400);
    EXPECT_EQ(c[2].count(), 600);

    EXPECT_EQ(c.get_object_creator(), custom);
}

TEST_F(tree_element_vector_forward_test, move) {
    int parent = 0;
    tree_element_vector<clonable> v { parent, custom };
    v.emplace_back(100);
    v.emplace_back(200);
    v.emplace_back(300);

    auto c = forward(custom, std::move(v));
    ASSERT_EQ(c.size(), 3);
    EXPECT_EQ(c[0].count(), 100);
    EXPECT_EQ(c[1].count(), 200);
    EXPECT_EQ(c[2].count(), 300);

    EXPECT_EQ(c.get_object_creator(), custom);
}

TEST_F(tree_element_vector_forward_test, move_mismatch) {
    if (!util::object_creator_pmr_enabled) {
        GTEST_SKIP();
    }
    int parent = 0;
    tree_element_vector<clonable> v { parent };
    v.emplace_back(100);
    v.emplace_back(200);
    v.emplace_back(300);

    auto c = forward(custom, std::move(v));
    ASSERT_EQ(c.size(), 3);
    EXPECT_EQ(c[0].count(), 101);
    EXPECT_EQ(c[1].count(), 201);
    EXPECT_EQ(c[2].count(), 301);

    EXPECT_EQ(c.get_object_creator(), custom);
}

} // namespace takatori::tree
