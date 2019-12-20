#include <takatori/util/consumer.h>

#include <functional>

#include <gtest/gtest.h>

namespace takatori::util {

class consumer_test : public ::testing::Test {};

struct s {
    int f(int x) { return r_ = x; }
    int r() const { return r_; }

private:
    int r_ {};
};

TEST_F(consumer_test, simple) {
    s o;
    consumer<s, int, int, &s::f> const c { o };
    EXPECT_EQ(c(100), 100);
    EXPECT_EQ(o.r(), 100);
}

TEST_F(consumer_test, wrap) {
    s o;
    std::function<void(int)> f { consumer<s, int, int, &s::f> { o } };
    f(100);
    EXPECT_EQ(o.r(), 100);
}

} // namespace takatori::util
