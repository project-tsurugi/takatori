#include "takatori/plan/broadcast.h"

#include <gtest/gtest.h>

#include "test_utils.h"

#include "takatori/util/clonable.h"

namespace takatori::plan {

class broadcast_test : public ::testing::Test {};

static_assert(broadcast::tag == step_kind::broadcast);

TEST_F(broadcast_test, simple) {
    broadcast s;
    EXPECT_EQ(s.columns().size(), 0);
    EXPECT_EQ(s.input_columns().size(), 0);
    EXPECT_EQ(s.output_columns().size(), 0);
}

TEST_F(broadcast_test, columns) {
    broadcast s {
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
}

TEST_F(broadcast_test, clone) {
    broadcast s {
            vardesc(1),
            vardesc(2),
            vardesc(3),
    };

    auto copy = util::clone_unique(s);
    EXPECT_EQ(s, *copy);
    EXPECT_NE(std::addressof(s), copy.get());
}

TEST_F(broadcast_test, clone_move) {
    broadcast s {
            vardesc(1),
            vardesc(2),
            vardesc(3),
    };

    auto copy = util::clone_unique(s);
    EXPECT_EQ(s, *copy);
    EXPECT_NE(std::addressof(s), copy.get());

    auto move = util::clone_unique(std::move(s));
    EXPECT_NE(std::addressof(s), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(broadcast_test, output) {
    broadcast s {
            vardesc(1),
            vardesc(2),
            vardesc(3),
    };

    std::cout << s << std::endl;
}

} // namespace takatori::plan
