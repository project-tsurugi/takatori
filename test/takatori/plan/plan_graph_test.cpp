#include <takatori/plan/graph.h>

#include <gtest/gtest.h>

#include <takatori/relation/filter.h>

#include <takatori/plan/process.h>
#include <takatori/plan/forward.h>

#include "test_utils.h"

namespace takatori::plan {

class plan_graph_test : public ::testing::Test {};

process& find_top(graph_type& g) {
    process* result = nullptr;
    enumerate_top(g, [&](step& s) {
        if (result) {
            throw std::logic_error("invalid");
        }
        result = util::downcast<process>(std::addressof(s));
    });
    if (result) {
        return *result;
    }
    std::abort();
}

process& find_bottom(graph_type& g) {
    process* result = nullptr;
    enumerate_bottom(g, [&](step& s) {
        if (result) {
            throw std::logic_error("invalid");
        }
        result = util::downcast<process>(std::addressof(s));
    });
    if (result) {
        return *result;
    }
    std::abort();
}

TEST_F(plan_graph_test, simple) {
    graph::graph<step> g;
    auto&& p = g.emplace<process>();

    EXPECT_EQ(find_top(g), p);
    EXPECT_EQ(find_bottom(g), p);
}

TEST_F(plan_graph_test, edge) {
    graph::graph<step> g;
    auto&& s0 = g.emplace<process>();
    auto&& s1 = g.emplace<forward>();
    auto&& s2 = g.emplace<process>();

    s0 >> s1;
    s1 >> s2;

    EXPECT_EQ(find_top(g), s0);
    EXPECT_EQ(find_bottom(g), s2);
}

TEST_F(plan_graph_test, sort) {
    graph::graph<step> g;
    auto&& s0 = g.emplace<process>();
    auto&& s1 = g.emplace<forward>();
    auto&& s2 = g.emplace<process>();

    s0 >> s1;
    s1 >> s2;

    std::vector<step*> r;
    sort_from_upstream(g, [&](step& s) { r.emplace_back(std::addressof(s)); });

    ASSERT_EQ(r.size(), 3);
    EXPECT_EQ(r[0], std::addressof(s0));
    EXPECT_EQ(r[1], std::addressof(s1));
    EXPECT_EQ(r[2], std::addressof(s2));
}

TEST_F(plan_graph_test, sort_rev) {
    graph::graph<step> g;
    auto&& s0 = g.emplace<process>();
    auto&& s1 = g.emplace<forward>();
    auto&& s2 = g.emplace<process>();

    s0 >> s1;
    s1 >> s2;

    std::vector<step*> r;
    sort_from_downstream(g, [&](step& s) { r.emplace_back(std::addressof(s)); });

    ASSERT_EQ(r.size(), 3);
    EXPECT_EQ(r[0], std::addressof(s2));
    EXPECT_EQ(r[1], std::addressof(s1));
    EXPECT_EQ(r[2], std::addressof(s0));
}

TEST_F(plan_graph_test, merge_copy) {
    graph::graph<step> g;
    auto&& s0 = g.emplace<process>();
    auto&& s1 = g.emplace<forward>();
    auto&& s2 = g.emplace<process>();

    s0 >> s1;
    s1 >> s2;

    graph::graph<step> h;
    merge_into(g, h, h.get_object_creator());

    ASSERT_EQ(h.size(), 3);

    auto&& t0 = find_top(h);
    ASSERT_EQ(t0.downstreams().size(), 1);

    auto&& t1 = t0.downstreams()[0];
    ASSERT_EQ(t1.downstreams().size(), 1);

    auto&& t2 = t1.downstreams()[0];
    ASSERT_EQ(t2.downstreams().size(), 0);
}

TEST_F(plan_graph_test, merge_move) {
    graph::graph<step> g;
    auto&& s0 = g.emplace<process>();
    auto&& s1 = g.emplace<forward>();
    auto&& s2 = g.emplace<process>();

    s0 >> s1;
    s1 >> s2;

    graph::graph<step> h;
    merge_into(std::move(g), h, h.get_object_creator());

    ASSERT_EQ(h.size(), 3);
    EXPECT_TRUE(h.contains(s0));
    EXPECT_TRUE(h.contains(s1));
    EXPECT_TRUE(h.contains(s2));
    EXPECT_TRUE(s0.has_downstream(s1));
    EXPECT_TRUE(s1.has_downstream(s2));
}

} // namespace takatori::plan
