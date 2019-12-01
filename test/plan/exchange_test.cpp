#include "takatori/plan/exchange.h"

#include <gtest/gtest.h>

#include "takatori/plan/process.h"
#include "takatori/plan/discard.h"

#include "takatori/relation/step/limit.h"

namespace takatori::plan {

class exchange_test : public ::testing::Test {};

TEST_F(exchange_test, upstream) {
    graph::graph<step> g;
    auto&& e = g.emplace<discard>();
    auto&& p = g.emplace<process>();
    auto&& x = g.emplace<process>();
    EXPECT_EQ(e.upstreams().size(), 0);
    EXPECT_EQ(e.downstreams().size(), 0);
    EXPECT_EQ(p.upstreams().size(), 0);
    EXPECT_EQ(p.downstreams().size(), 0);
    EXPECT_FALSE(e.has_upstream(p));
    EXPECT_FALSE(p.has_downstream(e));

    e.add_upstream(p);
    EXPECT_EQ(e.upstreams().size(), 1);
    EXPECT_EQ(e.downstreams().size(), 0);
    EXPECT_EQ(p.upstreams().size(), 0);
    EXPECT_EQ(p.downstreams().size(), 1);
    EXPECT_TRUE(e.has_upstream(p));
    EXPECT_TRUE(p.has_downstream(e));

    e.remove_upstream(x);
    EXPECT_EQ(e.upstreams().size(), 1);
    EXPECT_EQ(e.downstreams().size(), 0);
    EXPECT_EQ(p.upstreams().size(), 0);
    EXPECT_EQ(p.downstreams().size(), 1);
    EXPECT_TRUE(e.has_upstream(p));
    EXPECT_TRUE(p.has_downstream(e));

    e.remove_upstream(p);
    EXPECT_EQ(e.upstreams().size(), 0);
    EXPECT_EQ(e.downstreams().size(), 0);
    EXPECT_EQ(p.upstreams().size(), 0);
    EXPECT_EQ(p.downstreams().size(), 0);
    EXPECT_FALSE(e.has_upstream(p));
    EXPECT_FALSE(p.has_downstream(e));
}

TEST_F(exchange_test, downstream) {
    graph::graph<step> g;
    auto&& e = g.emplace<discard>();
    auto&& p = g.emplace<process>();
    auto&& x = g.emplace<process>();
    EXPECT_EQ(e.upstreams().size(), 0);
    EXPECT_EQ(e.downstreams().size(), 0);
    EXPECT_EQ(p.upstreams().size(), 0);
    EXPECT_EQ(p.downstreams().size(), 0);
    EXPECT_FALSE(e.has_downstream(p));
    EXPECT_FALSE(p.has_upstream(e));

    e.add_downstream(p);
    EXPECT_EQ(e.upstreams().size(), 0);
    ASSERT_EQ(e.downstreams().size(), 1);
    ASSERT_EQ(p.upstreams().size(), 1);
    EXPECT_EQ(p.downstreams().size(), 0);
    EXPECT_TRUE(e.has_downstream(p));
    EXPECT_TRUE(p.has_upstream(e));

    e.remove_downstream(x);
    EXPECT_EQ(e.upstreams().size(), 0);
    EXPECT_EQ(e.downstreams().size(), 1);
    EXPECT_EQ(p.upstreams().size(), 1);
    EXPECT_EQ(p.downstreams().size(), 0);
    EXPECT_TRUE(e.has_downstream(p));
    EXPECT_TRUE(p.has_upstream(e));

    e.remove_downstream(p);
    EXPECT_EQ(e.upstreams().size(), 0);
    EXPECT_EQ(e.downstreams().size(), 0);
    EXPECT_EQ(p.upstreams().size(), 0);
    EXPECT_EQ(p.downstreams().size(), 0);
    EXPECT_FALSE(e.has_downstream(p));
    EXPECT_FALSE(p.has_upstream(e));
}

TEST_F(exchange_test, output) {
    graph::graph<step> g;
    exchange& e = g.emplace<discard>();

    std::cout << e << std::endl;
}

} // namespace takatori::plan
