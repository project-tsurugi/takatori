#include <takatori/type/octet.h>

#include <gtest/gtest.h>

#include <takatori/util/clonable.h>

namespace takatori::type {

class octet_type_test : public ::testing::Test {};

static_assert(octet::tag == type_kind::octet);
static_assert(std::is_same_v<type_of_t<octet::tag>, octet>);

TEST_F(octet_type_test, simple) {
    octet t { 16 };
    EXPECT_FALSE(t.varying());
    EXPECT_EQ(t.length(), 16);
}

TEST_F(octet_type_test, varying) {
    octet t { varying };
    EXPECT_TRUE(t.varying());
    EXPECT_EQ(t.length().value_or(9999), 9999);
}

TEST_F(octet_type_test, not_varying) {
    octet t { ~varying, 100 };
    EXPECT_FALSE(t.varying());
    EXPECT_EQ(t.length(), 100);
}

TEST_F(octet_type_test, parameters) {
    octet t { varying, 4000 };
    EXPECT_TRUE(t.varying());
    EXPECT_EQ(t.length(), 4000);
}

TEST_F(octet_type_test, hash) {
    octet a { 16 };
    octet b { 4000 };
    octet c { varying, 4000 };

    using namespace std;

    EXPECT_EQ(std::hash<octet>{}(a), std::hash<octet>{}(a));
    EXPECT_EQ(std::hash<octet>{}(b), std::hash<octet>{}(b));
    EXPECT_EQ(std::hash<octet>{}(c), std::hash<octet>{}(c));
    EXPECT_NE(std::hash<octet>{}(a), std::hash<octet>{}(b));
    EXPECT_NE(std::hash<octet>{}(b), std::hash<octet>{}(c));
    EXPECT_NE(std::hash<octet>{}(c), std::hash<octet>{}(a));
}

TEST_F(octet_type_test, clone) {
    octet t { varying, 4000 };
    auto copy = util::clone_unique(t);
    EXPECT_EQ(t, *copy);
    EXPECT_NE(std::addressof(t), copy.get());
}

TEST_F(octet_type_test, clone_move) {
    octet t { varying, 4000 };
    auto copy = util::clone_unique(t);
    EXPECT_EQ(t, *copy);
    EXPECT_NE(std::addressof(t), copy.get());

    auto move = util::clone_unique(std::move(t));
    EXPECT_NE(std::addressof(t), move.get());
    EXPECT_EQ(*copy, *move);
}

TEST_F(octet_type_test, output) {
    octet t { varying, 4000 };
    std::cout << t << std::endl;
}

} // namespace takatori::type
