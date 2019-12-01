#pragma once

#include <vector>

#include "takatori/graph/vertex_base.h"

#include "takatori/util/object_creator.h"
#include "takatori/util/optional_ptr.h"

namespace takatori::graph {

class simple_vertex : public vertex_base<simple_vertex> {
public:
    explicit simple_vertex(int value) noexcept : value_(value) {}

    int value() const noexcept {
        return value_;
    }

    simple_vertex& value(int value) noexcept {
        value_ = value;
        return *this;
    }

    util::optional_ptr<graph_type> optional_owner() noexcept override {
        return util::optional_ptr { owner_ };
    }

    util::optional_ptr<graph_type const> optional_owner() const noexcept override  {
        return util::optional_ptr { owner_ };
    }

    input_port_type& input() noexcept {
        return input_;
    }

    output_port_type& output() noexcept {
        return output_;
    }

    util::sequence_view<input_port_type> input_ports() noexcept override {
        return util::sequence_view { &input_ };
    }

    util::sequence_view<input_port_type const> input_ports() const noexcept override {
        return util::sequence_view { &input_ };
    }

    util::sequence_view<output_port_type> output_ports() noexcept override {
        return util::sequence_view { &output_ };
    }

    util::sequence_view<output_port_type const> output_ports() const noexcept override {
        return util::sequence_view { &output_ };
    }

    simple_vertex* clone(util::object_creator creator) const& {
        return creator.create_object<simple_vertex>(value_ * 2);
    }

    /// @copydoc clone()
    simple_vertex* clone(util::object_creator creator) && {
        return creator.create_object<simple_vertex>(value_ + 1);
    }

    void on_join(graph_type* graph) noexcept override {
        owner_ = graph;
    }

private:
    int value_;
    input_port_type input_ { *this, 0 };
    output_port_type output_ { *this, 0 };

    graph_type* owner_ {};
};

} // namespace takatori::graph
