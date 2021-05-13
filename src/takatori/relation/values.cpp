#include <takatori/relation/values.h>

#include <takatori/tree/tree_fragment_vector.h>
#include <takatori/tree/tree_fragment_vector_forward.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::relation {

values::values(
        std::vector<column> columns,
        std::vector<row> rows) noexcept :
    output_ { *this, 0 },
    columns_ { std::move(columns) },
    rows_ { *this, std::move(rows) }
{}

values::values(
        std::initializer_list<column> columns,
        std::initializer_list<std::initializer_list<util::rvalue_reference_wrapper<scalar::expression>>> rows) :
    values {
            { columns.begin(), columns.end() },
            {}
    }
{
    rows_.reserve(rows.size());
    for (auto&& row : rows) {
        rows_.emplace_back(row);
    }
}

values::values(util::clone_tag_t, values const& other) :
    values {
            { other.columns_ },
            tree::forward(other.rows_),
    }
{}

values::values(util::clone_tag_t, values&& other) :
    values {
            { std::move(other.columns_) },
            tree::forward(std::move(other.rows_)),
    }
{}

expression_kind values::kind() const noexcept {
    return tag;
}

values* values::clone() const& {
    return new values(util::clone_tag, *this); // NOLINT
}

values* values::clone()&& {
    return new values(util::clone_tag, std::move(*this)); // NOLINT;
}


util::sequence_view<values::input_port_type> values::input_ports() noexcept {
    return {};
}

util::sequence_view<values::input_port_type const> values::input_ports() const noexcept {
    return {};
}

util::sequence_view<values::output_port_type> values::output_ports() noexcept {
    return util::sequence_view { std::addressof(output_) };
}

util::sequence_view<values::output_port_type const> values::output_ports() const noexcept {
    return util::sequence_view { std::addressof(output_) };
}

values::output_port_type& values::output() noexcept {
    return output_;
}

values::output_port_type const& values::output() const noexcept {
    return output_;
}

std::vector<values::column>& values::columns() noexcept {
    return columns_;
}

std::vector<values::column> const& values::columns() const noexcept {
    return columns_;
}

tree::tree_fragment_vector<values::row>& values::rows() noexcept {
    return rows_;
}

tree::tree_fragment_vector<values::row> const& values::rows() const noexcept {
    return rows_;
}

bool operator==(values const& a, values const& b) noexcept {
    return a.columns() == b.columns()
        && a.rows() == b.rows();
}

bool operator!=(values const& a, values const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, values const& value) {
    return out << value.kind() << "("
               << "columns=" << util::print_support { value.columns() } << ", "
               << "rows=" << value.rows() << ")";
}

bool values::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<values>(other);
}

std::ostream& values::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation
