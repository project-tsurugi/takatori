#include "takatori/graph/multiport.h"

#include <gtest/gtest.h>

#include "simple_vertex.h"

#include "takatori/graph/graph.h"

namespace takatori::graph {

class multiport_test : public ::testing::Test {};

using simple_graph = simple_vertex::graph_type;
using simple_input = simple_vertex::input_port_type;
using simple_output = simple_vertex::input_port_type;

TEST_F(multiport_test, simple) {
    simple_graph g;
    auto&& v = g.emplace(100);

    EXPECT_EQ(v.input().id(), 0);
    EXPECT_EQ(&v.input().owner(), &v);
    EXPECT_TRUE(v.input().opposites().empty());
}

TEST_F(multiport_test, connect) {
    simple_graph g;
    auto&& v1 = g.emplace(100);
    auto&& v2 = g.emplace(200);
    auto&& v3 = g.emplace(300);

    v1.output() >> v3.input();
    v3.input() << v2.output();

    EXPECT_TRUE(v1.input().opposites().empty());
    EXPECT_TRUE(v2.input().opposites().empty());
    EXPECT_TRUE(v3.output().opposites().empty());

    {
        auto o = v1.output().opposites();
        ASSERT_EQ(o.size(), 1);
        EXPECT_EQ(&o[0], &v3.input());
    }
    {
        auto o = v2.output().opposites();
        ASSERT_EQ(o.size(), 1);
        EXPECT_EQ(&o[0], &v3.input());
    }
    {
        auto o = v3.input().opposites();
        ASSERT_EQ(o.size(), 2);
        EXPECT_NE(
                std::find_if(o.begin(), o.end(), [&](auto& p) { return &p == &v1.output(); }),
                o.end());
        EXPECT_NE(
                std::find_if(o.begin(), o.end(), [&](auto& p) { return &p == &v2.output(); }),
                o.end());
    }
}

TEST_F(multiport_test, connect_multi) {
    simple_graph g;
    auto&& v1 = g.emplace(100);
    auto&& v2 = g.emplace(200);

    v1.output() >> v2.input();
    v1.output() >> v2.input();

    {
        auto o = v1.output().opposites();
        ASSERT_EQ(o.size(), 2);
        EXPECT_EQ(&o[0], &v2.input());
        EXPECT_EQ(&o[1], &v2.input());
    }
    {
        auto o = v2.input().opposites();
        ASSERT_EQ(o.size(), 2);
        EXPECT_EQ(&o[0], &v1.output());
        EXPECT_EQ(&o[1], &v1.output());
    }

    v1.output().disconnect_from(v2.input());
    {
        auto o = v1.output().opposites();
        ASSERT_EQ(o.size(), 1);
        EXPECT_EQ(&o[0], &v2.input());
    }
    {
        auto o = v2.input().opposites();
        ASSERT_EQ(o.size(), 1);
        EXPECT_EQ(&o[0], &v1.output());
    }

    v1.output().disconnect_from(v2.input());
    EXPECT_TRUE(v1.output().opposites().empty());
    EXPECT_TRUE(v2.input().opposites().empty());
}

TEST_F(multiport_test, disconnect_all) {
    simple_graph g;
    auto&& v1 = g.emplace(100);
    auto&& v2 = g.emplace(200);

    v1.output() >> v2.input();
    v1.output() >> v2.input();

    v2.input().disconnect_all();
    EXPECT_TRUE(v1.output().opposites().empty());
    EXPECT_TRUE(v2.input().opposites().empty());
}

TEST_F(multiport_test, dispose) {
    simple_graph g;
    auto&& v1 = g.emplace(100);
    auto&& v2 = g.emplace(200);

    v1.output() >> v2.input();
    v1.output() >> v2.input();

    g.erase(v2);

    EXPECT_TRUE(v1.output().opposites().empty());
}

TEST_F(multiport_test, move_ctor) {
    simple_graph g;
    auto&& v1 = g.emplace(100);
    auto&& v2 = g.emplace(200);

    v1.output() >> v2.input();

    ASSERT_EQ(v1.output().opposites().size(), 1);
    EXPECT_EQ(&v1.output().opposites()[0], &v2.input());

    ASSERT_EQ(v2.input().opposites().size(), 1);
    EXPECT_EQ(&v2.input().opposites()[0], &v1.output());

    simple_input p { std::move(v2.input()) };
    EXPECT_TRUE(v2.input().opposites().empty());

    EXPECT_EQ(&p.owner(), &v2);

    ASSERT_EQ(p.opposites().size(), 1);
    EXPECT_EQ(&p.opposites()[0], &v1.output());

    ASSERT_EQ(v1.output().opposites().size(), 1);
    EXPECT_EQ(&v1.output().opposites()[0], &p);
}

TEST_F(multiport_test, move_assign) {
    simple_graph g;
    auto&& v1 = g.emplace(100);
    auto&& v2 = g.emplace(200);

    v1.output() >> v2.input();

    ASSERT_EQ(v1.output().opposites().size(), 1);
    ASSERT_EQ(v2.input().opposites().size(), 1);

    EXPECT_EQ(&v1.output().opposites()[0], &v2.input());
    EXPECT_EQ(&v2.input().opposites()[0], &v1.output());

    simple_input p { v2, 1 };
    p = std::move(v2.input());
    EXPECT_TRUE(v2.input().opposites().empty());

    EXPECT_EQ(&p.owner(), &v2);
    ASSERT_EQ(p.opposites().size(), 1);
    EXPECT_EQ(&p.opposites()[0], &v1.output());

    ASSERT_EQ(v1.output().opposites().size(), 1);
    EXPECT_EQ(&v1.output().opposites()[0], &p);
}

TEST_F(multiport_test, output) {
    simple_graph g;
    auto&& v1 = g.emplace(100);
    std::cout << v1.input() << std::endl;
    std::cout << v1.output() << std::endl;
}

} // namespace takatori::graph
