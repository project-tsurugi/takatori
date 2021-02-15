#include <takatori/type/row_id.h>

#include <gtest/gtest.h>

#include <takatori/util/clonable.h>

namespace takatori::type {

class row_id_type_test : public ::testing::Test {};

static_assert(row_id::tag == type_kind::row_id);
static_assert(std::is_same_v<type_of_t<row_id::tag>, row_id>);

TEST_F(row_id_type_test, simple) {
    row_id t { 16 };
    EXPECT_EQ(t.sequence(), 16);
}

TEST_F(row_id_type_test, hash) {
    row_id a { 1 };
    row_id b { 2 };

    using namespace std;

    EXPECT_EQ(std::hash<row_id>{}(a), std::hash<row_id>{}(a));
    EXPECT_EQ(std::hash<row_id>{}(b), std::hash<row_id>{}(b));
    EXPECT_NE(std::hash<row_id>{}(a), std::hash<row_id>{}(b));
}

TEST_F(row_id_type_test, clone) {
    row_id t { 72 };
    auto copy = util::clone_unique(t);
    EXPECT_EQ(t, *copy);
    EXPECT_NE(std::addressof(t), copy.get());
}

TEST_F(row_id_type_test, clone_move) {
    row_id t { 72 };
    auto copy = util::clone_unique(t);
    EXPECT_EQ(t, *copy);
    EXPECT_NE(std::addressof(t), copy.get());

    auto move = util::clone_unique(std::move(t));
    EXPECT_NE(std::addressof(t), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(row_id_type_test, output) {
    row_id t { 72};
    std::cout << t << std::endl;
}

} // namespace takatori::type
