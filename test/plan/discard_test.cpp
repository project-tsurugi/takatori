#include <takatori/plan/discard.h>

#include <gtest/gtest.h>

#include <takatori/util/clonable.h>

namespace takatori::plan {

class discard_test : public ::testing::Test {};

static_assert(discard::tag == step_kind::discard);

TEST_F(discard_test, simple) {
    discard s;
    EXPECT_EQ(s.input_columns().size(), 0);
    EXPECT_EQ(s.output_columns().size(), 0);
}

TEST_F(discard_test, clone) {
    discard s;
    auto copy = util::clone_unique(s);
    EXPECT_EQ(s, *copy);
    EXPECT_NE(std::addressof(s), copy.get());
}

TEST_F(discard_test, clone_move) {
    discard s;

    auto copy = util::clone_unique(s);
    EXPECT_EQ(s, *copy);
    EXPECT_NE(std::addressof(s), copy.get());

    auto move = util::clone_unique(std::move(s));
    EXPECT_NE(std::addressof(s), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(discard_test, output) {
    discard s;

    std::cout << s << std::endl;
}

} // namespace takatori::plan
