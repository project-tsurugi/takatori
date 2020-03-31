#include <takatori/graph/graph.h>

#include <iterator>
#include <type_traits>

#include <gtest/gtest.h>

#include "simple_vertex.h"

#include <takatori/graph/port.h>

#include <takatori/graph/graph_element_traits.h>

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

static_assert(std::is_same_v<typename std::iterator_traits<simple_graph::iterator>::iterator_category, std::bidirectional_iterator_tag>);

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
    EXPECT_TRUE(g.contains(v1));
    EXPECT_TRUE(g.contains(v2));
    EXPECT_TRUE(g.contains(v3));
}

TEST_F(graph_test, ctor_move) {
    simple_graph mg;
    auto&& v1 = mg.emplace(100);
    auto&& v2 = mg.emplace(200);
    auto&& v3 = mg.emplace(300);

    simple_graph g { std::move(mg) };

    EXPECT_EQ(v1.value(), 100);
    EXPECT_EQ(v2.value(), 200);
    EXPECT_EQ(v3.value(), 300);

    EXPECT_EQ(&v1.owner(), &g);
    EXPECT_EQ(&v2.owner(), &g);
    EXPECT_EQ(&v3.owner(), &g);

    ASSERT_EQ(g.size(), 3);
    EXPECT_TRUE(g.contains(v1));
    EXPECT_TRUE(g.contains(v2));
    EXPECT_TRUE(g.contains(v3));
}

TEST_F(graph_test, assign_move) {
    simple_graph mg;
    auto&& v1 = mg.emplace(100);
    auto&& v2 = mg.emplace(200);
    auto&& v3 = mg.emplace(300);

    simple_graph g;
    g.emplace(400);
    g.emplace(500);

    g = std::move(mg);

    EXPECT_EQ(v1.value(), 100);
    EXPECT_EQ(v2.value(), 200);
    EXPECT_EQ(v3.value(), 300);

    EXPECT_EQ(&v1.owner(), &g);
    EXPECT_EQ(&v2.owner(), &g);
    EXPECT_EQ(&v3.owner(), &g);

    ASSERT_EQ(g.size(), 3);
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

TEST_F(graph_test, find) {
    simple_graph g;

    g.emplace(100);
    auto&& v2 = g.emplace(200);
    g.emplace(300);

    auto it0 = g.find(v2);
    ASSERT_NE(it0, g.end());
    EXPECT_EQ(&*it0, &v2);

    vertex vx { 999 };
    auto it1 = g.find(vx);
    ASSERT_EQ(it1, g.end());
}

TEST_F(graph_test, find_const) {
    simple_graph mg;

    mg.emplace(100);
    auto&& v2 = mg.emplace(200);
    mg.emplace(300);

    auto&& g = make_const(mg);

    auto it0 = g.find(v2);
    ASSERT_NE(it0, g.end());
    EXPECT_EQ(&*it0, &v2);

    vertex vx { 999 };
    auto it1 = g.find(vx);
    ASSERT_EQ(it1, g.end());
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
    EXPECT_EQ(r.value(), 11); // +1 on move
    EXPECT_EQ(r.optional_owner().get(), &g);
}

TEST_F(graph_test, insert_unique_ptr) {
    util::object_creator c;
    auto v = c.create_unique<vertex>(10);
    auto* p = v.get();

    simple_graph g { c };
    auto&& r = g.insert(std::move(v));

    EXPECT_EQ(g.size(), 1);
    EXPECT_TRUE(g.contains(r));
    EXPECT_EQ(&r, p);
    EXPECT_EQ(r.value(), 10);
    EXPECT_EQ(r.optional_owner().get(), &g);
}

TEST_F(graph_test, insert_unique_ptr_custom) {
    util::pmr::monotonic_buffer_resource mbr;
    util::object_creator c { &mbr };
    auto v = c.create_unique<vertex>(10);
    auto* p = v.get();

    simple_graph g;
    auto&& r = g.insert(std::move(v));

    EXPECT_EQ(g.size(), 1);
    EXPECT_TRUE(g.contains(r));
    EXPECT_NE(&r, p);
    EXPECT_EQ(r.value(), 11); // +1 on move
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

TEST_F(graph_test, merge) {
    simple_graph g;
    auto&& r1 = g.emplace<vertex>(10);
    auto&& r2 = g.emplace<vertex>(20);
    r1.output() >> r2.input();

    simple_graph h;
    auto&& r3 = h.emplace<vertex>(30);
    auto&& r4 = h.emplace<vertex>(40);
    r3.output() >> r4.input();

    g.merge(std::move(h));

    EXPECT_EQ(g.size(), 4);
    EXPECT_TRUE(g.contains(r1));
    EXPECT_TRUE(g.contains(r2));
    EXPECT_TRUE(g.contains(r3));
    EXPECT_TRUE(g.contains(r4));

    EXPECT_EQ(h.size(), 0);

    EXPECT_EQ(&r1.owner(), &g);
    EXPECT_EQ(&r2.owner(), &g);
    EXPECT_EQ(&r3.owner(), &g);
    EXPECT_EQ(&r4.owner(), &g);

    EXPECT_EQ(r1.value(), 10);
    EXPECT_EQ(r2.value(), 20);
    EXPECT_EQ(r3.value(), 30);
    EXPECT_EQ(r4.value(), 40);

    EXPECT_EQ(r1.output().opposite().get(), &r2.input());
    EXPECT_EQ(r2.input().opposite().get(), &r1.output());
    EXPECT_EQ(r3.output().opposite().get(), &r4.input());
    EXPECT_EQ(r4.input().opposite().get(), &r3.output());
}

TEST_F(graph_test, swap) {
    simple_graph g;
    auto&& r1 = g.emplace<vertex>(10);
    auto&& r2 = g.emplace<vertex>(20);
    r1.output() >> r2.input();

    simple_graph h;
    auto&& r3 = h.emplace<vertex>(30);
    auto&& r4 = h.emplace<vertex>(40);
    r3.output() >> r4.input();

    g.swap(h);

    EXPECT_EQ(g.size(), 2);
    EXPECT_TRUE(g.contains(r3));
    EXPECT_TRUE(g.contains(r4));

    EXPECT_EQ(h.size(), 2);
    EXPECT_TRUE(h.contains(r1));
    EXPECT_TRUE(h.contains(r2));

    EXPECT_EQ(&r1.owner(), &h);
    EXPECT_EQ(&r2.owner(), &h);
    EXPECT_EQ(&r3.owner(), &g);
    EXPECT_EQ(&r4.owner(), &g);

    EXPECT_EQ(r1.value(), 10);
    EXPECT_EQ(r2.value(), 20);
    EXPECT_EQ(r3.value(), 30);
    EXPECT_EQ(r4.value(), 40);

    EXPECT_EQ(r1.output().opposite().get(), &r2.input());
    EXPECT_EQ(r2.input().opposite().get(), &r1.output());
    EXPECT_EQ(r3.output().opposite().get(), &r4.input());
    EXPECT_EQ(r4.input().opposite().get(), &r3.output());
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
