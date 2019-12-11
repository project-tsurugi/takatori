#include "takatori/util/enum_set.h"

#include <gtest/gtest.h>

namespace takatori::util {

class enum_set_test : public ::testing::Test {
public:
    template<class T>
    static T const& make_const(T const& t) { return t; }
};

enum class X {
    _ = -1,
    A = 0, B, C, D, E,
    Z,
};

std::ostream& operator<<(std::ostream& out, X x) {
    return out << static_cast<char>('A' + static_cast<std::size_t>(x));
}

using eset = enum_set<X, X::A, X::E>;

TEST_F(enum_set_test, simple) {
    eset s;
    EXPECT_TRUE(s.empty());
    EXPECT_EQ(s.size(), 0);

    EXPECT_FALSE(s.contains(X::A));
    EXPECT_FALSE(s.contains(X::B));
    EXPECT_FALSE(s.contains(X::C));
    EXPECT_FALSE(s.contains(X::D));
    EXPECT_FALSE(s.contains(X::E));

    s.insert(X::B);
    s.insert(X::D);

    EXPECT_FALSE(s.empty());
    EXPECT_EQ(s.size(), 2);

    EXPECT_FALSE(s.contains(X::A));
    EXPECT_TRUE(s.contains(X::B));
    EXPECT_FALSE(s.contains(X::C));
    EXPECT_TRUE(s.contains(X::D));
    EXPECT_FALSE(s.contains(X::E));
}

TEST_F(enum_set_test, initializer_list) {
    eset s { X::B, X::D };

    EXPECT_FALSE(s.contains(X::A));
    EXPECT_TRUE(s.contains(X::B));
    EXPECT_FALSE(s.contains(X::C));
    EXPECT_TRUE(s.contains(X::D));
    EXPECT_FALSE(s.contains(X::E));
}

TEST_F(enum_set_test, at) {
    eset s;

    EXPECT_FALSE(s.at(X::A));

    s.at(X::A) = true;
    EXPECT_TRUE(s.at(X::A));
}

TEST_F(enum_set_test, at_const) {
    eset ms;
    auto& s = make_const(ms);

    EXPECT_FALSE(s.at(X::A));

    ms.at(X::A) = true;
    EXPECT_TRUE(s.at(X::A));
}

TEST_F(enum_set_test, clear) {
    eset s { X::B, X::D };
    EXPECT_FALSE(s.empty());
    EXPECT_EQ(s.size(), 2);

    s.clear();
    EXPECT_TRUE(s.empty());
    EXPECT_EQ(s.size(), 0);
}

TEST_F(enum_set_test, and) {
    EXPECT_EQ(
            (eset { X::A, X::B, X::C }) & (eset { X::B, X::C, X::D }),
            (eset { X::B, X::C }));
    EXPECT_EQ(
            (eset { X::A, X::B, X::C }) &= (eset { X::B, X::C, X::D }),
            (eset { X::B, X::C }));
}

TEST_F(enum_set_test, or) {
    EXPECT_EQ(
            (eset { X::A, X::B }) | (eset { X::B, X::C }),
            (eset { X::A, X::B, X::C }));
    EXPECT_EQ(
            (eset { X::A, X::B }) |= (eset { X::B, X::C }),
            (eset { X::A, X::B, X::C }));
}

TEST_F(enum_set_test, diff) {
    EXPECT_EQ(
            (eset { X::A, X::B, X::C }) - (eset { X::B, X::C }),
            (eset { X::A, }));
    EXPECT_EQ(
            (eset { X::A, X::B, X::C }) -= (eset { X::B, X::C }),
            (eset { X::A, }));
}

TEST_F(enum_set_test, output) {
    eset s { X::B, X::D };

    std::cout << s << std::endl;
}

} // namespace takatori::util
