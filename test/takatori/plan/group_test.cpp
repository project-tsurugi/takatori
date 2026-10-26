#include <takatori/plan/group.h>

#include <gtest/gtest.h>

#include <stdexcept>
#include <type_traits>

#include "test_utils.h"

#include <takatori/util/clonable.h>

namespace takatori::plan {

class group_test : public ::testing::Test {};

static_assert(group::tag == step_kind::group);

using relation::details::row_slice;

TEST_F(group_test, simple) {
    group s;
    EXPECT_EQ(s.input_columns().size(), 0);
    EXPECT_EQ(s.output_columns().size(), 0);
    EXPECT_EQ(s.columns().size(), 0);
    EXPECT_EQ(s.group_keys().size(), 0);
    EXPECT_EQ(s.sort_keys().size(), 0);
    EXPECT_EQ(s.row_slice(), row_slice {});
    EXPECT_EQ(s.mode(), group_mode::equivalence);
}

// compatibility API: the row count must be an integral type
static_assert(std::is_same_v<group::size_type, std::size_t>);
static_assert(std::is_constructible_v<group,
        std::vector<descriptor::variable>, std::vector<descriptor::variable>, std::vector<group::sort_key>, int>);
static_assert(!std::is_constructible_v<group,
        std::vector<descriptor::variable>, std::vector<descriptor::variable>, std::vector<group::sort_key>, std::nullopt_t>);
static_assert(!std::is_constructible_v<group,
        std::vector<descriptor::variable>, std::vector<descriptor::variable>, std::vector<group::sort_key>, std::optional<std::size_t>>);

TEST_F(group_test, compat_limit) {
    group s {
            {
                    vardesc(1),
                    vardesc(2),
            },
            {
                    vardesc(1),
            },
            {
                    vardesc(2),
            },
            5,
    };
    EXPECT_EQ(s.group_keys().size(), 1);
    EXPECT_EQ(s.sort_keys().size(), 1);
    EXPECT_EQ(s.row_slice(), (row_slice { {}, 5 }));
    EXPECT_EQ(s.limit(), 5);
    EXPECT_EQ(s.mode(), group_mode::equivalence);
}

TEST_F(group_test, compat_limit_mode) {
    group s {
            {
                    vardesc(1),
            },
            {},
            {},
            5,
            group_mode::equivalence_or_whole,
    };
    EXPECT_EQ(s.row_slice(), (row_slice { {}, 5 }));
    EXPECT_EQ(s.mode(), group_mode::equivalence_or_whole);
}

TEST_F(group_test, compat_limit_vector) {
    std::vector<descriptor::variable> columns { vardesc(1), vardesc(2) };
    std::vector<descriptor::variable> keys { vardesc(1) };
    std::vector<group::sort_key> sorts { vardesc(2) };
    group s { std::move(columns), std::move(keys), std::move(sorts), 5 };
    EXPECT_EQ(s.row_slice(), (row_slice { {}, 5 }));
}

TEST_F(group_test, compat_limit_brace) {
    std::vector<descriptor::variable> columns { vardesc(1), vardesc(2) };
    std::vector<descriptor::variable> keys { vardesc(1) };
    std::vector<group::sort_key> sorts { vardesc(2) };
    // `{}` must be resolved as an empty row slice (unlimited), as same as `std::optional` in the previous API
    group s { std::move(columns), std::move(keys), std::move(sorts), {}, group_mode::equivalence_or_whole };
    EXPECT_EQ(s.row_slice(), row_slice {});
    EXPECT_EQ(s.limit(), std::nullopt);
    EXPECT_EQ(s.mode(), group_mode::equivalence_or_whole);
}

TEST_F(group_test, compat_limit_no_limit) {
    // the previous calls without limit must be resolved to the new constructor
    group s {
            {
                    vardesc(1),
            },
            {
                    vardesc(1),
            },
    };
    EXPECT_EQ(s.row_slice(), row_slice {});
}

TEST_F(group_test, compat_limit_getter) {
    group s {};
    EXPECT_EQ(s.limit(), std::nullopt);

    s.row_slice() = { 0, 5 };
    EXPECT_EQ(s.limit(), 5);

    s.row_slice() = { 1, 5 };
    EXPECT_THROW((void) s.limit(), std::domain_error);

    s.row_slice() = { {}, vardesc(2) };
    EXPECT_THROW((void) s.limit(), std::domain_error);
}

TEST_F(group_test, compat_limit_setter) {
    group s {};
    s.row_slice() = { 10, {} };

    s.limit(5);
    EXPECT_EQ(s.row_slice(), (row_slice { 10, 5 }));

    s.limit(std::nullopt);
    EXPECT_EQ(s.row_slice(), (row_slice { 10, {} }));
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
    EXPECT_EQ(s.row_slice(), row_slice {});
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

    EXPECT_EQ(s.row_slice(), row_slice {});
    EXPECT_EQ(s.mode(), group_mode::equivalence);
}

TEST_F(group_test, row_slice) {
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
            {
                    10,
                    5,
            },
    };

    EXPECT_EQ(s.input_columns().size(), 3);
    EXPECT_EQ(s.output_columns().size(), 3);
    EXPECT_EQ(s.columns().size(), 3);
    EXPECT_EQ(s.group_keys().size(), 1);
    ASSERT_EQ(s.sort_keys().size(), 2);
    EXPECT_EQ(s.row_slice(), (row_slice {
            10,
            5,
    }));
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
    EXPECT_EQ(s.row_slice(), row_slice {});
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
            {
                    10,
                    5,
            },
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
            {
                    10,
                    5,
            },
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
            {
                    10,
                    5,
            },
            group_mode::equivalence_or_whole,
    };

    std::cout << s << std::endl;
}

} // namespace takatori::plan
