#include <takatori/relation/intermediate/limit.h>

#include <stdexcept>
#include <type_traits>

#include <gtest/gtest.h>

#include <takatori/relation/test_utils.h>

#include <takatori/util/clonable.h>

namespace takatori::relation::intermediate {

class limit_relation_test : public ::testing::Test {};

static_assert(limit::tag == expression_kind::limit_relation);
static_assert(std::is_same_v<type_of_t<limit::tag>, limit>);
static_assert(is_available_in_intermediate_plan(limit::tag));
static_assert(!is_available_in_step_plan(limit::tag));

using row_slice = details::row_slice;

TEST_F(limit_relation_test, simple) {
    limit expr {};

    ASSERT_EQ(expr.group_keys().size(), 0);
    ASSERT_EQ(expr.sort_keys().size(), 0);
    EXPECT_FALSE(expr.row_slice());

    {
        auto p = expr.input_ports();
        ASSERT_EQ(p.size(), 1);
        EXPECT_EQ(&p[0], &expr.input());
        EXPECT_TRUE(is_valid_port_list(p));
    }
    {
        auto p = expr.output_ports();
        ASSERT_EQ(p.size(), 1);
        EXPECT_EQ(&p[0], &expr.output());
        EXPECT_TRUE(is_valid_port_list(p));
    }

    EXPECT_EQ(&expr.input().owner(), &expr);
    EXPECT_EQ(&expr.output().owner(), &expr);
}

TEST_F(limit_relation_test, group_keys) {
    limit expr {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
    };

    ASSERT_EQ(expr.group_keys().size(), 3);
    EXPECT_EQ(expr.group_keys()[0], vardesc(1));
    EXPECT_EQ(expr.group_keys()[1], vardesc(2));
    EXPECT_EQ(expr.group_keys()[2], vardesc(3));
    EXPECT_FALSE(expr.row_slice());
}

TEST_F(limit_relation_test, sort_keys) {
    limit expr {
            {},
            {
                    { vardesc(10) },
                    { vardesc(20), sort_direction::descendant },
                    { vardesc(30), sort_direction::ascendant },
            },
    };

    ASSERT_EQ(expr.sort_keys().size(), 3);
    {
        auto&& k = expr.sort_keys()[0];
        EXPECT_EQ(k.variable(), vardesc(10));
        EXPECT_EQ(k.direction(), sort_direction::ascendant);
    }
    {
        auto&& k = expr.sort_keys()[1];
        EXPECT_EQ(k.variable(), vardesc(20));
        EXPECT_EQ(k.direction(), sort_direction::descendant);
    }
    {
        auto&& k = expr.sort_keys()[2];
        EXPECT_EQ(k.variable(), vardesc(30));
        EXPECT_EQ(k.direction(), sort_direction::ascendant);
    }
    EXPECT_FALSE(expr.row_slice());
}

TEST_F(limit_relation_test, row_slice) {
    limit expr {
            {
                    10,
                    5,
            },
    };

    ASSERT_EQ(expr.group_keys().size(), 0);
    ASSERT_EQ(expr.sort_keys().size(), 0);
    EXPECT_EQ(expr.row_slice(), (row_slice {
            10,
            5,
    }));
}

// compatibility API: the row count accepts std::optional as same as the previous API
static_assert(std::is_same_v<limit::size_type, std::size_t>);
static_assert(std::is_constructible_v<limit, int>);
static_assert(std::is_constructible_v<limit, std::nullopt_t>);
static_assert(std::is_constructible_v<limit, std::optional<std::size_t>>);

TEST_F(limit_relation_test, compat_count) {
    limit expr { 5 };

    ASSERT_EQ(expr.group_keys().size(), 0);
    ASSERT_EQ(expr.sort_keys().size(), 0);
    EXPECT_EQ(expr.row_slice(), (row_slice { {}, 5 }));
    EXPECT_EQ(expr.count(), 5);
}

TEST_F(limit_relation_test, compat_count_keys) {
    limit expr {
            5,
            {
                    vardesc(1),
            },
            {
                    vardesc(2),
            },
    };

    ASSERT_EQ(expr.group_keys().size(), 1);
    EXPECT_EQ(expr.group_keys()[0], vardesc(1));
    ASSERT_EQ(expr.sort_keys().size(), 1);
    EXPECT_EQ(expr.sort_keys()[0].variable(), vardesc(2));
    EXPECT_EQ(expr.row_slice(), (row_slice { {}, 5 }));
}

TEST_F(limit_relation_test, compat_count_brace) {
    // `{}` for the row count must be resolved to the compatibility constructor (unlimited),
    // and the trailing arguments must be the group keys and the sort keys, as same as the previous API
    limit expr {
            {},
            {
                    vardesc(1),
            },
    };

    ASSERT_EQ(expr.group_keys().size(), 1);
    EXPECT_EQ(expr.group_keys()[0], vardesc(1));
    ASSERT_EQ(expr.sort_keys().size(), 0);
    EXPECT_EQ(expr.row_slice(), row_slice {});
    EXPECT_EQ(expr.count(), std::nullopt);
}

TEST_F(limit_relation_test, compat_count_brace_sort) {
    limit expr {
            {},
            {
                    vardesc(1),
            },
            {
                    vardesc(2),
            },
    };

    ASSERT_EQ(expr.group_keys().size(), 1);
    EXPECT_EQ(expr.group_keys()[0], vardesc(1));
    ASSERT_EQ(expr.sort_keys().size(), 1);
    EXPECT_EQ(expr.sort_keys()[0].variable(), vardesc(2));
    EXPECT_EQ(expr.row_slice(), row_slice {});
}

TEST_F(limit_relation_test, compat_count_nullopt) {
    limit expr {
            std::nullopt,
            {
                    vardesc(1),
            },
    };

    ASSERT_EQ(expr.group_keys().size(), 1);
    EXPECT_EQ(expr.row_slice(), row_slice {});
}

TEST_F(limit_relation_test, compat_count_vector) {
    std::vector<descriptor::variable> keys { vardesc(1) };
    std::vector<limit::sort_key> sorts { vardesc(2) };
    limit expr { 5, std::move(keys), std::move(sorts) };

    ASSERT_EQ(expr.group_keys().size(), 1);
    ASSERT_EQ(expr.sort_keys().size(), 1);
    EXPECT_EQ(expr.row_slice(), (row_slice { {}, 5 }));
}

TEST_F(limit_relation_test, compat_count_getter) {
    limit expr {};
    EXPECT_EQ(expr.count(), std::nullopt);

    expr.row_slice() = { 0, 5 };
    EXPECT_EQ(expr.count(), 5);

    expr.row_slice() = { 1, 5 };
    EXPECT_THROW((void) expr.count(), std::domain_error);

    expr.row_slice() = { {}, vardesc(2) };
    EXPECT_THROW((void) expr.count(), std::domain_error);
}

TEST_F(limit_relation_test, compat_count_setter) {
    limit expr { row_slice { 10, {} } };

    expr.count(5);
    EXPECT_EQ(expr.row_slice(), (row_slice { 10, 5 }));

    expr.count(std::nullopt);
    EXPECT_EQ(expr.row_slice(), (row_slice { 10, {} }));
}

TEST_F(limit_relation_test, clone) {
    limit expr {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
            {
                    { vardesc(10) },
                    { vardesc(20), sort_direction::descendant },
                    { vardesc(30), sort_direction::ascendant },
            },
            {
                    {}, 5,
            },
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(limit_relation_test, clone_move) {
    limit expr {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
            {
                    { vardesc(10) },
                    { vardesc(20), sort_direction::descendant },
                    { vardesc(30), sort_direction::ascendant },
            },
            {
                    10,
                    5,
            },
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_NE(std::addressof(expr), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(limit_relation_test, output) {
    limit expr {
            {
                    vardesc(1),
                    vardesc(2),
                    vardesc(3),
            },
            {
                    { vardesc(10) },
                    { vardesc(20), sort_direction::descendant },
                    { vardesc(30), sort_direction::ascendant },
            },
            {
                    10,
                    5,
            },
    };

    std::cout << expr << std::endl;
}

} // namespace takatori::relation::intermediate
