#include "takatori/plan/exchange.h"
#include "takatori/plan/process.h"

#include "takatori/util/clonable.h"
#include "takatori/util/downcast.h"

namespace takatori::plan {

exchange::~exchange() {
    for (auto* e : upstreams_) {
        e->internal_remove_downstream(*this);
    }
    upstreams_.clear();
    for (auto* e : downstreams_) {
        e->internal_remove_upstream(*this);
    }
    downstreams_.clear();
}

step_list_view<process> exchange::upstreams() noexcept {
    return step_list_view<process> { upstreams_ };
}

step_list_view<process const> exchange::upstreams() const noexcept {
    return step_list_view<process const> { upstreams_ };
}

step_list_view<process> exchange::downstreams() noexcept {
    return step_list_view<process> { downstreams_ };
}

step_list_view<process const> exchange::downstreams() const noexcept {
    return step_list_view<process const> { downstreams_ };
}

bool exchange::has_upstream(process const& upstream) const noexcept {
    auto&& element = std::addressof(upstream);
    auto&& elements = upstreams_;
    auto iter = std::find(elements.begin(), elements.end(), element);
    return iter != elements.end();
}

exchange& exchange::add_upstream(process& upstream) {
    internal_add_upstream(upstream);
    upstream.internal_add_downstream(*this);
    return *this;
}

exchange& exchange::remove_upstream(process& upstream) {
    internal_remove_upstream(upstream);
    upstream.internal_remove_downstream(*this);
    return *this;
}

bool exchange::has_downstream(process const& downstream) const noexcept {
    auto&& element = std::addressof(downstream);
    auto&& elements = downstreams_;
    auto iter = std::find(elements.begin(), elements.end(), element);
    return iter != elements.end();
}

exchange& exchange::add_downstream(process& downstream) {
    internal_add_downstream(downstream);
    downstream.internal_add_upstream(*this);
    return *this;
}

exchange& exchange::remove_downstream(process& downstream) {
    internal_remove_downstream(downstream);
    downstream.internal_remove_upstream(*this);
    return *this;
}

exchange& operator<<(exchange& downstream, process& upstream) {
    return downstream.add_upstream(upstream);
}

exchange& operator>>(exchange& upstream, process& downstream) {
    return upstream.add_downstream(downstream);
}

exchange::exchange(util::object_creator creator) noexcept
        : upstreams_(creator.allocator<process*>())
        , downstreams_(creator.allocator<process*>())
{}

void exchange::internal_add_upstream(process& upstream) {
    auto element = std::addressof(upstream);
    auto&& elements = upstreams_;
    if (auto iter = std::find(elements.begin(), elements.end(), element); iter == elements.end()) {
        elements.push_back(element);
    }
}

void exchange::internal_remove_upstream(process& upstream) noexcept {
    auto element = std::addressof(upstream);
    auto&& elements = upstreams_;
    if (auto iter = std::find(elements.begin(), elements.end(), element);
            iter != elements.end()) {
        elements.erase(iter);
    }
}

void exchange::internal_add_downstream(process& downstream) {
    auto element = std::addressof(downstream);
    auto&& elements = downstreams_;
    if (auto iter = std::find(elements.begin(), elements.end(), element); iter == elements.end()) {
        elements.push_back(element);
    }
}

void exchange::internal_remove_downstream(process& downstream) noexcept {
    auto element = std::addressof(downstream);
    auto&& elements = downstreams_;
    if (auto iter = std::find(elements.begin(), elements.end(), element); iter != elements.end()) {
        elements.erase(iter);
    }
}

} // namespace takatori::plan
