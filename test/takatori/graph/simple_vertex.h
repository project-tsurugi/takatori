#pragma once

#include <vector>

#include <takatori/graph/vertex_base.h>

#include <takatori/util/optional_ptr.h>

namespace takatori::graph {

template<template <class, port_direction> class Port>
class simple_vertex : public vertex_base<simple_vertex<Port>, Port> {
public:
    using base = vertex_base<simple_vertex, Port>;
    using graph_type = typename base::graph_type;
    using input_port_type = typename base::input_port_type;
    using output_port_type = typename base::output_port_type;

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

    simple_vertex* clone() const& {
         return new simple_vertex(value_ * 2); // NOLINT
    }

    /// @copydoc clone()
    simple_vertex* clone() && {
         return new simple_vertex(value_ + 1); // NOLINT
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
