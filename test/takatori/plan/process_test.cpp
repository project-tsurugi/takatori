#include <takatori/plan/process.h>

#include <gtest/gtest.h>

#include <takatori/plan/discard.h>

#include <takatori/relation/scan.h>
#include <takatori/relation/emit.h>
#include <takatori/relation/buffer.h>

#include "test_utils.h"

namespace takatori::plan {

class process_test : public ::testing::Test {};

static_assert(process::tag == step_kind::process);

template<class T>
static T& find(process& p) {
    for (auto&& e : p.operators()) {
        if (e.kind() == T::tag) {
            return util::unsafe_downcast<T>(e);
        }
    }
    std::abort();
}

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

TEST_F(process_test, copy) {
    process p0;
    auto&& r0 = p0.operators().insert(relation::scan {
            tabledesc("T"),
            {
                    { columndesc("C0"), vardesc(0) },
            }
    });
    auto&& r1 = p0.operators().insert(relation::emit {
            vardesc(0),
    });
    r0.output() >> r1.input();

    graph::graph<step> g;
    auto&& p1 = g.emplace<process>(p0, util::object_creator {});

    ASSERT_EQ(p0.operators().size(), 2);
    EXPECT_GT(r0.output(), r1.input());

    auto&& r0c = find<relation::scan>(p1);
    auto&& r1c = find<relation::emit>(p1);
    ASSERT_EQ(p1.operators().size(), 2);
    EXPECT_GT(r0c.output(), r1c.input());
}

TEST_F(process_test, move) {
    process p0;
    auto&& r0 = p0.operators().insert(relation::scan {
            tabledesc("T"),
            {
                    { columndesc("C0"), vardesc(0) },
            }
    });
    auto&& r1 = p0.operators().insert(relation::emit {
            vardesc(0),
    });
    r0.output() >> r1.input();

    graph::graph<step> g;
    auto&& p1 = g.emplace<process>(std::move(p0), util::object_creator {});

    ASSERT_EQ(p1.operators().size(), 2);
    auto&& r0c = find<relation::scan>(p1);
    auto&& r1c = find<relation::emit>(p1);
    EXPECT_GT(r0c.output(), r1c.input());

    EXPECT_EQ(std::addressof(r0c), std::addressof(r0));
    EXPECT_EQ(std::addressof(r1c), std::addressof(r1));
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
    auto&& p0 = g.emplace<process>();
    auto&& r0 = p0.operators().insert(relation::scan {
            tabledesc("T"),
            {
                    { columndesc("C0"), vardesc(0) },
            }
    });
    auto&& r1 = p0.operators().insert(relation::emit {
            vardesc(0),
    });
    r0.output() >> r1.input();

    std::cout << p0 << std::endl;
}

} // namespace takatori::plan
