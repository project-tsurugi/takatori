#include "takatori/plan/group.h"

#include <gtest/gtest.h>

#include "test_utils.h"

#include "takatori/util/clonable.h"

namespace takatori::plan {

class group_test : public ::testing::Test {};

static_assert(group::tag == step_kind::group);

TEST_F(group_test, simple) {
    group s;
    EXPECT_EQ(s.input_columns().size(), 0);
    EXPECT_EQ(s.output_columns().size(), 0);
    EXPECT_EQ(s.columns().size(), 0);
    EXPECT_EQ(s.group_keys().size(), 0);
    EXPECT_EQ(s.sort_keys().size(), 0);
    EXPECT_EQ(s.limit(), std::nullopt);
    EXPECT_EQ(s.mode(), group_mode::equivalence);
}

TEST_F(group_test, creator) {
    group s { util::object_creator {} };
    EXPECT_EQ(s.input_columns().size(), 0);
    EXPECT_EQ(s.output_columns().size(), 0);
    EXPECT_EQ(s.columns().size(), 0);
    EXPECT_EQ(s.group_keys().size(), 0);
    EXPECT_EQ(s.sort_keys().size(), 0);
    EXPECT_EQ(s.limit(), std::nullopt);
    EXPECT_EQ(s.mode(), group_mode::equivalence);
}

TEST_F(group_test, columns) {
    group s {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
            {
                    vardesc(2),
                    vardesc(1),
            },
    };
    EXPECT_EQ(s.input_columns().size(), 3);
    EXPECT_EQ(s.output_columns().size(), 3);
    ASSERT_EQ(s.columns().size(), 3);
    EXPECT_EQ(s.columns()[0], vardesc(1));
    EXPECT_EQ(s.columns()[1], vardesc(2));
    EXPECT_EQ(s.columns()[2], vardesc(3));
    EXPECT_TRUE(std::equal(
            s.columns().begin(), s.columns().end(),
            s.input_columns().begin(), s.input_columns().end()));
    EXPECT_TRUE(std::equal(
            s.columns().begin(), s.columns().end(),
            s.output_columns().begin(), s.output_columns().end()));

    ASSERT_EQ(s.group_keys().size(), 2);
    EXPECT_EQ(s.group_keys()[0], vardesc(2));
    EXPECT_EQ(s.group_keys()[1], vardesc(1));

    EXPECT_EQ(s.sort_keys().size(), 0);
    EXPECT_EQ(s.limit(), std::nullopt);
    EXPECT_EQ(s.mode(), group_mode::equivalence);
}

TEST_F(group_test, sort_keys) {
    group s {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
            {},
            {
                    vardesc(1),
                    { vardesc(2), sort_direction::ascendant },
                    { vardesc(3), sort_direction::descendant },
            },
    };
    EXPECT_EQ(s.input_columns().size(), 3);
    EXPECT_EQ(s.output_columns().size(), 3);
    EXPECT_EQ(s.columns().size(), 3);
    EXPECT_EQ(s.group_keys().size(), 0);

    ASSERT_EQ(s.sort_keys().size(), 3);
    {
        auto&& k = s.sort_keys()[0];
        EXPECT_EQ(k.variable(), vardesc(1));
        EXPECT_EQ(k.direction(), sort_direction::ascendant);
    }
    {
        auto&& k = s.sort_keys()[1];
        EXPECT_EQ(k.variable(), vardesc(2));
        EXPECT_EQ(k.direction(), sort_direction::ascendant);
    }
    {
        auto&& k = s.sort_keys()[2];
        EXPECT_EQ(k.variable(), vardesc(3));
        EXPECT_EQ(k.direction(), sort_direction::descendant);
    }

    EXPECT_EQ(s.limit(), std::nullopt);
    EXPECT_EQ(s.mode(), group_mode::equivalence);
}

TEST_F(group_test, limit) {
    group s {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
            {
                    vardesc(1),
            },
            {
                    { vardesc(2), sort_direction::ascendant },
                    { vardesc(3), sort_direction::descendant },
            },
            10,
    };

    EXPECT_EQ(s.input_columns().size(), 3);
    EXPECT_EQ(s.output_columns().size(), 3);
    EXPECT_EQ(s.columns().size(), 3);
    EXPECT_EQ(s.group_keys().size(), 1);
    ASSERT_EQ(s.sort_keys().size(), 2);
    EXPECT_EQ(s.limit(), 10);
    EXPECT_EQ(s.mode(), group_mode::equivalence);
}

TEST_F(group_test, mode) {
    group s {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
            {},
            {
                    vardesc(1),
            },
            {},
            group_mode::equivalence_or_whole,
    };

    EXPECT_EQ(s.input_columns().size(), 3);
    EXPECT_EQ(s.output_columns().size(), 3);
    EXPECT_EQ(s.columns().size(), 3);
    EXPECT_EQ(s.group_keys().size(), 0);
    ASSERT_EQ(s.sort_keys().size(), 1);
    EXPECT_EQ(s.limit(), std::nullopt);
    EXPECT_EQ(s.mode(), group_mode::equivalence_or_whole);
}

TEST_F(group_test, clone) {
    group s {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
            {
                    vardesc(1),
            },
            {
                    { vardesc(2), sort_direction::ascendant },
                    { vardesc(3), sort_direction::descendant },
            },
            10,
            group_mode::equivalence_or_whole,
    };

    auto copy = util::clone_unique(s);
    EXPECT_EQ(s, *copy);
    EXPECT_NE(std::addressof(s), copy.get());
}

TEST_F(group_test, clone_move) {
    group s {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
            {
                    vardesc(1),
            },
            {
                    { vardesc(2), sort_direction::ascendant },
                    { vardesc(3), sort_direction::descendant },
            },
            10,
            group_mode::equivalence_or_whole,
    };

    auto copy = util::clone_unique(s);
    EXPECT_EQ(s, *copy);
    EXPECT_NE(std::addressof(s), copy.get());

    auto move = util::clone_unique(std::move(s));
    EXPECT_NE(std::addressof(s), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(group_test, output) {
    group s {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
            {
                    vardesc(1),
            },
            {
                    { vardesc(2), sort_direction::ascendant },
                    { vardesc(3), sort_direction::descendant },
            },
            10,
            group_mode::equivalence_or_whole,
    };

    std::cout << s << std::endl;
}

} // namespace takatori::plan
