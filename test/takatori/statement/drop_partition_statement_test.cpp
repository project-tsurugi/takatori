#include <takatori/statement/drop_partition.h>

#include <string_view>
#include <type_traits>

#include <gtest/gtest.h>

#include <takatori/value/primitive.h>
#include <takatori/util/clonable.h>

#include "test_utils.h"

namespace takatori::statement {

class drop_partition_statement_test : public ::testing::Test {};

static_assert(!std::is_copy_constructible_v<drop_partition>);
static_assert(!std::is_copy_assignable_v<drop_partition>);
static_assert(!std::is_move_constructible_v<drop_partition>);
static_assert(!std::is_move_assignable_v<drop_partition>);

using details::partition_criteria;
using details::partition_key_element;
using scalar::comparison_operator;

namespace {

drop_partition build(
        std::string_view index,
        comparison_operator op,
        int literal,
        std::string_view column_name = "C1") {
    return drop_partition {
            tabledesc(index),
            {
                    partition_criteria {
                            {
                                    {
                                            columndesc("C0"),
                                            "C0",
                                            { { comparison_operator::equal, value::int4 { 1 } } },
                                    },
                                    {
                                            columndesc("C1"),
                                            partition_key_element::name_type { column_name },
                                            {
                                                    { op, value::int4 { literal } },
                                                    { comparison_operator::less, value::int4 { 100 } },
                                            },
                                    },
                            },
                    },
                    partition_criteria {
                            {
                                    {
                                            columndesc("C0"),
                                            "C0",
                                            { { comparison_operator::equal, value::int4 { 2 } } },
                                    },
                            },
                    },
            },
    };
}

} // namespace

TEST_F(drop_partition_statement_test, equals) {
    auto a = build("I0", comparison_operator::greater_equal, 10);
    auto b = build("I0", comparison_operator::greater_equal, 10);
    auto c = build("I1", comparison_operator::greater_equal, 10);
    auto d = build("I0", comparison_operator::greater, 10);
    auto e = build("I0", comparison_operator::greater_equal, 11);
    auto f = build("I0", comparison_operator::greater_equal, 10, "V1");
    drop_partition g { tabledesc("I0"), {} };

    // literal values are compared by their contents, not by their pointers
    ASSERT_NE(
            a.target()[0].elements()[1].predicates()[0].second.get(),
            b.target()[0].elements()[1].predicates()[0].second.get());
    EXPECT_EQ(a, b);

    EXPECT_NE(a, c);
    EXPECT_NE(a, d);
    EXPECT_NE(a, e);
    EXPECT_NE(a, f);
    EXPECT_NE(a, g);
}

TEST_F(drop_partition_statement_test, clone) {
    auto stmt = build("I0", comparison_operator::greater_equal, 10);

    auto copy = util::clone_unique(stmt);
    EXPECT_EQ(stmt, *copy);
    EXPECT_NE(std::addressof(stmt), copy.get());
}

TEST_F(drop_partition_statement_test, clone_move) {
    auto stmt = build("I0", comparison_operator::greater_equal, 10);

    auto copy = util::clone_unique(stmt);
    EXPECT_EQ(stmt, *copy);
    EXPECT_NE(std::addressof(stmt), copy.get());

    auto move = util::clone_unique(std::move(stmt));
    EXPECT_NE(std::addressof(stmt), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(drop_partition_statement_test, output) {
    auto stmt = build("I0", comparison_operator::greater_equal, 10);

    std::cout << stmt << std::endl;
}

} // namespace takatori::statement
