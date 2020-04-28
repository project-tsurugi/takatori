#include <takatori/graph/port.h>

#include <gtest/gtest.h>

#include "simple_vertex.h"

#include <takatori/graph/graph.h>

namespace takatori::graph {

class port_test : public ::testing::Test {};

using vertex = simple_vertex<port>;
using simple_graph = typename vertex::graph_type;
using simple_input = typename vertex::input_port_type;
using simple_output = typename vertex::input_port_type;

TEST_F(port_test, simple) {
    simple_graph g;
    auto&& v = g.emplace(100);

    EXPECT_EQ(v.input().index(), 0);
    EXPECT_EQ(&v.input().owner(), &v);
    EXPECT_FALSE(v.input().opposite());
}

TEST_F(port_test, connect) {
    simple_graph g;
    auto&& v1 = g.emplace(100);
    auto&& v2 = g.emplace(200);
    auto&& v3 = g.emplace(300);

    v1.output() >> v2.input();
    v3.input() << v2.output();

    EXPECT_FALSE(v1.input().opposite());
    EXPECT_FALSE(v3.output().opposite());

    EXPECT_EQ(v1.output().opposite().get(), &v2.input());
    EXPECT_EQ(v2.input().opposite().get(), &v1.output());

    EXPECT_EQ(v2.output().opposite().get(), &v3.input());
    EXPECT_EQ(v3.input().opposite().get(), &v2.output());
}

TEST_F(port_test, connect_multi) {
    simple_graph g;
    auto&& v1 = g.emplace(100);
    auto&& v2 = g.emplace(200);
    auto&& v3 = g.emplace(200);

    EXPECT_TRUE(v1.output().connect_to(v2.input()));
    EXPECT_EQ(v1.output().opposite().get(), &v2.input());
    EXPECT_EQ(v2.input().opposite().get(), &v1.output());

    EXPECT_FALSE(v1.output().connect_to(v2.input()));
    EXPECT_EQ(v1.output().opposite().get(), &v2.input());
    EXPECT_EQ(v2.input().opposite().get(), &v1.output());

    EXPECT_ANY_THROW(v1.output() >> v3.input());
    EXPECT_ANY_THROW(v3.output() >> v2.input());
}

TEST_F(port_test, reconnect) {
    simple_graph g;
    auto&& v1 = g.emplace(100);
    auto&& v2 = g.emplace(200);
    auto&& v3 = g.emplace(200);

    v1.output() >> v2.input();
    v2.output() >> v3.input();

    auto existing = v1.output().reconnect_to(v3.input());
    EXPECT_GT(v1.output(), v3.input());
    EXPECT_EQ(existing.get(), std::addressof(v2.input()));
    EXPECT_FALSE(v2.input().opposite());
    EXPECT_FALSE(v2.output().opposite());
}

TEST_F(port_test, disconnect) {
    simple_graph g;
    auto&& v1 = g.emplace(100);
    auto&& v2 = g.emplace(200);

    v1.output() >> v2.input();
    EXPECT_EQ(v1.output().opposite().get(), &v2.input());
    EXPECT_EQ(v2.input().opposite().get(), &v1.output());

    EXPECT_TRUE(v1.output().disconnect_from(v2.input()));
    EXPECT_FALSE(v1.output().opposite());
    EXPECT_FALSE(v2.input().opposite());

    EXPECT_FALSE(v1.output().disconnect_from(v2.input()));
    EXPECT_FALSE(v1.output().opposite());
    EXPECT_FALSE(v2.input().opposite());
}

TEST_F(port_test, disconnect_all) {
    simple_graph g;
    auto&& v1 = g.emplace(100);
    auto&& v2 = g.emplace(200);

    v1.output() >> v2.input();
    EXPECT_EQ(v1.output().opposite().get(), &v2.input());
    EXPECT_EQ(v2.input().opposite().get(), &v1.output());

    v2.input().disconnect_all();
    EXPECT_FALSE(v1.output().opposite());
    EXPECT_FALSE(v2.input().opposite());
}

TEST_F(port_test, dispose) {
    simple_graph g;
    auto&& v1 = g.emplace(100);
    auto&& v2 = g.emplace(200);

    v1.output() >> v2.input();

    g.erase(v2);
    EXPECT_FALSE(v1.output().opposite());
}

TEST_F(port_test, move_ctor) {
    simple_graph g;
    auto&& v1 = g.emplace(100);
    auto&& v2 = g.emplace(200);

    v1.output() >> v2.input();
    EXPECT_EQ(v1.output().opposite().get(), &v2.input());
    EXPECT_EQ(v2.input().opposite().get(), &v1.output());

    simple_input p { std::move(v2.input()) };
    EXPECT_FALSE(v2.input().opposite());

    EXPECT_EQ(v1.output().opposite().get(), &p);
    EXPECT_EQ(p.opposite().get(), &v1.output());
}

TEST_F(port_test, move_assign) {
    simple_graph g;
    auto&& v1 = g.emplace(100);
    auto&& v2 = g.emplace(200);

    v1.output() >> v2.input();
    EXPECT_EQ(v1.output().opposite().get(), &v2.input());
    EXPECT_EQ(v2.input().opposite().get(), &v1.output());

    simple_input p { v2, 1 };
    p = std::move(v2.input());
    EXPECT_FALSE(v2.input().opposite());

    EXPECT_EQ(v1.output().opposite().get(), &p);
    EXPECT_EQ(p.opposite().get(), &v1.output());
}

TEST_F(port_test, output) {
    simple_graph g;
    auto&& v1 = g.emplace(100);
    std::cout << v1.input() << std::endl;
    std::cout << v1.output() << std::endl;
}

} // namespace takatori::graph
