#include "takatori/graph/graph.h"

#include <gtest/gtest.h>

#include "simple_vertex.h"

#include "takatori/graph/port.h"

#include "takatori/graph/graph_element_traits.h"

namespace takatori::graph {

class graph_test : public ::testing::Test {
public:
    template<class T>
    static std::remove_reference_t<T> const& make_const(T&& v) { return v; }

    template<class T>
    static constexpr bool is_const_v = std::is_const_v<std::remove_reference_t<T>>;
};

using vertex = simple_vertex<port>;

static_assert(is_graph_element_v<vertex>);

using traits = graph_element_traits<vertex>;

using simple_graph = traits::graph_type;

TEST_F(graph_test, simple) {
    simple_graph g;
    auto&& v1 = g.emplace(100);
    auto&& v2 = g.emplace(200);
    auto&& v3 = g.emplace(300);

    EXPECT_EQ(v1.value(), 100);
    EXPECT_EQ(v2.value(), 200);
    EXPECT_EQ(v3.value(), 300);

    EXPECT_EQ(&v1.owner(), &g);
    EXPECT_EQ(&v2.owner(), &g);
    EXPECT_EQ(&v3.owner(), &g);

    EXPECT_TRUE(g.contains(v1));
    EXPECT_TRUE(g.contains(v2));
    EXPECT_TRUE(g.contains(v3));
}

TEST_F(graph_test, contains) {
    simple_graph mg;
    auto&& v1 = mg.emplace(100);
    auto&& v2 = mg.emplace(200);
    auto&& v3 = mg.emplace(300);

    auto&& g = make_const(mg);
    EXPECT_TRUE(g.contains(v1));
    EXPECT_TRUE(g.contains(v2));
    EXPECT_TRUE(g.contains(v3));

    simple_graph other;
    EXPECT_FALSE(g.contains(other.emplace(100)));
}

TEST_F(graph_test, empty) {
    simple_graph mg;
    auto&& g = make_const(mg);
    EXPECT_TRUE(g.empty());

    mg.emplace(100);
    EXPECT_FALSE(g.empty());

    mg.emplace(200);
    mg.emplace(300);
    EXPECT_FALSE(g.empty());
}

TEST_F(graph_test, size) {
    simple_graph mg;
    auto&& g = make_const(mg);
    EXPECT_EQ(g.size(), 0);

    mg.emplace(100);
    EXPECT_EQ(g.size(), 1);

    mg.emplace(200);
    EXPECT_EQ(g.size(), 2);

    mg.emplace(300);
    EXPECT_EQ(g.size(), 3);
}

TEST_F(graph_test, clear) {
    simple_graph g;
    g.emplace(100);
    g.emplace(200);
    g.emplace(300);
    EXPECT_EQ(g.size(), 3);

    g.clear();
    EXPECT_EQ(g.size(), 0);
}

TEST_F(graph_test, erase) {
    simple_graph g;
    auto&& v1 = g.emplace(100);
    auto&& v2 = g.emplace(200);
    auto&& v3 = g.emplace(300);

    EXPECT_TRUE(g.contains(v1));
    EXPECT_TRUE(g.contains(v2));
    EXPECT_TRUE(g.contains(v3));

    EXPECT_TRUE(g.erase(v2));
    ASSERT_EQ(g.size(), 2);
    EXPECT_TRUE(g.contains(v1));
    EXPECT_TRUE(g.contains(v3));
}

TEST_F(graph_test, erase_iter) {
    simple_graph g;
    auto&& v1 = g.emplace(100);
    auto&& v2 = g.emplace(200);
    auto&& v3 = g.emplace(300);

    EXPECT_TRUE(g.contains(v1));
    EXPECT_TRUE(g.contains(v2));
    EXPECT_TRUE(g.contains(v3));

    auto iter = g.begin();
    bool saw1 = false, saw3 = false;
    while (iter != g.end()) {
        auto&& v = *iter;
        if (&v == &v1) {
            saw1 = true;
            ++iter;
        } else if (&v == &v2) {
            iter = g.erase(iter);
        } else if (&v == &v3) {
            saw3 = true;
            ++iter;
        } else {
            FAIL();
        }
    }
    EXPECT_TRUE(saw1);
    EXPECT_TRUE(saw3);

    ASSERT_EQ(g.size(), 2);
    EXPECT_TRUE(g.contains(v1));
    EXPECT_TRUE(g.contains(v3));
}

TEST_F(graph_test, insert) {
    simple_graph g;
    vertex const v { 10 };
    auto&& r = g.insert(v);

    EXPECT_EQ(g.size(), 1);
    EXPECT_TRUE(g.contains(r));
    EXPECT_EQ(r.value(), 20); // x2 on copy
    EXPECT_EQ(r.optional_owner().get(), &g);
    EXPECT_EQ(v.optional_owner().get(), nullptr);
}

TEST_F(graph_test, insert_rvalue) {
    simple_graph g;
    vertex v { 10 };
    auto&& r = g.insert(std::move(v));

    EXPECT_EQ(g.size(), 1);
    EXPECT_TRUE(g.contains(r));
    EXPECT_EQ(r.value(), 11); // +1 on copy
    EXPECT_EQ(r.optional_owner().get(), &g);
}

TEST_F(graph_test, emplace) {
    simple_graph g;
    auto&& r = g.emplace<vertex>(10);

    EXPECT_EQ(g.size(), 1);
    EXPECT_TRUE(g.contains(r));
    EXPECT_EQ(r.value(), 10);
    EXPECT_EQ(r.optional_owner().get(), &g);
}

TEST_F(graph_test, release) {
    simple_graph g;
    auto&& v1 = g.emplace(100);
    auto&& v2 = g.emplace(200);
    auto&& v3 = g.emplace(300);

    EXPECT_TRUE(g.contains(v1));
    EXPECT_TRUE(g.contains(v2));
    EXPECT_TRUE(g.contains(v3));

    auto r = g.release(v2);
    ASSERT_TRUE(r);
    ASSERT_EQ(r.get(), std::addressof(v2));
    EXPECT_EQ(r->optional_owner().get(), nullptr);

    EXPECT_TRUE(g.contains(v1));
    EXPECT_FALSE(g.contains(v2));
    EXPECT_TRUE(g.contains(v3));

    EXPECT_FALSE(g.release(v2));
}

TEST_F(graph_test, release_iter) {
    simple_graph g;
    auto&& v1 = g.emplace(100);
    auto&& v2 = g.emplace(200);
    auto&& v3 = g.emplace(300);

    EXPECT_TRUE(g.contains(v1));
    EXPECT_TRUE(g.contains(v2));
    EXPECT_TRUE(g.contains(v3));

    auto iter = g.begin();
    bool saw1 = false, saw3 = false;
    while (iter != g.end()) {
        auto&& v = *iter;
        if (&v == &v1) {
            saw1 = true;
            ++iter;
        } else if (&v == &v2) {
            auto [r, next] = g.release(iter);
            ASSERT_TRUE(r);
            EXPECT_EQ(r.get(), &v2);
            EXPECT_EQ(r->optional_owner().get(), nullptr);
            EXPECT_EQ(r->value(), 200);
            iter = next;
        } else if (&v == &v3) {
            saw3 = true;
            ++iter;
        } else {
            FAIL();
        }
    }
    EXPECT_TRUE(saw1);
    EXPECT_TRUE(saw3);

    ASSERT_EQ(g.size(), 2);
    EXPECT_TRUE(g.contains(v1));
    EXPECT_TRUE(g.contains(v3));
}

TEST_F(graph_test, object_creator) {
    util::pmr::monotonic_buffer_resource resource;
    util::object_creator custom { &resource };

    simple_graph g { custom };
    EXPECT_EQ(g.get_object_creator(), custom);

    auto&& v1 = g.emplace(100);
    auto&& v2 = g.emplace(200);
    auto&& v3 = g.emplace(300);

    EXPECT_TRUE(g.contains(v1));
    EXPECT_TRUE(g.contains(v2));
    EXPECT_TRUE(g.contains(v3));
}

} // namespace takatori::graph
