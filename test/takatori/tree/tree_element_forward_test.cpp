#include <takatori/tree/tree_element_forward.h>

#include "simple_clonable.h"

#include <gtest/gtest.h>

namespace takatori::tree {

class tree_element_forward_test : public ::testing::Test {
protected:
    template<class T = void>
    static util::object_creator to_creator(util::object_creator::deleter_type<T> const& deleter) {
        return util::get_object_creator_from_deleter(deleter);
    }

    util::pmr::monotonic_buffer_resource resource {};
    util::object_creator custom { &resource };
};

TEST_F(tree_element_forward_test, copy) {
    auto v = std::make_unique<clonable>(100);
    auto copy = forward(custom, v);

    EXPECT_EQ(copy->count(), 200);
    EXPECT_EQ(to_creator(copy.get_deleter()), custom);
}

TEST_F(tree_element_forward_test, unique_ptr_copy_empty) {
    std::unique_ptr<clonable> v {};
    auto copy = forward(custom, v);

    EXPECT_FALSE(copy);
}

TEST_F(tree_element_forward_test, move) {
    auto v = custom.create_unique<clonable>(100);
    auto copy = forward(custom, std::move(v));

    EXPECT_EQ(copy->count(), 100);
    EXPECT_EQ(to_creator(copy.get_deleter()), custom);
}

TEST_F(tree_element_forward_test, move_empty) {
    std::unique_ptr<clonable> v {};
    auto copy = forward(custom, std::move(v));

    EXPECT_FALSE(copy);
}

TEST_F(tree_element_forward_test, move_mismatch) {
    if (!util::object_creator_pmr_enabled) {
        GTEST_SKIP();
    }
    auto v = std::make_unique<clonable>(100);
    auto copy = forward(custom, std::move(v));

    EXPECT_EQ(copy->count(), 101);
    EXPECT_EQ(to_creator(copy.get_deleter()), custom);
}

TEST_F(tree_element_forward_test, move_standard) {
    auto v = std::make_unique<clonable>(100);
    auto copy = forward(util::standard_object_creator(), std::move(v));

    EXPECT_EQ(copy->count(), 100);
    EXPECT_EQ(to_creator(copy.get_deleter()), util::standard_object_creator());
}

} // namespace takatori::tree
