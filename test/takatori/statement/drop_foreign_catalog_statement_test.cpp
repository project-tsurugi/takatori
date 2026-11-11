#include <takatori/statement/drop_foreign_catalog.h>

#include <type_traits>

#include <gtest/gtest.h>

#include <takatori/util/clonable.h>

namespace takatori::statement {

class drop_foreign_catalog_statement_test : public ::testing::Test {};

static_assert(!std::is_copy_constructible_v<drop_foreign_catalog>);
static_assert(!std::is_copy_assignable_v<drop_foreign_catalog>);
static_assert(!std::is_move_constructible_v<drop_foreign_catalog>);
static_assert(!std::is_move_assignable_v<drop_foreign_catalog>);

TEST_F(drop_foreign_catalog_statement_test, simple) {
    drop_foreign_catalog stmt {
            "c",
    };
    EXPECT_EQ(stmt.kind(), statement_kind::drop_foreign_catalog);
    EXPECT_EQ(stmt.catalog_name(), "c");
    EXPECT_EQ(stmt.options(), drop_foreign_catalog::option_set_type {});
}

TEST_F(drop_foreign_catalog_statement_test, options) {
    drop_foreign_catalog stmt {
            "c",
            { drop_foreign_catalog_option_kind::if_catalog_exists },
    };
    EXPECT_TRUE(stmt.options().contains(drop_foreign_catalog_option_kind::if_catalog_exists));
}

TEST_F(drop_foreign_catalog_statement_test, equals) {
    drop_foreign_catalog a { "c", { drop_foreign_catalog_option_kind::if_catalog_exists } };
    drop_foreign_catalog b { "c", { drop_foreign_catalog_option_kind::if_catalog_exists } };
    drop_foreign_catalog c { "x", { drop_foreign_catalog_option_kind::if_catalog_exists } };
    drop_foreign_catalog d { "c" };
    EXPECT_EQ(a, b);
    EXPECT_NE(a, c);
    EXPECT_NE(a, d);
}

TEST_F(drop_foreign_catalog_statement_test, clone) {
    drop_foreign_catalog stmt {
            "c",
            { drop_foreign_catalog_option_kind::if_catalog_exists },
    };

    auto copy = util::clone_unique(stmt);
    EXPECT_EQ(stmt, *copy);
    EXPECT_NE(std::addressof(stmt), copy.get());
}

TEST_F(drop_foreign_catalog_statement_test, clone_move) {
    drop_foreign_catalog stmt {
            "c",
            { drop_foreign_catalog_option_kind::if_catalog_exists },
    };

    auto copy = util::clone_unique(stmt);
    EXPECT_EQ(stmt, *copy);
    EXPECT_NE(std::addressof(stmt), copy.get());

    auto move = util::clone_unique(std::move(stmt));
    EXPECT_NE(std::addressof(stmt), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(drop_foreign_catalog_statement_test, output) {
    drop_foreign_catalog stmt {
            "c",
            { drop_foreign_catalog_option_kind::if_catalog_exists },
    };

    std::cout << stmt << std::endl;
}

} // namespace takatori::statement
