#include <takatori/plan/forward.h>

#include <gtest/gtest.h>

#include "test_utils.h"

#include <takatori/util/clonable.h>

namespace takatori::plan {

class forward_test : public ::testing::Test {};

static_assert(forward::tag == step_kind::forward);

TEST_F(forward_test, simple) {
    forward s;
    EXPECT_EQ(s.columns().size(), 0);
    EXPECT_EQ(s.input_columns().size(), 0);
    EXPECT_EQ(s.output_columns().size(), 0);
    EXPECT_EQ(s.limit(), std::nullopt);
}

TEST_F(forward_test, limits) {
    forward s { 100 };
    EXPECT_EQ(s.columns().size(), 0);
    EXPECT_EQ(s.input_columns().size(), 0);
    EXPECT_EQ(s.output_columns().size(), 0);
    EXPECT_EQ(s.limit(), 100);
}

TEST_F(forward_test, columns) {
    forward s {
            vardesc(1),
            vardesc(2),
            vardesc(3),
    };

    ASSERT_EQ(s.columns().size(), 3);
    {
        auto&& cs = s.columns();
        EXPECT_EQ(cs[0], vardesc(1));
        EXPECT_EQ(cs[1], vardesc(2));
        EXPECT_EQ(cs[2], vardesc(3));
    }

    ASSERT_EQ(s.input_columns().size(), 3);
    {
        auto&& cs = s.input_columns();
        EXPECT_EQ(cs[0], vardesc(1));
        EXPECT_EQ(cs[1], vardesc(2));
        EXPECT_EQ(cs[2], vardesc(3));
    }

    ASSERT_EQ(s.output_columns().size(), 3);
    {
        auto&& cs = s.output_columns();
        EXPECT_EQ(cs[0], vardesc(1));
        EXPECT_EQ(cs[1], vardesc(2));
        EXPECT_EQ(cs[2], vardesc(3));
    }

    EXPECT_EQ(s.limit(), std::nullopt);
}

TEST_F(forward_test, clone) {
    forward s {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
            10,
    };

    auto copy = util::clone_unique(s);
    EXPECT_EQ(s, *copy);
    EXPECT_NE(std::addressof(s), copy.get());
}

TEST_F(forward_test, clone_move) {
    forward s {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
            10,
    };

    auto copy = util::clone_unique(s);
    EXPECT_EQ(s, *copy);
    EXPECT_NE(std::addressof(s), copy.get());

    auto move = util::clone_unique(std::move(s));
    EXPECT_NE(std::addressof(s), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(forward_test, output) {
    forward s {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
            10,
    };

    std::cout << s << std::endl;
}

} // namespace takatori::plan
