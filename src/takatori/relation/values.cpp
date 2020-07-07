#include <takatori/relation/values.h>

#include <takatori/tree/tree_fragment_vector.h>
#include <takatori/tree/tree_fragment_vector_forward.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::relation {

values::values(
        std::vector<column, util::object_allocator<column>> columns,
        std::vector<row, util::object_allocator<row>> rows,
        util::object_creator creator) noexcept :
    output_ { *this, 0, creator },
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

values::values(values const& other, util::object_creator creator) :
    values {
            { other.columns_, creator.allocator() },
            tree::forward(creator, other.rows_),
            creator,
    }
{}

values::values(values&& other, util::object_creator creator) :
    values {
            { std::move(other.columns_), creator.allocator() },
            tree::forward(creator, std::move(other.rows_)),
            creator,
    }
{}

expression_kind values::kind() const noexcept {
    return tag;
}

values* values::clone(util::object_creator creator) const& {
    return creator.create_object<values>(*this, creator);
}

values* values::clone(util::object_creator creator)&& {
    return creator.create_object<values>(std::move(*this), creator);
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

std::vector<values::column, util::object_allocator<values::column>>& values::columns() noexcept {
    return columns_;
}

std::vector<values::column, util::object_allocator<values::column>> const& values::columns() const noexcept {
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
