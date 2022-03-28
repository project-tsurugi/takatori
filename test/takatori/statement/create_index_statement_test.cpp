#include <takatori/statement/create_index.h>

#include <type_traits>

#include <gtest/gtest.h>

#include <takatori/util/clonable.h>

#include "test_utils.h"

namespace takatori::statement {

class create_index_statement_test : public ::testing::Test {};

static_assert(!std::is_copy_constructible_v<create_index>);
static_assert(!std::is_copy_assignable_v<create_index>);
static_assert(!std::is_move_constructible_v<create_index>);
static_assert(!std::is_move_assignable_v<create_index>);

TEST_F(create_index_statement_test, simple) {
    create_index stmt {
            schemadesc("S"),
            tabledesc("T0"),
    };
    EXPECT_EQ(stmt.kind(), statement_kind::create_index);
    EXPECT_EQ(stmt.schema(), schemadesc("S"));
    EXPECT_EQ(stmt.definition(), tabledesc("T0"));
}

TEST_F(create_index_statement_test, clone) {
    create_index stmt {
            schemadesc("S"),
            tabledesc("T0"),
    };

    auto copy = util::clone_unique(stmt);
    EXPECT_EQ(stmt, *copy);
    EXPECT_NE(std::addressof(stmt), copy.get());
}

TEST_F(create_index_statement_test, clone_move) {
    create_index stmt {
            schemadesc("S"),
            tabledesc("T0"),
    };

    auto copy = util::clone_unique(stmt);
    EXPECT_EQ(stmt, *copy);
    EXPECT_NE(std::addressof(stmt), copy.get());

    auto move = util::clone_unique(std::move(stmt));
    EXPECT_NE(std::addressof(stmt), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(create_index_statement_test, output) {
    create_index stmt {
            schemadesc("S"),
            tabledesc("T0"),
    };

    std::cout << stmt << std::endl;
}

} // namespace takatori::statement
