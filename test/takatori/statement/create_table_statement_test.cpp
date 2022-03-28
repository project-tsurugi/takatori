#include <takatori/statement/create_table.h>

#include <type_traits>

#include <gtest/gtest.h>

#include <takatori/util/clonable.h>

#include "test_utils.h"

namespace takatori::statement {

class create_table_statement_test : public ::testing::Test {};

static_assert(!std::is_copy_constructible_v<create_table>);
static_assert(!std::is_copy_assignable_v<create_table>);
static_assert(!std::is_move_constructible_v<create_table>);
static_assert(!std::is_move_assignable_v<create_table>);

TEST_F(create_table_statement_test, simple) {
    create_table stmt {
            schemadesc("S"),
            storagedesc("T0"),
            tabledesc("PK"),
    };
    EXPECT_EQ(stmt.kind(), statement_kind::create_table);
    EXPECT_EQ(stmt.schema(), schemadesc("S"));
    EXPECT_EQ(stmt.definition(), storagedesc("T0"));
    EXPECT_EQ(stmt.primary_key(), tabledesc("PK"));
    EXPECT_TRUE(stmt.unique_keys().empty());
}

TEST_F(create_table_statement_test, unique_key) {
    create_table stmt {
            schemadesc("S"),
            storagedesc("T0"),
            tabledesc("PK"),
            {
                    tabledesc("UK0"),
                    tabledesc("UK1"),
                    tabledesc("UK2"),
            },
    };
    ASSERT_EQ(stmt.unique_keys().size(), 3);
    EXPECT_EQ(stmt.unique_keys()[0], tabledesc("UK0"));
    EXPECT_EQ(stmt.unique_keys()[1], tabledesc("UK1"));
    EXPECT_EQ(stmt.unique_keys()[2], tabledesc("UK2"));
}

TEST_F(create_table_statement_test, clone) {
    create_table stmt {
            schemadesc("S"),
            storagedesc("T0"),
            tabledesc("PK"),
            {
                    tabledesc("UK0"),
                    tabledesc("UK1"),
                    tabledesc("UK2"),
            },
    };

    auto copy = util::clone_unique(stmt);
    EXPECT_EQ(stmt, *copy);
    EXPECT_NE(std::addressof(stmt), copy.get());
}

TEST_F(create_table_statement_test, clone_move) {
    create_table stmt {
            schemadesc("S"),
            storagedesc("T0"),
            tabledesc("PK"),
            {
                    tabledesc("UK0"),
                    tabledesc("UK1"),
                    tabledesc("UK2"),
            },
    };

    auto copy = util::clone_unique(stmt);
    EXPECT_EQ(stmt, *copy);
    EXPECT_NE(std::addressof(stmt), copy.get());

    auto move = util::clone_unique(std::move(stmt));
    EXPECT_NE(std::addressof(stmt), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(create_table_statement_test, output) {
    create_table stmt {
            schemadesc("S"),
            storagedesc("T0"),
            tabledesc("PK"),
            {
                    tabledesc("UK0"),
                    tabledesc("UK1"),
                    tabledesc("UK2"),
            },
    };

    std::cout << stmt << std::endl;
}

} // namespace takatori::statement
