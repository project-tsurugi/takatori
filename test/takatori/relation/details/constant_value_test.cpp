#include <takatori/relation/details/constant_value.h>

#include <gtest/gtest.h>

#include <type_traits>

#include <takatori/testing/descriptors.h>

namespace takatori::relation::details {

static_assert(std::is_copy_constructible_v<constant_value<int>>);
static_assert(std::is_copy_assignable_v<constant_value<int>>);
static_assert(std::is_move_constructible_v<constant_value<int>>);
static_assert(std::is_move_assignable_v<constant_value<int>>);

class constant_value_test : public ::testing::Test {};

TEST_F(constant_value_test, absent) {
    constant_value<int> cv;

    EXPECT_FALSE(cv);

    EXPECT_EQ(cv.kind(), constant_value_kind::absent);
    EXPECT_TRUE(cv.is_absent());
    EXPECT_FALSE(cv.has_immediate());
    EXPECT_FALSE(cv.has_variable());

    EXPECT_EQ(cv.immediate_if(), nullptr);
    EXPECT_EQ(cv.variable_if(), nullptr);

    EXPECT_EQ(cv, constant_value<int> {});
    EXPECT_NE(cv, constant_value<int> { 42 });
    EXPECT_NE(cv, constant_value<int> { testing::vardesc(1) });
}

TEST_F(constant_value_test, absent_const) {
    constant_value<int> const cv { std::nullopt };

    EXPECT_FALSE(cv);

    EXPECT_EQ(cv.kind(), constant_value_kind::absent);
    EXPECT_TRUE(cv.is_absent());
    EXPECT_FALSE(cv.has_immediate());
    EXPECT_FALSE(cv.has_variable());

    EXPECT_EQ(cv.immediate_if(), nullptr);
    EXPECT_EQ(cv.variable_if(), nullptr);

    EXPECT_EQ(cv, constant_value<int> {});
    EXPECT_NE(cv, constant_value<int> { 42 });
    EXPECT_NE(cv, constant_value<int> { testing::vardesc(1) });
}

TEST_F(constant_value_test, immediate) {
    constant_value<int> cv(42);

    EXPECT_TRUE(cv);

    EXPECT_EQ(cv.kind(), constant_value_kind::immediate);
    EXPECT_FALSE(cv.is_absent());
    EXPECT_TRUE(cv.has_immediate());
    EXPECT_FALSE(cv.has_variable());

    EXPECT_EQ(cv.immediate(), 42);
    EXPECT_EQ(cv.immediate_if(), 42);
    EXPECT_EQ(cv.variable_if(), nullptr);

    EXPECT_NE(cv, constant_value<int> {});
    EXPECT_EQ(cv, constant_value<int> { 42 });
    EXPECT_NE(cv, constant_value<int> { 43 });
    EXPECT_NE(cv, constant_value<int> { testing::vardesc(1) });
}

TEST_F(constant_value_test, immediate_const) {
    constant_value<int> const cv(42);

    EXPECT_TRUE(cv);

    EXPECT_EQ(cv.kind(), constant_value_kind::immediate);
    EXPECT_FALSE(cv.is_absent());
    EXPECT_TRUE(cv.has_immediate());
    EXPECT_FALSE(cv.has_variable());

    EXPECT_EQ(cv.immediate(), 42);
    EXPECT_EQ(cv.immediate_if(), 42);
    EXPECT_EQ(cv.variable_if(), nullptr);

    EXPECT_NE(cv, constant_value<int> {});
    EXPECT_EQ(cv, constant_value<int> { 42 });
    EXPECT_NE(cv, constant_value<int> { 43 });
    EXPECT_NE(cv, constant_value<int> { testing::vardesc(1) });
}

TEST_F(constant_value_test, variable) {
    constant_value<int> cv(testing::vardesc(123));

    EXPECT_TRUE(cv);

    EXPECT_EQ(cv.kind(), constant_value_kind::variable);
    EXPECT_FALSE(cv.is_absent());
    EXPECT_FALSE(cv.has_immediate());
    EXPECT_TRUE(cv.has_variable());

    EXPECT_EQ(cv.variable(), testing::vardesc(123));
    EXPECT_EQ(cv.immediate_if(), nullptr);
    EXPECT_EQ(cv.variable_if(), testing::vardesc(123));

    EXPECT_FALSE(cv.immediate_if());

    EXPECT_NE(cv, constant_value<int> {});
    EXPECT_NE(cv, constant_value<int> { 42 });
    EXPECT_EQ(cv, constant_value<int> { testing::vardesc(123) });
    EXPECT_NE(cv, constant_value<int> { testing::vardesc(124) });
}

TEST_F(constant_value_test, variable_const) {
    constant_value<int> const cv(testing::vardesc(123));

    EXPECT_TRUE(cv);

    EXPECT_EQ(cv.kind(), constant_value_kind::variable);
    EXPECT_FALSE(cv.is_absent());
    EXPECT_FALSE(cv.has_immediate());
    EXPECT_TRUE(cv.has_variable());

    EXPECT_EQ(cv.variable(), testing::vardesc(123));
    EXPECT_EQ(cv.immediate_if(), nullptr);
    EXPECT_EQ(cv.variable_if(), testing::vardesc(123));

    EXPECT_FALSE(cv.immediate_if());

    EXPECT_NE(cv, constant_value<int> {});
    EXPECT_NE(cv, constant_value<int> { 42 });
    EXPECT_EQ(cv, constant_value<int> { testing::vardesc(123) });
    EXPECT_NE(cv, constant_value<int> { testing::vardesc(124) });
}

TEST_F(constant_value_test, assign_absent) {
    constant_value<int> cv { 42 };
    cv = std::nullopt;

    EXPECT_EQ(cv, constant_value<int> {});
}

TEST_F(constant_value_test, assign_immediate) {
    constant_value<int> cv {};
    cv = 42;

    EXPECT_EQ(cv, constant_value<int> { 42 });
}

TEST_F(constant_value_test, assign_variable) {
    constant_value<int> cv {};
    cv = testing::vardesc(1);

    EXPECT_EQ(cv, constant_value<int> { testing::vardesc(1) });
}

TEST_F(constant_value_test, output_absent) {
    constant_value<int> cv {};
    std::cout << cv << std::endl;
}

TEST_F(constant_value_test, output_immediate) {
    constant_value<int> cv { 123 };
    std::cout << cv << std::endl;
}

TEST_F(constant_value_test, output_variable) {
    constant_value<int> cv { testing::vardesc(99) };
    std::cout << cv << std::endl;
}

} // namespace takatori::relation::details
