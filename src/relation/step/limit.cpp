#include "takatori/relation/step/limit.h"

#include "takatori/util/downcast.h"

namespace takatori::relation::step {

limit::limit(
        size_type count,
        util::object_creator creator) noexcept
    : input_(*this, 0, creator)
    , output_(*this, 0, creator)
    , count_(count)
{}

limit::limit(limit const& other, util::object_creator creator)
    : limit(
            other.count_,
            creator)
{}

limit::limit(limit&& other, util::object_creator creator)
    : limit(
            other.count_,
            creator)
{}

expression_kind limit::kind() const noexcept {
    return tag;
}

limit* limit::clone(util::object_creator creator) const& {
    return creator.create_object<limit>(*this, creator);
}

limit* limit::clone(util::object_creator creator)&& {
    return creator.create_object<limit>(std::move(*this), creator);
}

util::sequence_view<limit::input_port_type> limit::input_ports() noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<limit::input_port_type const> limit::input_ports() const noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<limit::output_port_type> limit::output_ports() noexcept {
    return util::sequence_view { std::addressof(output_) };
}

util::sequence_view<limit::output_port_type const> limit::output_ports() const noexcept {
    return util::sequence_view { std::addressof(output_) };
}

limit::input_port_type& limit::input() noexcept {
    return input_;
}

limit::input_port_type const& limit::input() const noexcept {
    return input_;
}

limit::output_port_type& limit::output() noexcept {
    return output_;
}

limit::output_port_type const& limit::output() const noexcept {
    return output_;
}

limit::size_type limit::count() const noexcept {
    return count_;
}

limit& limit::count(limit::size_type count) noexcept {
    count_ = count;
    return *this;
}

bool operator==(limit const& a, limit const& b) noexcept {
    return a.count() == b.count();
}

bool operator!=(limit const& a, limit const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, limit const& value) {
    return out << value.kind() << "("
               << "count=" << value.count() << ")";
}

bool limit::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<limit>(other);
}

std::ostream& limit::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation::step
