#include <takatori/plan/process.h>
#include <takatori/plan/exchange.h>

#include <algorithm>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>

namespace takatori::plan {

process::~process() {
    for (auto* e : upstreams_) {
        e->internal_remove_downstream(*this);
    }
    upstreams_.clear();
    for (auto* e : downstreams_) {
        e->internal_remove_upstream(*this);
    }
    downstreams_.clear();
}

process::process(util::object_creator creator) noexcept
    : operators_(creator)
    , upstreams_(creator.allocator<exchange*>())
    , downstreams_(creator.allocator<exchange*>())
{}

process::process(graph::graph<relation::expression> operators, util::object_creator creator) noexcept
    : operators_(std::move(operators))
    , upstreams_(creator.allocator<exchange*>())
    , downstreams_(creator.allocator<exchange*>())
{}

process::process(process const& other, util::object_creator creator)
    : process(creator)
{
    (void) other; // FIXME: impl
}

process::process(process&& other, util::object_creator creator)
    : process(
            std::move(other.operators_),
            creator)
{}

step_kind process::kind() const noexcept {
    return tag;
}

process* process::clone(util::object_creator creator) const& {
    return creator.create_object<process>(*this, creator);
}

process* process::clone(util::object_creator creator) && {
    return creator.create_object<process>(std::move(*this), creator);
}

graph::graph<relation::expression>& process::operators() noexcept {
    return operators_;
}

graph::graph<relation::expression> const& process::operators() const noexcept {
    return operators_;
}

step_list_view<exchange> process::upstreams() noexcept {
    return step_list_view<exchange> { upstreams_ };
}

step_list_view<exchange const> process::upstreams() const noexcept {
    return step_list_view<exchange const> { upstreams_ };
}

step_list_view<exchange> process::downstreams() noexcept {
    return step_list_view<exchange> { downstreams_ };
}

step_list_view<exchange const> process::downstreams() const noexcept {
    return step_list_view<exchange const> { downstreams_ };
}

bool process::has_upstream(exchange const& upstream) const noexcept {
    auto&& element = std::addressof(upstream);
    auto&& elements = upstreams_;
    auto iter = std::find(elements.begin(), elements.end(), element);
    return iter != elements.end();
}

process& process::add_upstream(exchange& upstream) {
    internal_add_upstream(upstream);
    upstream.internal_add_downstream(*this);
    return *this;
}

process& process::remove_upstream(exchange& upstream) {
    internal_remove_upstream(upstream);
    upstream.internal_remove_downstream(*this);
    return *this;
}

bool process::has_downstream(exchange const& downstream) const noexcept {
    auto&& element = std::addressof(downstream);
    auto&& elements = downstreams_;
    auto iter = std::find(elements.begin(), elements.end(), element);
    return iter != elements.end();
}

process& process::add_downstream(exchange& downstream) {
    internal_add_downstream(downstream);
    downstream.internal_add_upstream(*this);
    return *this;
}

process& process::remove_downstream(exchange& downstream) {
    internal_remove_downstream(downstream);
    downstream.internal_remove_upstream(*this);
    return *this;
}

process& operator<<(process& downstream, exchange& upstream) {
    return downstream.add_upstream(upstream);
}

process& operator>>(process& upstream, exchange& downstream) {
    return upstream.add_downstream(downstream);
}

bool operator==(process const& a, process const& b) noexcept {
    return std::addressof(a) == std::addressof(b);
}

bool operator!=(process const& a, process const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, process const& value) {
    return out << value.kind() << "("
               << ")";
}

bool process::equals(step const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<process>(other);
}

std::ostream& process::print_to(std::ostream& out) const {
    return out << *this;
}

void process::internal_add_upstream(exchange& upstream) {
    auto element = std::addressof(upstream);
    auto&& elements = upstreams_;
    if (auto iter = std::find(elements.begin(), elements.end(), element); iter == elements.end()) {
        elements.push_back(element);
    }
}

void process::internal_remove_upstream(exchange& upstream) noexcept {
    auto element = std::addressof(upstream);
    auto&& elements = upstreams_;
    if (auto iter = std::find(elements.begin(), elements.end(), element); iter != elements.end()) {
        elements.erase(iter);
    }
}

void process::internal_add_downstream(exchange& downstream) {
    auto element = std::addressof(downstream);
    auto&& elements = downstreams_;
    if (auto iter = std::find(elements.begin(), elements.end(), element); iter == elements.end()) {
        elements.push_back(element);
    }
}

void process::internal_remove_downstream(exchange& downstream) noexcept {
    auto element = std::addressof(downstream);
    auto&& elements = downstreams_;
    if (auto iter = std::find(elements.begin(), elements.end(), element); iter != elements.end()) {
        elements.erase(iter);
    }
}

} // namespace takatori::plan
