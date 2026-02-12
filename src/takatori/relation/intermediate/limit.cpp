#include <takatori/relation/intermediate/limit.h>

#include <takatori/util/downcast.h>
#include <takatori/util/optional_print_support.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::relation::intermediate {

limit::limit(
        std::vector<descriptor::variable> group_keys,
        std::vector<sort_key> sort_keys,
        row_slice_type row_slice) noexcept :
    input_ { *this, 0 },
    output_ { *this, 0 },
    group_keys_ { std::move(group_keys) },
    sort_keys_ { std::move(sort_keys) },
    row_slice_ { std::move(row_slice) }
{}

limit::limit(row_slice_type row_slice) noexcept :
    limit {
            {},
            {},
            std::move(row_slice),
    }
{}

limit::limit(util::clone_tag_t, limit const& other) :
    limit {
            { other.group_keys_ },
            { other.sort_keys_ },
            other.row_slice_,
    }
{}

limit::limit(util::clone_tag_t, limit&& other) :
    limit {
            { std::move(other.group_keys_) },
            { std::move(other.sort_keys_) },
            std::move(other.row_slice_),
    }
{}

expression_kind limit::kind() const noexcept {
    return tag;
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

limit* limit::clone() const& {
    return new limit(util::clone_tag, *this); // NOLINT
}

limit* limit::clone()&& {
    return new limit(util::clone_tag, std::move(*this)); // NOLINT;
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

std::vector<descriptor::variable>& limit::group_keys() noexcept {
    return group_keys_;
}

std::vector<descriptor::variable> const& limit::group_keys() const noexcept {
    return group_keys_;
}

std::vector<limit::sort_key>& limit::sort_keys() noexcept {
    return sort_keys_;
}

std::vector<limit::sort_key> const& limit::sort_keys() const noexcept {
    return sort_keys_;
}

limit::row_slice_type& limit::row_slice() noexcept {
    return row_slice_;
}

limit::row_slice_type const& limit::row_slice() const noexcept {
    return row_slice_;
}

bool operator==(limit const& a, limit const& b) noexcept {
    return a.group_keys() == b.group_keys()
        && a.sort_keys() == b.sort_keys()
        && a.row_slice() == b.row_slice();
}

bool operator!=(limit const& a, limit const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, limit const& value) {
    return out << value.kind() << "("
               << "group_keys=" << util::print_support { value.group_keys() } << ", "
               << "sort_keys=" << util::print_support { value.sort_keys() } << ", "
               << "row_slice=" << util::print_support { value.row_slice() } << ")";
}

bool limit::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<limit>(other);
}

std::ostream& limit::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation::intermediate
