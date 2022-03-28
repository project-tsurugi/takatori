#include <takatori/statement/empty.h>

#include <type_traits>

#include <gtest/gtest.h>

#include <takatori/statement/dispatch.h>
#include <takatori/util/clonable.h>

namespace takatori::statement {

class empty_statement_test : public ::testing::Test {};

static_assert(!std::is_copy_constructible_v<empty>);
static_assert(!std::is_copy_assignable_v<empty>);
static_assert(!std::is_move_constructible_v<empty>);
static_assert(!std::is_move_assignable_v<empty>);

TEST_F(empty_statement_test, simple) {
    empty stmt {};
    EXPECT_EQ(stmt.kind(), statement_kind::empty);
}

TEST_F(empty_statement_test, clone) {
    empty stmt {};

    auto copy = util::clone_unique(stmt);
    EXPECT_EQ(stmt, *copy);
    EXPECT_NE(std::addressof(stmt), copy.get());
}

TEST_F(empty_statement_test, clone_move) {
    empty stmt {};

    auto copy = util::clone_unique(stmt);
    EXPECT_EQ(stmt, *copy);
    EXPECT_NE(std::addressof(stmt), copy.get());

    auto move = util::clone_unique(std::move(stmt));
    EXPECT_NE(std::addressof(stmt), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(empty_statement_test, output) {
    empty stmt {};

    std::cout << stmt << std::endl;
}

TEST_F(empty_statement_test, dispatch) {
    empty stmt {};
    struct s {
        bool operator()(empty const&) { return true; };
        bool operator()(statement const&) { return false; };
    };
    EXPECT_TRUE(dispatch(s {}, stmt));
}

} // namespace takatori::statement
