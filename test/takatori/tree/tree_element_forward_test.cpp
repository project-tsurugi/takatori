#include <takatori/tree/tree_element_forward.h>

#include "simple_clonable.h"

#include <gtest/gtest.h>

namespace takatori::tree {

class tree_element_forward_test : public ::testing::Test {};

TEST_F(tree_element_forward_test, copy) {
    auto v = std::make_unique<clonable>(100);
    auto copy = forward(v);

    EXPECT_EQ(copy->count(), 200);
}

TEST_F(tree_element_forward_test, unique_ptr_copy_empty) {
    std::unique_ptr<clonable> v {};
    auto copy = forward(v);

    EXPECT_FALSE(copy);
}

TEST_F(tree_element_forward_test, move) {
    auto v = std::make_unique<clonable>(100);
    auto copy = forward(std::move(v));

    EXPECT_EQ(copy->count(), 100);
}

TEST_F(tree_element_forward_test, move_empty) {
    std::unique_ptr<clonable> v {};
    auto copy = forward(std::move(v));

    EXPECT_FALSE(copy);
}

} // namespace takatori::tree
