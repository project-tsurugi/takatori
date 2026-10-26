#include <takatori/relation/details/row_slice.h>

#include <gtest/gtest.h>

#include <iostream>
#include <type_traits>

#include <takatori/testing/descriptors.h>

namespace takatori::relation::details {

static_assert(std::is_default_constructible_v<row_slice>);
static_assert(std::is_constructible_v<row_slice, row_slice::position_type, row_slice::size_type>);

// a single value must not be accepted, to avoid mistaking the row count for the start row position
static_assert(!std::is_constructible_v<row_slice, std::size_t>);
static_assert(!std::is_constructible_v<row_slice, row_slice::position_type>);
static_assert(!std::is_convertible_v<std::size_t, row_slice>);

class row_slice_test : public ::testing::Test {};

TEST_F(row_slice_test, simple) {
    row_slice s {};

    EXPECT_FALSE(s);
    EXPECT_TRUE(s.start().is_absent());
    EXPECT_TRUE(s.count().is_absent());
}

TEST_F(row_slice_test, start_count) {
    row_slice s { 10, 20 };

    EXPECT_TRUE(s);
    EXPECT_EQ(s.start(), row_slice::position_type { 10 });
    EXPECT_EQ(s.count(), row_slice::size_type { 20 });
}

TEST_F(row_slice_test, count_only) {
    row_slice s { {}, 20 };

    EXPECT_TRUE(s);
    EXPECT_TRUE(s.start().is_absent());
    EXPECT_EQ(s.count(), row_slice::size_type { 20 });

    EXPECT_EQ(s, (row_slice { std::nullopt, 20 }));
}

TEST_F(row_slice_test, start_only) {
    row_slice s { 10, {} };

    EXPECT_TRUE(s);
    EXPECT_EQ(s.start(), row_slice::position_type { 10 });
    EXPECT_TRUE(s.count().is_absent());
}

TEST_F(row_slice_test, count_zero) {
    row_slice s { {}, 0 };

    // count = 0 is a valid specification, which indicates no rows
    EXPECT_TRUE(s);
}

TEST_F(row_slice_test, start_zero) {
    row_slice s { 0, {} };

    // start = 0 is treated as same as empty
    EXPECT_FALSE(s);

    // but they are distinguished in comparison
    EXPECT_NE(s, row_slice {});
}

TEST_F(row_slice_test, start_zero_count) {
    row_slice s { 0, 20 };

    EXPECT_TRUE(s);
}

TEST_F(row_slice_test, variable) {
    row_slice s { testing::vardesc(1), testing::vardesc(2) };

    EXPECT_TRUE(s);
    EXPECT_TRUE(s.start().has_variable());
    EXPECT_TRUE(s.count().has_variable());
}

TEST_F(row_slice_test, variable_start_only) {
    row_slice s { testing::vardesc(1), {} };

    EXPECT_TRUE(s);
}

TEST_F(row_slice_test, compare) {
    row_slice a { 10, 20 };
    row_slice b { 10, 20 };
    row_slice c { 10, 21 };
    row_slice d { 11, 20 };

    EXPECT_EQ(a, b);
    EXPECT_NE(a, c);
    EXPECT_NE(a, d);
}

TEST_F(row_slice_test, output) {
    row_slice s { 10, 20 };

    std::cout << s << std::endl;
}

} // namespace takatori::relation::details
