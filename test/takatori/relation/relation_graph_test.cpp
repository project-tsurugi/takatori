#include <takatori/relation/graph.h>

#include <gtest/gtest.h>

#include <takatori/relation/filter.h>

#include "test_utils.h"

namespace takatori::relation {

class relation_graph_test : public ::testing::Test {};

static graph_type release(
        graph_type& source,
        std::initializer_list<std::reference_wrapper<expression const>> elements) {
    std::vector<expression const*> es;
    for (expression const& e : elements) {
        es.emplace_back(std::addressof(e));
    }
    return relation::release(source, es);
}

TEST_F(relation_graph_test, merge_simple) {
    graph_type g0;
    graph_type g1;

    auto&& r0 = g0.insert(filter { constant(1) });
    auto&& r1 = g1.insert(filter { constant(2) });

    merge_into(std::move(g0), g1);

    ASSERT_EQ(g1.size(), 2);

    EXPECT_TRUE(g1.contains(r0));
    EXPECT_TRUE(g1.contains(r1));
}

TEST_F(relation_graph_test, merge_connected) {
    graph_type g0;
    graph_type g1;

    auto&& r0 = g0.insert(filter { constant(1) });
    auto&& r1 = g0.insert(filter { constant(2) });
    auto&& r2 = g1.insert(filter { constant(3) });
    auto&& r3 = g1.insert(filter { constant(4) });

    r0.output() >> r1.input();
    r2.output() >> r3.input();

    merge_into(std::move(g0), g1);

    ASSERT_EQ(g1.size(), 4);

    EXPECT_TRUE(g1.contains(r0));
    EXPECT_TRUE(g1.contains(r1));
    EXPECT_TRUE(g1.contains(r2));
    EXPECT_TRUE(g1.contains(r3));

    EXPECT_EQ(r0.output().opposite().get(), &r1.input());
    EXPECT_EQ(r2.output().opposite().get(), &r3.input());
}

TEST_F(relation_graph_test, release_simple) {
    graph_type g0;
    auto&& r0 = g0.insert(filter { constant(1) });

    graph_type g1 = release(g0, { r0 });

    ASSERT_EQ(g0.size(), 0);
    ASSERT_EQ(g1.size(), 1);

    EXPECT_TRUE(g1.contains(r0));
}

TEST_F(relation_graph_test, release_connected) {
    graph_type g0;
    auto&& r0 = g0.insert(filter { constant(1) });
    auto&& r1 = g0.insert(filter { constant(2) });
    auto&& r2 = g0.insert(filter { constant(3) });

    r0.output() >> r1.input();
    r1.output() >> r2.input();

    graph_type g1 = release(g0, { r0, r1, r2 });

    ASSERT_EQ(g0.size(), 0);
    ASSERT_EQ(g1.size(), 3);

    EXPECT_TRUE(g1.contains(r0));
    EXPECT_TRUE(g1.contains(r1));
    EXPECT_TRUE(g1.contains(r2));

    EXPECT_EQ(r0.output().opposite().get(), &r1.input());
    EXPECT_EQ(r1.output().opposite().get(), &r2.input());
}

TEST_F(relation_graph_test, release_partial) {
    graph_type g0;
    auto&& r0 = g0.insert(filter { constant(1) });
    auto&& r1 = g0.insert(filter { constant(2) });
    auto&& r2 = g0.insert(filter { constant(3) });
    auto&& r3 = g0.insert(filter { constant(4) });

    r0.output() >> r1.input();
    r1.output() >> r2.input();
    r2.output() >> r3.input();

    graph_type g1 = release(g0, { r2, r3 });

    ASSERT_EQ(g0.size(), 2);
    ASSERT_EQ(g1.size(), 2);

    EXPECT_TRUE(g0.contains(r0));
    EXPECT_TRUE(g0.contains(r1));
    EXPECT_TRUE(g1.contains(r2));
    EXPECT_TRUE(g1.contains(r3));

    EXPECT_EQ(r0.output().opposite().get(), &r1.input());
    EXPECT_TRUE(r1.output().opposite().empty());

    EXPECT_TRUE(r2.input().opposite().empty());
    EXPECT_EQ(r2.output().opposite().get(), &r3.input());
}

} // namespace takatori::relation
