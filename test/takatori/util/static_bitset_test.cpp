#include <takatori/util/static_bitset.h>

#include <type_traits>

#include <gtest/gtest.h>

namespace takatori::util {

class static_bitset_test : public ::testing::Test {};

static_assert(std::is_trivially_copyable_v<static_bitset<8>>);
static_assert(std::is_trivially_copyable_v<static_bitset<16>>);
static_assert(std::is_trivially_copyable_v<static_bitset<32>>);
static_assert(std::is_trivially_copyable_v<static_bitset<64>>);
static_assert(std::is_trivially_copyable_v<static_bitset<128>>);

template<class... Args>
static constexpr static_bitset<sizeof...(Args)> of(Args... args) {
    return static_bitset<sizeof...(Args)> { args... };
}

TEST_F(static_bitset_test, simple) {
    static_bitset<10> bits {};

    EXPECT_EQ(bits.size(), 10);
}

TEST_F(static_bitset_test, from_list) {
    static_bitset<5> bits { 1, 0, 1, 1, 0 };

    EXPECT_TRUE(bits.get(0));
    EXPECT_FALSE(bits.get(1));
    EXPECT_TRUE(bits.get(2));
    EXPECT_TRUE(bits.get(3));
    EXPECT_FALSE(bits.get(4));
}

TEST_F(static_bitset_test, bit) {
    static_bitset<5> bits;

    EXPECT_FALSE(bits.get(1));

    bits.set(1, true);
    EXPECT_TRUE(bits.get(1));

    bits.reset(1);
    EXPECT_FALSE(bits.get(1));

    bits.set(2, true);
    EXPECT_FALSE(bits.get(1));

    bits.flip(1);
    EXPECT_TRUE(bits.get(1));

    bits.flip(1);
    EXPECT_FALSE(bits.get(1));
}


TEST_F(static_bitset_test, range) {
    static_bitset<128> bits {};
    bits.set(0);
    bits.set(32);
    bits.set(64);
    bits.set(65);
    bits.set(127);
    EXPECT_THROW(    (void) bits.set(128), std::out_of_range);

    EXPECT_TRUE(bits.test(0));
    EXPECT_TRUE(bits.test(32));
    EXPECT_TRUE(bits.test(64));
    EXPECT_TRUE(bits.test(65));
    EXPECT_TRUE(bits.test(127));
}

TEST_F(static_bitset_test, reference) {
    static_bitset<128> bits {};

    auto ref = bits[88];

    EXPECT_FALSE(ref);

    ref = true;
    EXPECT_TRUE(ref);
    EXPECT_TRUE(bits.test(88));

    bits.reset(88);
    EXPECT_FALSE(ref);
}

TEST_F(static_bitset_test, count) {
    static_bitset<5> a {};
    static_bitset<5> b { 1, 0, 0, 1, 1 };
    static_bitset<5> c {};
    c.set();

    EXPECT_EQ(a.count(), 0);
    EXPECT_EQ(b.count(), 3);
    EXPECT_EQ(c.count(), 5);
}

TEST_F(static_bitset_test, all) {
    static_bitset<5> a {};
    static_bitset<5> b { 1, 0, 0, 1, 1 };
    static_bitset<5> c {};
    c.set();

    EXPECT_FALSE(a.all());
    EXPECT_FALSE(b.all());
    EXPECT_TRUE(c.all());
}

TEST_F(static_bitset_test, any) {
    static_bitset<5> a {};
    static_bitset<5> b { 1, 0, 0, 1, 1 };
    static_bitset<5> c {};
    c.set();

    EXPECT_FALSE(a.any());
    EXPECT_TRUE(b.any());
    EXPECT_TRUE(c.any());
}

TEST_F(static_bitset_test, none) {
    static_bitset<5> a {};
    static_bitset<5> b { 1, 0, 0, 1, 1 };
    static_bitset<5> c {};
    c.set();

    EXPECT_TRUE(a.none());
    EXPECT_FALSE(b.none());
    EXPECT_FALSE(c.none());
}

TEST_F(static_bitset_test, for) {
    static_bitset<5> bits { 1, 0, 0, 1, 1 };

    EXPECT_EQ(bits.find_first(), 0);
    EXPECT_EQ(bits.find_next(0), 3);
    EXPECT_EQ(bits.find_next(3), 4);
    EXPECT_EQ(bits.find_next(4), decltype(bits)::npos);
}

TEST_F(static_bitset_test, for_false) {
    static_bitset<5> bits { 0, 1, 1, 0, 0 };

    EXPECT_EQ(bits.find_first(false), 0);
    EXPECT_EQ(bits.find_next(0, false), 3);
    EXPECT_EQ(bits.find_next(3, false), 4);
    EXPECT_EQ(bits.find_next(4, false), decltype(bits)::npos);
}

TEST_F(static_bitset_test, eq) {
    static_bitset<3> a { 0, 1, 1 };
    static_bitset<3> b { 0, 0, 0 };
    static_bitset<3> c { 1, 1, 1 };
    static_bitset<3> d { 0, 1, 1 };

    EXPECT_EQ(a, a);
    EXPECT_EQ(a, d);
    EXPECT_NE(a, b);
    EXPECT_NE(a, c);
}

TEST_F(static_bitset_test, is_subset) {
    static_bitset<3> a { 0, 1, 1 };
    static_bitset<3> b { 0, 0, 0 };
    static_bitset<3> c { 0, 0, 1 };
    static_bitset<3> d { 0, 1, 1 };
    static_bitset<3> e { 1, 1, 0 };

    EXPECT_TRUE(a.is_subset_of(a));
    EXPECT_TRUE(b.is_subset_of(a));
    EXPECT_TRUE(c.is_subset_of(a));
    EXPECT_TRUE(d.is_subset_of(a));
    EXPECT_FALSE(e.is_subset_of(a));
}

TEST_F(static_bitset_test, intersects) {
    static_bitset<3> a { 0, 0, 1 };
    static_bitset<3> b { 0, 0, 0 };
    static_bitset<3> c { 0, 1, 0 };
    static_bitset<3> d { 1, 1, 0 };
    static_bitset<3> e { 0, 1, 1 };

    EXPECT_TRUE(a.intersects(a));
    EXPECT_FALSE(b.intersects(a));
    EXPECT_FALSE(c.intersects(a));
    EXPECT_FALSE(d.intersects(a));
    EXPECT_TRUE(e.intersects(a));
}

TEST_F(static_bitset_test, set_all) {
    static_bitset<3> a {0, 1, 0};
    a.set();
    EXPECT_EQ(a, of(1, 1, 1));
}

TEST_F(static_bitset_test, reset_all) {
    static_bitset<3> a {0, 1, 0};
    a.reset();
    EXPECT_EQ(a, of(0, 0, 0));
}

TEST_F(static_bitset_test, flip_all) {
    static_bitset<3> a {0, 1, 0};
    a.flip();
    EXPECT_EQ(a, of(1, 0, 1));
}

TEST_F(static_bitset_test, complement_bitwise) {
    static_bitset<4> a {0, 1, 0, 1};
    static_bitset<4> r {1, 0, 1, 0};
    EXPECT_EQ(~a, r);
}

TEST_F(static_bitset_test, add_bitwise) {
    static_bitset<4> a {0, 1, 0, 1};
    static_bitset<4> b {0, 0, 1, 1};
    static_bitset<4> r {0, 1, 1, 1};

    EXPECT_EQ(a | b, r);

    a |= b;
    EXPECT_EQ(a, r);
}

TEST_F(static_bitset_test, retain_bitwise) {
    static_bitset<4> a {0, 1, 0, 1};
    static_bitset<4> b {0, 0, 1, 1};
    static_bitset<4> r {0, 0, 0, 1};

    EXPECT_EQ(a & b, r);

    a &= b;
    EXPECT_EQ(a, r);
}

TEST_F(static_bitset_test, remove_bitwise) {
    static_bitset<4> a {0, 1, 0, 1};
    static_bitset<4> b {0, 0, 1, 1};
    static_bitset<4> r {0, 1, 0, 0};

    EXPECT_EQ(a - b, r);

    a -= b;
    EXPECT_EQ(a, r);
}

TEST_F(static_bitset_test, flip_bitwise) {
    static_bitset<4> a {0, 1, 0, 1};
    static_bitset<4> b {0, 0, 1, 1};
    static_bitset<4> r {0, 1, 1, 0};

    EXPECT_EQ(a ^ b, r);

    a ^= b;
    EXPECT_EQ(a, r);
}

TEST_F(static_bitset_test, output) {
    static_bitset<4> a {0, 1, 0, 1};
    std::cout << a << std::endl;
}

} // namespace takatori::util
