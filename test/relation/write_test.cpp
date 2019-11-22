#include "takatori/relation/write.h"

#include <type_traits>

#include <gtest/gtest.h>

#include "test_utils.h"

#include "takatori/util/clonable.h"

namespace takatori::relation {

class write_test : public ::testing::Test {};

static_assert(write::tag == expression_kind::write);
static_assert(std::is_same_v<type_of_t<write::tag>, write>);
static_assert(is_available_in_intermediate_plan(write::tag));
static_assert(is_available_in_step_plan(write::tag));

using mapping = details::mapping_element;

TEST_F(write_test, simple) {
    write expr {
            write_kind::insert,
            tabledesc("T"),
            {
                    { vardesc(1), vardesc(10) },
            },
            {
                    { vardesc(6), vardesc(60) },
            },
    };

    EXPECT_EQ(expr.operator_kind(), write_kind::insert);
    EXPECT_EQ(expr.destination(), tabledesc("T"));
    ASSERT_EQ(expr.keys().size(), 1);
    EXPECT_EQ(expr.keys()[0], mapping(vardesc(1), vardesc(10)));
    ASSERT_EQ(expr.columns().size(), 1);
    EXPECT_EQ(expr.columns()[0], mapping(vardesc(6), vardesc(60)));

    EXPECT_EQ(&expr.input().owner(), &expr);

    {
        auto p = expr.input_ports();
        ASSERT_EQ(p.size(), 1);
        EXPECT_EQ(&p[0], &expr.input());
    }
    {
        auto p = expr.output_ports();
        ASSERT_EQ(p.size(), 0);
    }
}

TEST_F(write_test, empty_keys) {
    write expr {
            write_kind::update,
            tabledesc("T"),
            {},
            {
                    { vardesc(6), vardesc(60) },
            },
    };

    EXPECT_EQ(expr.operator_kind(), write_kind::update);
    EXPECT_EQ(expr.destination(), tabledesc("T"));
    ASSERT_EQ(expr.keys().size(), 0);
    ASSERT_EQ(expr.columns().size(), 1);
    EXPECT_EQ(expr.columns()[0], mapping(vardesc(6), vardesc(60)));
}

TEST_F(write_test, multiple_keys) {
    write expr {
            write_kind::update,
            tabledesc("T"),
            {
                    { vardesc(1), vardesc(10) },
                    { vardesc(2), vardesc(20) },
                    { vardesc(3), vardesc(30) },
            },
            {
                    { vardesc(6), vardesc(60) },
            },
    };

    EXPECT_EQ(expr.operator_kind(), write_kind::update);
    EXPECT_EQ(expr.destination(), tabledesc("T"));
    ASSERT_EQ(expr.keys().size(), 3);
    EXPECT_EQ(expr.keys()[0], mapping(vardesc(1), vardesc(10)));
    EXPECT_EQ(expr.keys()[1], mapping(vardesc(2), vardesc(20)));
    EXPECT_EQ(expr.keys()[2], mapping(vardesc(3), vardesc(30)));
    ASSERT_EQ(expr.columns().size(), 1);
    EXPECT_EQ(expr.columns()[0], mapping(vardesc(6), vardesc(60)));
}

TEST_F(write_test, empty_columns) {
    write expr {
            write_kind::delete_,
            tabledesc("T"),
            {
                    { vardesc(1), vardesc(10) },
            },
            {},
    };

    EXPECT_EQ(expr.operator_kind(), write_kind::delete_);
    EXPECT_EQ(expr.destination(), tabledesc("T"));
    ASSERT_EQ(expr.keys().size(), 1);
    EXPECT_EQ(expr.keys()[0], mapping(vardesc(1), vardesc(10)));
    ASSERT_EQ(expr.columns().size(), 0);
}

TEST_F(write_test, multiple_columns) {
    write expr {
            write_kind::insert_or_update,
            tabledesc("T"),
            {
                    { vardesc(1), vardesc(10) },
            },
            {
                    { vardesc(6), vardesc(60) },
                    { vardesc(7), vardesc(70) },
                    { vardesc(8), vardesc(80) },
            },
    };

    EXPECT_EQ(expr.operator_kind(), write_kind::insert_or_update);
    EXPECT_EQ(expr.destination(), tabledesc("T"));
    ASSERT_EQ(expr.keys().size(), 1);
    EXPECT_EQ(expr.keys()[0], mapping(vardesc(1), vardesc(10)));
    ASSERT_EQ(expr.columns().size(), 3);
    EXPECT_EQ(expr.columns()[0], mapping(vardesc(6), vardesc(60)));
    EXPECT_EQ(expr.columns()[1], mapping(vardesc(7), vardesc(70)));
    EXPECT_EQ(expr.columns()[2], mapping(vardesc(8), vardesc(80)));
}

TEST_F(write_test, clone) {
    write expr {
            write_kind::insert_or_update,
            tabledesc("T"),
            {
                    { vardesc(1), vardesc(10) },
            },
            {
                    { vardesc(6), vardesc(60) },
                    { vardesc(7), vardesc(70) },
                    { vardesc(8), vardesc(80) },
            },
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());
}

TEST_F(write_test, clone_move) {
    write expr {
            write_kind::insert_or_update,
            tabledesc("T"),
            {
                    { vardesc(1), vardesc(10) },
            },
            {
                    { vardesc(6), vardesc(60) },
                    { vardesc(7), vardesc(70) },
                    { vardesc(8), vardesc(80) },
            },
    };

    auto copy = util::clone_unique(expr);
    EXPECT_EQ(expr, *copy);
    EXPECT_NE(std::addressof(expr), copy.get());

    auto move = util::clone_unique(std::move(expr));
    EXPECT_NE(std::addressof(expr), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(write_test, output) {
    write expr {
            write_kind::insert_or_update,
            tabledesc("T"),
            {
                    { vardesc(1), vardesc(10) },
            },
            {
                    { vardesc(6), vardesc(60) },
                    { vardesc(7), vardesc(70) },
                    { vardesc(8), vardesc(80) },
            },
    };

    std::cout << expr << std::endl;
}

} // namespace takatori::relation
