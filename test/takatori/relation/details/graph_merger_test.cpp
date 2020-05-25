#include "relation/details/graph_merger.h"

#include <gtest/gtest.h>

#include <takatori/relation/test_utils.h>

#include <takatori/relation/filter.h>
#include <takatori/relation/buffer.h>
#include <takatori/relation/intermediate/union.h>

#include <takatori/util/clonable.h>

namespace takatori::relation::details {

class graph_merger_test : public ::testing::Test {};

using intermediate::union_;
using G = graph_merger::graph_type;

TEST_F(graph_merger_test, simple) {
    G g;
    auto&& s1 = g.insert(filter { constant(1) });
    auto&& s2 = g.insert(filter { constant(2) });
    s1.output() >> s2.input();

    G h;
    graph_merger m { h };
    m.add(g);
    m.resolve();

    ASSERT_EQ(g.size(), 2);
    ASSERT_EQ(h.size(), 2);

    auto d1 = util::dynamic_pointer_cast<filter>(m.find(s1));
    auto d2 = util::dynamic_pointer_cast<filter>(m.find(s2));
    ASSERT_EQ(s1, d1);
    ASSERT_EQ(s2, d2);
    EXPECT_EQ(&d1->owner(), &h);
    EXPECT_EQ(&d2->owner(), &h);
    EXPECT_EQ(d1->output().opposite().get(), &d2->input());
    EXPECT_EQ(d2->input().opposite().get(), &d1->output());
}

TEST_F(graph_merger_test, multi_inputs) {
    G g;
    auto&& s1 = g.insert(filter { constant(1) });
    auto&& s2 = g.insert(filter { constant(2) });
    auto&& s3 = g.insert(union_ {});
    s1.output() >> s3.left();
    s2.output() >> s3.right();

    G h;
    graph_merger m { h };
    m.add(g);
    m.resolve();

    auto d1 = util::dynamic_pointer_cast<filter>(m.find(s1));
    auto d2 = util::dynamic_pointer_cast<filter>(m.find(s2));
    auto d3 = util::dynamic_pointer_cast<union_>(m.find(s3));
    ASSERT_EQ(s1, d1);
    ASSERT_EQ(s2, d2);
    ASSERT_EQ(s3, d3);
    EXPECT_EQ(d1->output().opposite().get(), &d3->left());
    EXPECT_EQ(d2->output().opposite().get(), &d3->right());
}

TEST_F(graph_merger_test, multi_outputs) {
    G g;
    auto&& s1 = g.insert(buffer { 2 });
    auto&& s2 = g.insert(filter { constant(2) });
    auto&& s3 = g.insert(filter { constant(3) });
    s1.output_ports()[0] >> s2.input();
    s1.output_ports()[1] >> s3.input();

    G h;
    graph_merger m { h };
    m.add(g);
    m.resolve();

    auto d1 = util::dynamic_pointer_cast<buffer>(m.find(s1));
    auto d2 = util::dynamic_pointer_cast<filter>(m.find(s2));
    auto d3 = util::dynamic_pointer_cast<filter>(m.find(s3));
    ASSERT_EQ(s1, d1);
    ASSERT_EQ(s2, d2);
    ASSERT_EQ(s3, d3);
    EXPECT_EQ(&d1->owner(), &h);
    EXPECT_EQ(&d2->owner(), &h);
    EXPECT_EQ(d1->output_ports()[0].opposite().get(), &d2->input());
    EXPECT_EQ(d1->output_ports()[1].opposite().get(), &d3->input());
}

TEST_F(graph_merger_test, move) {
    G g;
    auto&& s1 = g.insert(filter { constant(1) });
    auto&& s2 = g.insert(filter { constant(2) });
    s1.output() >> s2.input();

    G h;
    graph_merger m { h };
    m.add(std::move(g));
    m.resolve();

    ASSERT_EQ(h.size(), 2);
    EXPECT_EQ(&s1.owner(), &h);
    EXPECT_EQ(&s2.owner(), &h);

    EXPECT_EQ(s1.output().opposite().get(), &s2.input());
    EXPECT_EQ(s2.input().opposite().get(), &s1.output());
}

TEST_F(graph_merger_test, move_copy) {
    G g;
    auto* p1 = &g.insert(filter { constant(1) });
    auto* p2 = &g.insert(filter { constant(2) });
    p1->output() >> p2->input();

    util::pmr::monotonic_buffer_resource r;
    util::object_creator custom { &r };
    G h { custom };
    graph_merger m { h };
    m.add(std::move(g));
    m.resolve();

    ASSERT_EQ(h.size(), 2);
    auto i1 = std::find(h.begin(), h.end(), filter { constant(1) });
    auto i2 = std::find(h.begin(), h.end(), filter { constant(2) });
    ASSERT_NE(i1, h.end());
    ASSERT_NE(i2, h.end());

    auto&& d1 = util::downcast<filter>(*i1);
    auto&& d2 = util::downcast<filter>(*i2);
    EXPECT_EQ(&d1.owner(), &h);
    EXPECT_EQ(&d2.owner(), &h);
    EXPECT_EQ(d1.output().opposite().get(), &d2.input());

    EXPECT_NE(&d1, p1);
    EXPECT_NE(&d2, p2);
}

} // namespace takatori::relation::details
