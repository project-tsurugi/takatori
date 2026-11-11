#include <takatori/statement/create_foreign_catalog.h>

#include <type_traits>

#include <gtest/gtest.h>

#include <takatori/util/clonable.h>

namespace takatori::statement {

class create_foreign_catalog_statement_test : public ::testing::Test {};

static_assert(!std::is_copy_constructible_v<create_foreign_catalog>);
static_assert(!std::is_copy_assignable_v<create_foreign_catalog>);
static_assert(!std::is_move_constructible_v<create_foreign_catalog>);
static_assert(!std::is_move_assignable_v<create_foreign_catalog>);

TEST_F(create_foreign_catalog_statement_test, simple) {
    create_foreign_catalog stmt {
            "c",
            "s",
    };
    EXPECT_EQ(stmt.kind(), statement_kind::create_foreign_catalog);
    EXPECT_EQ(stmt.catalog_name(), "c");
    EXPECT_EQ(stmt.server_name(), "s");
    EXPECT_EQ(stmt.options(), create_foreign_catalog::option_set_type {});
}

TEST_F(create_foreign_catalog_statement_test, options) {
    create_foreign_catalog stmt {
            "c",
            "s",
            {
                    create_foreign_catalog_option_kind::if_catalog_not_exists,
                    create_foreign_catalog_option_kind::if_server_exists,
            },
    };
    EXPECT_TRUE(stmt.options().contains(create_foreign_catalog_option_kind::if_catalog_not_exists));
    EXPECT_TRUE(stmt.options().contains(create_foreign_catalog_option_kind::if_server_exists));
}

TEST_F(create_foreign_catalog_statement_test, equals) {
    create_foreign_catalog a { "c", "s", { create_foreign_catalog_option_kind::if_server_exists } };
    create_foreign_catalog b { "c", "s", { create_foreign_catalog_option_kind::if_server_exists } };
    create_foreign_catalog c { "x", "s", { create_foreign_catalog_option_kind::if_server_exists } };
    create_foreign_catalog d { "c", "x", { create_foreign_catalog_option_kind::if_server_exists } };
    create_foreign_catalog e { "c", "s", { create_foreign_catalog_option_kind::if_catalog_not_exists } };
    EXPECT_EQ(a, b);
    EXPECT_NE(a, c);
    EXPECT_NE(a, d);
    EXPECT_NE(a, e);
}

TEST_F(create_foreign_catalog_statement_test, clone) {
    create_foreign_catalog stmt {
            "c",
            "s",
            { create_foreign_catalog_option_kind::if_catalog_not_exists },
    };

    auto copy = util::clone_unique(stmt);
    EXPECT_EQ(stmt, *copy);
    EXPECT_NE(std::addressof(stmt), copy.get());
}

TEST_F(create_foreign_catalog_statement_test, clone_move) {
    create_foreign_catalog stmt {
            "c",
            "s",
            { create_foreign_catalog_option_kind::if_catalog_not_exists },
    };

    auto copy = util::clone_unique(stmt);
    EXPECT_EQ(stmt, *copy);
    EXPECT_NE(std::addressof(stmt), copy.get());

    auto move = util::clone_unique(std::move(stmt));
    EXPECT_NE(std::addressof(stmt), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(create_foreign_catalog_statement_test, output) {
    create_foreign_catalog stmt {
            "c",
            "s",
            { create_foreign_catalog_option_kind::if_server_exists },
    };

    std::cout << stmt << std::endl;
}

} // namespace takatori::statement
