#include <takatori/plan/process.h>

#include <gtest/gtest.h>

#include <takatori/plan/discard.h>

#include <takatori/relation/buffer.h>

namespace takatori::plan {

class process_test : public ::testing::Test {};

static_assert(process::tag == step_kind::process);

TEST_F(process_test, simple) {
    graph::graph<step> g;
    auto&& p = g.emplace<process>();

    auto&& ops = p.operators();
    ops.emplace<relation::buffer>(2);
}

TEST_F(process_test, upstream) {
    graph::graph<step> g;
    auto&& p = g.emplace<process>();
    auto&& e = g.emplace<discard>();
    auto&& x = g.emplace<discard>();
    EXPECT_EQ(p.upstreams().size(), 0);
    EXPECT_EQ(p.downstreams().size(), 0);
    EXPECT_EQ(e.upstreams().size(), 0);
    EXPECT_EQ(e.downstreams().size(), 0);
    EXPECT_FALSE(p.has_upstream(e));
    EXPECT_FALSE(e.has_downstream(p));

    p.add_upstream(e);
    EXPECT_EQ(p.upstreams().size(), 1);
    EXPECT_EQ(p.downstreams().size(), 0);
    EXPECT_EQ(e.upstreams().size(), 0);
    EXPECT_EQ(e.downstreams().size(), 1);
    EXPECT_TRUE(p.has_upstream(e));
    EXPECT_TRUE(e.has_downstream(p));

    p.remove_upstream(x);
    EXPECT_EQ(p.upstreams().size(), 1);
    EXPECT_EQ(p.downstreams().size(), 0);
    EXPECT_EQ(e.upstreams().size(), 0);
    EXPECT_EQ(e.downstreams().size(), 1);
    EXPECT_TRUE(p.has_upstream(e));
    EXPECT_TRUE(e.has_downstream(p));

    p.remove_upstream(e);
    EXPECT_EQ(p.upstreams().size(), 0);
    EXPECT_EQ(p.downstreams().size(), 0);
    EXPECT_EQ(e.upstreams().size(), 0);
    EXPECT_EQ(e.downstreams().size(), 0);
    EXPECT_FALSE(p.has_upstream(e));
    EXPECT_FALSE(e.has_downstream(p));
}

TEST_F(process_test, downstream) {
    graph::graph<step> g;
    auto&& p = g.emplace<process>();
    auto&& e = g.emplace<discard>();
    auto&& x = g.emplace<discard>();
    EXPECT_EQ(p.upstreams().size(), 0);
    EXPECT_EQ(p.downstreams().size(), 0);
    EXPECT_EQ(e.upstreams().size(), 0);
    EXPECT_EQ(e.downstreams().size(), 0);
    EXPECT_FALSE(p.has_downstream(e));
    EXPECT_FALSE(e.has_upstream(p));

    p.add_downstream(e);
    EXPECT_EQ(p.upstreams().size(), 0);
    ASSERT_EQ(p.downstreams().size(), 1);
    ASSERT_EQ(e.upstreams().size(), 1);
    EXPECT_EQ(e.downstreams().size(), 0);
    EXPECT_TRUE(p.has_downstream(e));
    EXPECT_TRUE(e.has_upstream(p));

    p.remove_downstream(x);
    EXPECT_EQ(p.upstreams().size(), 0);
    EXPECT_EQ(p.downstreams().size(), 1);
    EXPECT_EQ(e.upstreams().size(), 1);
    EXPECT_EQ(e.downstreams().size(), 0);
    EXPECT_TRUE(p.has_downstream(e));
    EXPECT_TRUE(e.has_upstream(p));

    p.remove_downstream(e);
    EXPECT_EQ(p.upstreams().size(), 0);
    EXPECT_EQ(p.downstreams().size(), 0);
    EXPECT_EQ(e.upstreams().size(), 0);
    EXPECT_EQ(e.downstreams().size(), 0);
    EXPECT_FALSE(p.has_downstream(e));
    EXPECT_FALSE(e.has_upstream(p));
}

TEST_F(process_test, output) {
    graph::graph<step> g;
    auto&& p = g.emplace<process>();
    auto&& ops = p.operators();
    ops.emplace<relation::buffer>(2);

    std::cout << p << std::endl;
}

} // namespace takatori::plan
