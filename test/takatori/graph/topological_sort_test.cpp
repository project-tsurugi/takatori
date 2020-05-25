#include <takatori/graph/topological_sort.h>

#include <functional>
#include <vector>
#include <set>

#include <gtest/gtest.h>

#include <takatori/graph/graph.h>
#include <takatori/graph/vertex_base.h>
#include <takatori/graph/port.h>

namespace takatori::graph {

class topological_sort_test : public ::testing::Test {};

class vertex : public vertex_base<vertex, port> {
public:
    explicit vertex(int value, std::size_t n_input = 1, std::size_t n_output = 1)
        : value_(value)
    {
        for (std::size_t i = 0; i < n_input; ++i) {
            inputs_.emplace_back(*this, i);
        }
        for (std::size_t i = 0; i < n_output; ++i) {
            outputs_.emplace_back(*this, i);
        }
    }

    int value() const noexcept {
        return value_;
    }

    input_port_type& input(std::size_t index = 0) noexcept {
        return inputs_[index];
    }

    output_port_type& output(std::size_t index = 0) noexcept {
        return outputs_[index];
    }

    util::optional_ptr<graph_type> optional_owner() noexcept override {
        return util::optional_ptr { owner_ };
    }

    util::optional_ptr<graph_type const> optional_owner() const noexcept override  {
        return util::optional_ptr { owner_ };
    }

    util::sequence_view<input_port_type> input_ports() noexcept override {
        return inputs_;
    }

    util::sequence_view<input_port_type const> input_ports() const noexcept override {
        return inputs_;
    }

    util::sequence_view<output_port_type> output_ports() noexcept override {
        return outputs_;
    }

    util::sequence_view<output_port_type const> output_ports() const noexcept override {
        return outputs_;
    }

    vertex* clone(util::object_creator creator) const& {
        return creator.create_object<vertex>(value_, inputs_.size(), outputs_.size());
    }

    /// @copydoc clone()
    vertex* clone(util::object_creator creator) && {
        return creator.create_object<vertex>(value_, inputs_.size(), outputs_.size());
    }

    void on_join(graph_type* graph) noexcept override {
        owner_ = graph;
    }

private:
    int value_;
    std::vector<vertex_base::input_port_type> inputs_;
    std::vector<vertex_base::output_port_type> outputs_;

    graph_type* owner_ {};
};

using simple_graph = typename vertex::graph_type;
using simple_input = typename vertex::input_port_type;
using simple_output = typename vertex::input_port_type;

namespace {

struct enumerator {
    void operator()(vertex& v, std::function<void(vertex&)> const& c) {
        for (auto& e : v.output_ports()) {
            if (auto o = e.opposite()) {
                c(o->owner());
            }
        }
    }
};

struct consumer {
    std::vector<vertex*> r;

    void operator()(vertex& v) {
        r.emplace_back(std::addressof(v));
    }
};

bool eq(
        std::initializer_list<std::reference_wrapper<vertex const>> expect,
        std::initializer_list<vertex const*> actual) {
    std::set<vertex const*> e;
    for (vertex const& v : expect) {
        e.emplace(std::addressof(v));
    }
    for (vertex const* a : actual) {
        if (auto it = e.find(a); it != e.end()) {
            e.erase(it);
        } else {
            return false;
        }
    }
    return e.empty();
}

} // namespace

TEST_F(topological_sort_test, simple) {
    simple_graph g;
    auto&& v = g.emplace(100);

    consumer c;
    topological_sort<enumerator>(g, std::ref(c));

    auto&& r = c.r;
    ASSERT_EQ(r.size(), 1);
    EXPECT_EQ(r[0], std::addressof(v));
}

TEST_F(topological_sort_test, line) {
    simple_graph g;
    auto&& v1 = g.emplace(100);
    auto&& v2 = g.emplace(200);
    auto&& v3 = g.emplace(300);

    v1.output() >> v2.input();
    v2.output() >> v3.input();

    consumer c;
    topological_sort<enumerator>(g, std::ref(c));

    auto&& r = c.r;
    ASSERT_EQ(r.size(), 3);
    EXPECT_EQ(r[0], std::addressof(v3));
    EXPECT_EQ(r[1], std::addressof(v2));
    EXPECT_EQ(r[2], std::addressof(v1));
}

TEST_F(topological_sort_test, scatter) {
    simple_graph g;
    auto&& v1 = g.emplace(100, 0, 3);
    auto&& v2 = g.emplace(200, 1, 0);
    auto&& v3 = g.emplace(300, 1, 0);
    auto&& v4 = g.emplace(400, 1, 0);

    v1.output(0) >> v2.input();
    v1.output(1) >> v3.input();
    v1.output(2) >> v4.input();

    consumer c;
    topological_sort<enumerator>(g, std::ref(c));

    auto&& r = c.r;
    ASSERT_EQ(r.size(), 4);
    EXPECT_TRUE(eq(
            { v2, v3, v4 },
            { r[0], r[1], r[2] }));
    EXPECT_EQ(r[3], std::addressof(v1));
}

TEST_F(topological_sort_test, gather) {
    simple_graph g;
    auto&& v1 = g.emplace(100, 0, 1);
    auto&& v2 = g.emplace(200, 0, 1);
    auto&& v3 = g.emplace(300, 0, 1);
    auto&& v4 = g.emplace(400, 3, 0);

    v1.output() >> v4.input(0);
    v2.output() >> v4.input(1);
    v3.output() >> v4.input(2);

    consumer c;
    topological_sort<enumerator>(g, std::ref(c));

    auto&& r = c.r;
    ASSERT_EQ(r.size(), 4);
    EXPECT_EQ(r[0], std::addressof(v4));
    EXPECT_TRUE(eq(
            { v1, v2, v3 },
            { r[1], r[2], r[3] }));
}

TEST_F(topological_sort_test, diamond) {
    simple_graph g;
    auto&& v1 = g.emplace(100, 0, 3);
    auto&& v2 = g.emplace(200, 1, 1);
    auto&& v3 = g.emplace(300, 1, 1);
    auto&& v4 = g.emplace(400, 1, 1);
    auto&& v5 = g.emplace(500, 3, 0);

    v1.output(0) >> v2.input();
    v1.output(1) >> v3.input();
    v1.output(2) >> v4.input();

    v2.output() >> v5.input(0);
    v3.output() >> v5.input(1);
    v4.output() >> v5.input(2);

    consumer c;
    topological_sort<enumerator>(g, std::ref(c));

    auto&& r = c.r;
    ASSERT_EQ(r.size(), 5);
    EXPECT_EQ(r[0], std::addressof(v5));
    EXPECT_TRUE(eq(
            { v2, v3, v4 },
            { r[1], r[2], r[3] }));
    EXPECT_EQ(r[4], std::addressof(v1));
}

} // namespace takatori::graph
