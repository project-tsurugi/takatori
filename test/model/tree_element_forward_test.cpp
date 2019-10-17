#include "model/tree_element_forward.h"

#include "simple_clonable.h"

#include <gtest/gtest.h>

namespace model {

class tree_element_forward_test : public ::testing::Test {
public:
    template<class T>
    static T const& make_const(T& object) { return object; }

protected:
    util::pmr::monotonic_buffer_resource resource {};
    util::object_creator custom { &resource };
};

TEST_F(tree_element_forward_test, copy) {
    auto v = std::make_unique<clonable>(100);
    auto copy = forward(custom, v);

    EXPECT_EQ(copy->count(), 200);
    EXPECT_EQ(copy.get_deleter().creator(), custom);
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
    EXPECT_EQ(copy.get_deleter().creator(), custom);
}

TEST_F(tree_element_forward_test, move_empty) {
    std::unique_ptr<clonable> v {};
    auto copy = forward(custom, std::move(v));

    EXPECT_FALSE(copy);
}

TEST_F(tree_element_forward_test, move_mismatch) {
    auto v = std::make_unique<clonable>(100);
    auto copy = forward(custom, std::move(v));

    EXPECT_EQ(copy->count(), 101);
    EXPECT_EQ(copy.get_deleter().creator(), custom);
}

TEST_F(tree_element_forward_test, move_standard) {
    auto v = std::make_unique<clonable>(100);
    auto copy = forward(util::standard_object_creator(), std::move(v));

    EXPECT_EQ(copy->count(), 100);
    EXPECT_EQ(copy.get_deleter().creator(), util::standard_object_creator());
}

} // namespace model
