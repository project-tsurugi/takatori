#include <takatori/statement/write.h>

#include <type_traits>

#include <gtest/gtest.h>

#include <takatori/plan/forward.h>
#include <takatori/util/clonable.h>

#include "test_utils.h"

namespace takatori::statement {

class write_statement_test : public ::testing::Test {};

static_assert(!std::is_copy_constructible_v<write>);
static_assert(!std::is_copy_assignable_v<write>);
static_assert(!std::is_move_constructible_v<write>);
static_assert(!std::is_move_assignable_v<write>);

TEST_F(write_statement_test, simple) {
    write stmt {
            write_kind::insert,
            tabledesc("T0"),
            {
                    columndesc("C0"),
                    columndesc("C1"),
                    columndesc("C2"),
            },
            {
                    {
                            constant(0),
                            constant(1),
                            constant(2),
                    },
            },
    };

    EXPECT_EQ(stmt.kind(), statement_kind::write);
    EXPECT_EQ(stmt.operator_kind(), write_kind::insert);
    EXPECT_EQ(stmt.destination(), tabledesc("T0"));
    ASSERT_EQ(stmt.columns().size(), 3);
    EXPECT_EQ(stmt.columns()[0], columndesc("C0"));
    EXPECT_EQ(stmt.columns()[1], columndesc("C1"));
    EXPECT_EQ(stmt.columns()[2], columndesc("C2"));
    ASSERT_EQ(stmt.tuples().size(), 1);
    {
        auto&& t = stmt.tuples()[0].elements();
        ASSERT_EQ(t.size(), 3);
        EXPECT_EQ(t[0], constant(0));
        EXPECT_EQ(t[1], constant(1));
        EXPECT_EQ(t[2], constant(2));
        for (auto&& e : t) {
            EXPECT_EQ(e.parent_element(), std::addressof(stmt));
        }
    }
    ASSERT_EQ(stmt.default_columns().size(), 0);
}

TEST_F(write_statement_test, multi_tuples) {
    write stmt {
            write_kind::insert,
            tabledesc("T0"),
            {
                    columndesc("C0"),
                    columndesc("C1"),
                    columndesc("C2"),
            },
            {
                    {
                            constant(10),
                            constant(11),
                            constant(12),
                    },
                    {
                            constant(20),
                            constant(21),
                            constant(22),
                    },
                    {
                            constant(30),
                            constant(31),
                            constant(32),
                    },
            },
    };

    ASSERT_EQ(stmt.tuples().size(), 3);
    {
        auto&& t = stmt.tuples()[0].elements();
        ASSERT_EQ(t.size(), 3);
        EXPECT_EQ(t[0], constant(10));
        EXPECT_EQ(t[1], constant(11));
        EXPECT_EQ(t[2], constant(12));
        for (auto&& e : t) {
            EXPECT_EQ(e.parent_element(), std::addressof(stmt));
        }
    }
    {
        auto&& t = stmt.tuples()[1].elements();
        ASSERT_EQ(t.size(), 3);
        EXPECT_EQ(t[0], constant(20));
        EXPECT_EQ(t[1], constant(21));
        EXPECT_EQ(t[2], constant(22));
        for (auto&& e : t) {
            EXPECT_EQ(e.parent_element(), std::addressof(stmt));
        }
    }
    {
        auto&& t = stmt.tuples()[2].elements();
        ASSERT_EQ(t.size(), 3);
        EXPECT_EQ(t[0], constant(30));
        EXPECT_EQ(t[1], constant(31));
        EXPECT_EQ(t[2], constant(32));
        for (auto&& e : t) {
            EXPECT_EQ(e.parent_element(), std::addressof(stmt));
        }
    }
}

TEST_F(write_statement_test, default_columns) {
    write stmt {
            write_kind::insert,
            tabledesc("T0"),
            {
            },
            {
                    {},
                    {},
                    {},
            },
            {
                    columndesc("C0"),
                    columndesc("C1"),
                    columndesc("C2"),
            },
    };

    ASSERT_EQ(stmt.tuples().size(), 3);
    ASSERT_EQ(stmt.tuples()[0].elements().size(), 0);
    ASSERT_EQ(stmt.tuples()[1].elements().size(), 0);
    ASSERT_EQ(stmt.tuples()[2].elements().size(), 0);
    ASSERT_EQ(stmt.default_columns().size(), 3);
    EXPECT_EQ(stmt.default_columns()[0], columndesc("C0"));
    EXPECT_EQ(stmt.default_columns()[1], columndesc("C1"));
    EXPECT_EQ(stmt.default_columns()[2], columndesc("C2"));
}

TEST_F(write_statement_test, clone) {
    write stmt {
            write_kind::insert,
            tabledesc("T0"),
            {
                    columndesc("C0"),
                    columndesc("C1"),
            },
            {
                    {
                            constant(10),
                            constant(11),
                    },
                    {
                            constant(20),
                            constant(21),
                    },
            },
            {
                    columndesc("C2"),
            },
    };

    auto copy = util::clone_unique(stmt);
    EXPECT_EQ(stmt, *copy);
    EXPECT_NE(std::addressof(stmt), copy.get());
}

TEST_F(write_statement_test, clone_move) {
    write stmt {
            write_kind::insert,
            tabledesc("T0"),
            {
                    columndesc("C0"),
                    columndesc("C1"),
            },
            {
                    {
                            constant(10),
                            constant(11),
                    },
                    {
                            constant(20),
                            constant(21),
                    },
            },
            {
                    columndesc("C2"),
            },
    };

    auto copy = util::clone_unique(stmt);
    EXPECT_EQ(stmt, *copy);
    EXPECT_NE(std::addressof(stmt), copy.get());

    auto move = util::clone_unique(std::move(stmt));
    EXPECT_NE(std::addressof(stmt), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(write_statement_test, output) {
    write stmt {
            write_kind::insert,
            tabledesc("T0"),
            {
                    columndesc("C0"),
                    columndesc("C1"),
            },
            {
                    {
                            constant(10),
                            constant(11),
                    },
                    {
                            constant(20),
                            constant(21),
                    },
            },
            {
                    columndesc("C2"),
            },
    };

    std::cout << stmt << std::endl;
}

} // namespace takatori::statement
