#include "takatori/graph/graph.h"

#include <gtest/gtest.h>

#include "simple_vertex.h"

#include "takatori/graph/graph_element_traits.h"

namespace takatori::graph {

class graph_test : public ::testing::Test {
public:
    template<class T>
    static std::remove_reference_t<T> const& make_const(T&& v) { return v; }

    template<class T>
    static constexpr bool is_const_v = std::is_const_v<std::remove_reference_t<T>>;
};

static_assert(is_graph_element_v<simple_vertex>);

using traits = graph_element_traits<simple_vertex>;

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

    ASSERT_EQ(g.size(), 3);
    EXPECT_EQ(&g[v1.id()], &v1);
    EXPECT_EQ(&g[v2.id()], &v2);
    EXPECT_EQ(&g[v3.id()], &v3);
}

TEST_F(graph_test, at) {
    simple_graph g;
    auto&& v1 = g.emplace(100);
    auto&& v2 = g.emplace(200);
    auto&& v3 = g.emplace(300);

    ASSERT_EQ(g.size(), 3);
    EXPECT_EQ(&g.at(v1.id()), &v1);
    EXPECT_EQ(&g.at(v2.id()), &v2);
    EXPECT_EQ(&g.at(v3.id()), &v3);
    EXPECT_THROW(g.at(v1.id() + v2.id() + v3.id()), std::domain_error);
}

TEST_F(graph_test, at_const) {
    simple_graph mg;
    auto&& v1 = mg.emplace(100);
    auto&& v2 = mg.emplace(200);
    auto&& v3 = mg.emplace(300);

    auto&& g = make_const(mg);
    EXPECT_TRUE(is_const_v<decltype(g.at(v1.id()))>);
    ASSERT_EQ(g.size(), 3);
    EXPECT_EQ(&g.at(v1.id()), &v1);
    EXPECT_EQ(&g.at(v2.id()), &v2);
    EXPECT_EQ(&g.at(v3.id()), &v3);
    EXPECT_THROW(g.at(v1.id() + v2.id() + v3.id()), std::domain_error);
}

TEST_F(graph_test, operator_at) {
    simple_graph g;
    auto&& v1 = g.emplace(100);
    auto&& v2 = g.emplace(200);
    auto&& v3 = g.emplace(300);

    ASSERT_EQ(g.size(), 3);
    EXPECT_EQ(&g[v1.id()], &v1);
    EXPECT_EQ(&g[v2.id()], &v2);
    EXPECT_EQ(&g[v3.id()], &v3);
}

TEST_F(graph_test, operator_at_const) {
    simple_graph mg;
    auto&& v1 = mg.emplace(100);
    auto&& v2 = mg.emplace(200);
    auto&& v3 = mg.emplace(300);

    auto&& g = make_const(mg);
    EXPECT_TRUE(is_const_v<decltype(g[v1.id()])>);
    ASSERT_EQ(g.size(), 3);
    EXPECT_EQ(&g[v1.id()], &v1);
    EXPECT_EQ(&g[v2.id()], &v2);
    EXPECT_EQ(&g[v3.id()], &v3);
}

TEST_F(graph_test, find) {
    simple_graph g;
    auto&& v1 = g.emplace(100);
    auto&& v2 = g.emplace(200);
    auto&& v3 = g.emplace(300);

    EXPECT_EQ(g.find(v1.id()).get(), &v1);
    EXPECT_EQ(g.find(v2.id()).get(), &v2);
    EXPECT_EQ(g.find(v3.id()).get(), &v3);
    EXPECT_FALSE(g.find(v1.id() + v2.id() + v3.id()));
}

TEST_F(graph_test, find_const) {
    simple_graph mg;
    auto&& v1 = mg.emplace(100);
    auto&& v2 = mg.emplace(200);
    auto&& v3 = mg.emplace(300);

    auto&& g = make_const(mg);
    EXPECT_TRUE(is_const_v<decltype(g[v1.id()])>);
    EXPECT_EQ(g.find(v1.id()).get(), &v1);
    EXPECT_EQ(g.find(v2.id()).get(), &v2);
    EXPECT_EQ(g.find(v3.id()).get(), &v3);
    EXPECT_FALSE(g.find(v1.id() + v2.id() + v3.id()));
}

TEST_F(graph_test, contains) {
    simple_graph mg;
    auto&& v1 = mg.emplace(100);
    auto&& v2 = mg.emplace(200);
    auto&& v3 = mg.emplace(300);

    auto&& g = make_const(mg);
    EXPECT_TRUE(is_const_v<decltype(g[v1.id()])>);
    EXPECT_TRUE(g.contains(v1.id()));
    EXPECT_TRUE(g.contains(v2.id()));
    EXPECT_TRUE(g.contains(v3.id()));
    EXPECT_FALSE(g.contains(v1.id() + v2.id() + v3.id()));
}

TEST_F(graph_test, contains_element) {
    simple_graph mg;
    auto&& v1 = mg.emplace(100);
    auto&& v2 = mg.emplace(200);
    auto&& v3 = mg.emplace(300);

    auto&& g = make_const(mg);
    EXPECT_TRUE(is_const_v<decltype(g[v1.id()])>);
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

TEST_F(graph_test, erase_id) {
    simple_graph g;
    auto i1 = g.emplace(100).id();
    auto i2 = g.emplace(200).id();
    auto i3 = g.emplace(300).id();

    EXPECT_TRUE(g.contains(i1));
    EXPECT_TRUE(g.contains(i2));
    EXPECT_TRUE(g.contains(i3));

    EXPECT_TRUE(g.erase(i2));
    EXPECT_TRUE(g.contains(i1));
    EXPECT_FALSE(g.contains(i2));
    EXPECT_TRUE(g.contains(i3));

    EXPECT_FALSE(g.erase(i2));
}

TEST_F(graph_test, erase_element) {
    simple_graph g;
    auto&& v1 = g.emplace(100);
    auto&& v2 = g.emplace(200);
    auto&& v3 = g.emplace(300);

    EXPECT_TRUE(g.contains(v1));
    EXPECT_TRUE(g.contains(v2));
    EXPECT_TRUE(g.contains(v3));

    auto i2 = v2.id();
    EXPECT_TRUE(g.erase(v2));
    EXPECT_TRUE(g.contains(v1));
    EXPECT_FALSE(g.contains(i2));
    EXPECT_TRUE(g.contains(v3));

    EXPECT_FALSE(g.erase(i2));
}

TEST_F(graph_test, erase_iter) {
    simple_graph g;
    auto i1 = g.emplace(100).id();
    auto i2 = g.emplace(200).id();
    auto i3 = g.emplace(300).id();

    EXPECT_TRUE(g.contains(i1));
    EXPECT_TRUE(g.contains(i2));
    EXPECT_TRUE(g.contains(i3));

    auto iter = g.begin();
    bool saw1 = false, saw3 = false;
    while (iter != g.end()) {
        auto id = iter->id();
        if (id == i1) {
            saw1 = true;
            ++iter;
        } else if (id == i2) {
            iter = g.erase(iter);
        } else if (id == i3) {
            saw3 = true;
            ++iter;
        } else {
            FAIL();
        }
    }
    EXPECT_TRUE(saw1);
    EXPECT_TRUE(saw3);

    EXPECT_TRUE(g.contains(i1));
    EXPECT_FALSE(g.contains(i2));
    EXPECT_TRUE(g.contains(i3));
}

TEST_F(graph_test, insert) {
    simple_graph g;
    simple_vertex const v { 10 };
    auto&& r = g.insert(v);

    EXPECT_EQ(g.size(), 1);
    EXPECT_TRUE(g.contains(r.id()));
    EXPECT_EQ(r.value(), 20); // x2 on copy
    EXPECT_EQ(r.optional_owner().get(), &g);
    EXPECT_EQ(v.optional_owner().get(), nullptr);
}

TEST_F(graph_test, insert_rvalue) {
    simple_graph g;
    simple_vertex v { 10 };
    auto&& r = g.insert(std::move(v));

    EXPECT_EQ(g.size(), 1);
    EXPECT_TRUE(g.contains(r.id()));
    EXPECT_EQ(r.value(), 11); // +1 on copy
    EXPECT_EQ(r.optional_owner().get(), &g);
}

TEST_F(graph_test, emplace) {
    simple_graph g;
    auto&& r = g.emplace<simple_vertex>(10);

    EXPECT_EQ(g.size(), 1);
    EXPECT_TRUE(g.contains(r.id()));
    EXPECT_EQ(r.value(), 10);
    EXPECT_EQ(r.optional_owner().get(), &g);
}

TEST_F(graph_test, release_id) {
    simple_graph g;
    auto i1 = g.emplace(100).id();
    auto i2 = g.emplace(200).id();
    auto i3 = g.emplace(300).id();

    EXPECT_TRUE(g.contains(i1));
    EXPECT_TRUE(g.contains(i2));
    EXPECT_TRUE(g.contains(i3));

    auto r = g.release(i2);
    ASSERT_TRUE(r);
    EXPECT_NE(r->id(), i2);
    EXPECT_EQ(r->optional_owner().get(), nullptr);
    EXPECT_EQ(r->value(), 200);

    EXPECT_TRUE(g.contains(i1));
    EXPECT_FALSE(g.contains(i2));
    EXPECT_TRUE(g.contains(i3));

    EXPECT_FALSE(g.release(i2));
}

TEST_F(graph_test, release_element) {
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
    auto i1 = g.emplace(100).id();
    auto i2 = g.emplace(200).id();
    auto i3 = g.emplace(300).id();

    EXPECT_TRUE(g.contains(i1));
    EXPECT_TRUE(g.contains(i2));
    EXPECT_TRUE(g.contains(i3));

    auto iter = g.begin();
    bool saw1 = false, saw3 = false;
    while (iter != g.end()) {
        auto id = iter->id();
        if (id == i1) {
            saw1 = true;
            ++iter;
        } else if (id == i2) {
            auto [r, next] = g.release(iter);
            ASSERT_TRUE(r);
            EXPECT_NE(r->id(), i2);
            EXPECT_EQ(r->optional_owner().get(), nullptr);
            EXPECT_EQ(r->value(), 200);
            iter = next;
        } else if (id == i3) {
            saw3 = true;
            ++iter;
        } else {
            FAIL();
        }
    }
    EXPECT_TRUE(saw1);
    EXPECT_TRUE(saw3);

    EXPECT_TRUE(g.contains(i1));
    EXPECT_FALSE(g.contains(i2));
    EXPECT_TRUE(g.contains(i3));
}

TEST_F(graph_test, object_creator) {
    util::pmr::monotonic_buffer_resource resource;
    util::object_creator custom { &resource };

    simple_graph g { custom };
    EXPECT_EQ(g.get_object_creator(), custom);

    auto i1 = g.emplace(100).id();
    auto i2 = g.emplace(200).id();
    auto i3 = g.emplace(300).id();

    EXPECT_TRUE(g.contains(i1));
    EXPECT_TRUE(g.contains(i2));
    EXPECT_TRUE(g.contains(i3));
}

} // namespace takatori::graph
