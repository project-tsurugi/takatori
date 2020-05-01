#include <takatori/statement/write.h>

#include <takatori/util/downcast.h>
#include <takatori/util/vector_print_support.h>

#include <takatori/tree/tree_fragment_vector_forward.h>

namespace takatori::statement {

write::write(
        operator_kind_type operator_kind,
        descriptor::relation destination,
        std::vector<column, util::object_allocator<column>> columns,
        std::vector<tuple, util::object_allocator<tuple>> tuples) noexcept
    : operator_kind_(operator_kind)
    , destination_(std::move(destination))
    , columns_(std::move(columns))
    , tuples_(*this, std::move(tuples))
{}

write::write(
        operator_kind_type operator_kind,
        descriptor::relation destination,
        std::initializer_list<column> columns,
        std::initializer_list<std::initializer_list<util::rvalue_reference_wrapper<scalar::expression>>> tuples)
    : write(
            operator_kind,
            std::move(destination),
            { columns.begin(), columns.end() },
            {})
{
    tuples_.reserve(tuples.size());
    for (auto&& tuple : tuples) {
        tuples_.emplace_back(tuple);
    }
}

write::write(write const& other, util::object_creator creator) noexcept
    : write(
            other.operator_kind_,
            other.destination_,
            { other.columns_, creator.allocator() },
            tree::forward(creator, other.tuples_))
{}

write::write(write&& other, util::object_creator creator) noexcept
    : write(
            other.operator_kind_,
            std::move(other.destination_),
            { std::move(other.columns_), creator.allocator() },
            tree::forward(creator, std::move(other.tuples_)))
{}

statement_kind write::kind() const noexcept {
    return tag;
}

write* write::clone(util::object_creator creator) const& {
    return creator.create_object<write>(*this, creator);
}

write* write::clone(util::object_creator creator) && {
    return creator.create_object<write>(std::move(*this), creator);
}

write::operator_kind_type write::operator_kind() const noexcept {
    return operator_kind_;
}

write& write::operator_kind(write::operator_kind_type operator_kind) noexcept {
    operator_kind_ = operator_kind;
    return *this;
}

descriptor::relation& write::destination() noexcept {
    return destination_;
}

descriptor::relation const& write::destination() const noexcept {
    return destination_;
}

std::vector<write::column, util::object_allocator<write::column>>& write::columns() noexcept {
    return columns_;
}

std::vector<write::column, util::object_allocator<write::column>> const& write::columns() const noexcept {
    return columns_;
}

tree::tree_fragment_vector<write::tuple>& write::tuples() noexcept {
    return tuples_;
}

tree::tree_fragment_vector<write::tuple> const& write::tuples() const noexcept {
    return tuples_;
}

bool operator==(write const& a, write const& b) noexcept {
    return a.operator_kind() == b.operator_kind()
            && a.destination() == b.destination()
            && a.columns() == b.columns()
            && a.tuples() == b.tuples();
}

bool operator!=(write const& a, write const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, write const& value) {
    return out << value.kind() << "("
            << "operator_kind=" << value.operator_kind() << ", "
            << "destination=" << value.destination() << ", "
            << "columns=" << util::print_support { value.columns() } << ", "
            << "tuples=" << value.tuples() << ")";
}

bool write::equals(statement const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<write>(other);
}

std::ostream& write::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::statement
