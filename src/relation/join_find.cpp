#include <takatori/relation/join_find.h>

#include <takatori/tree/tree_element_util.h>
#include <takatori/tree/tree_element_forward.h>
#include <takatori/tree/tree_fragment_vector_forward.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>

namespace takatori::relation {

join_find::join_find(
        operator_kind_type operator_kind,
        descriptor::relation source,
        std::vector<column, util::object_allocator<column>> columns,
        std::vector<key, util::object_allocator<key>> keys,
        util::unique_object_ptr<scalar::expression> condition,
        util::object_creator creator) noexcept
    : left_(*this, 0, creator)
    , output_(*this, 0, creator)
    , operator_kind_(operator_kind)
    , source_(std::move(source))
    , columns_(std::move(columns))
    , keys_(*this, std::move(keys))
    , condition_(tree::bless_element(*this, std::move(condition)))
{}

join_find::join_find(
        operator_kind_type operator_kind,
        descriptor::relation source,
        std::initializer_list<column> columns,
        std::initializer_list<util::rvalue_reference_wrapper<key>> keys,
        util::rvalue_ptr<scalar::expression> condition) noexcept
    : join_find(
            operator_kind,
            std::move(source),
            { columns.begin(), columns.end() },
            { keys.begin(), keys.end() },
            util::clone_unique(condition))
{}

join_find::join_find(join_find const& other, util::object_creator creator)
    : join_find(
            other.operator_kind_,
            other.source_,
            { other.columns_, creator.allocator<column>() },
            tree::forward(creator, other.keys_),
            tree::forward(creator, other.condition_),
            creator)
{}

join_find::join_find(join_find&& other, util::object_creator creator)
    : join_find(
            other.operator_kind_,
            std::move(other.source_),
            { std::move(other.columns_), creator.allocator<column>() },
            tree::forward(creator, std::move(other.keys_)),
            tree::forward(creator, std::move(other.condition_)),
            creator)
{}

expression_kind join_find::kind() const noexcept {
    return tag;
}

util::sequence_view<join_find::input_port_type> join_find::input_ports() noexcept {
    return util::sequence_view { std::addressof(left_) };
}

util::sequence_view<join_find::input_port_type const> join_find::input_ports() const noexcept {
    return util::sequence_view { std::addressof(left_) };
}

util::sequence_view<join_find::output_port_type> join_find::output_ports() noexcept {
    return util::sequence_view { std::addressof(output_) };
}

util::sequence_view<join_find::output_port_type const> join_find::output_ports() const noexcept {
    return util::sequence_view { std::addressof(output_) };
}

join_find* join_find::clone(util::object_creator creator) const& {
    return creator.create_object<join_find>(*this, creator);
}

join_find* join_find::clone(util::object_creator creator)&& {
    return creator.create_object<join_find>(std::move(*this), creator);
}

join_find::input_port_type& join_find::left() noexcept {
    return left_;
}

join_find::input_port_type const& join_find::left() const noexcept {
    return left_;
}

join_find::output_port_type& join_find::output() noexcept {
    return output_;
}

join_find::output_port_type const& join_find::output() const noexcept {
    return output_;
}

join_find::operator_kind_type join_find::operator_kind() const noexcept {
    return operator_kind_;
}

join_find& join_find::operator_kind(join_find::operator_kind_type operator_kind) noexcept {
    operator_kind_ = operator_kind;
    return *this;
}

descriptor::relation& join_find::source() noexcept {
    return source_;
}

descriptor::relation const& join_find::source() const noexcept {
    return source_;
}

std::vector<join_find::column, util::object_allocator<join_find::column>>& join_find::columns() noexcept {
    return columns_;
}

std::vector<join_find::column, util::object_allocator<join_find::column>> const& join_find::columns() const noexcept {
    return columns_;
}

tree::tree_fragment_vector<join_find::key>& join_find::keys() noexcept {
    return keys_;
}

tree::tree_fragment_vector<join_find::key> const& join_find::keys() const noexcept {
    return keys_;
}

util::optional_ptr<scalar::expression> join_find::condition() noexcept {
    return util::optional_ptr { condition_.get() };
}

util::optional_ptr<scalar::expression const> join_find::condition() const noexcept {
    return util::optional_ptr { condition_.get() };
}

util::unique_object_ptr<scalar::expression> join_find::release_condition() noexcept {
    return tree::release_element(std::move(condition_));
}

join_find& join_find::condition(util::unique_object_ptr<scalar::expression> condition) noexcept {
    return tree::assign_element(*this, condition_, std::move(condition));
}

util::object_ownership_reference<scalar::expression> join_find::ownership_condition() noexcept {
    return tree::ownership_element(*this, condition_);
}

bool operator==(join_find const& a, join_find const& b) noexcept {
    return a.operator_kind() == b.operator_kind()
        && a.source() == b.source()
        && a.columns() == b.columns()
        && a.keys() == b.keys()
        && a.condition() == b.condition();
}

bool operator!=(join_find const& a, join_find const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, join_find const& value) {
    return out << value.kind() << "("
               << "operator_kind=" << value.operator_kind() << ", "
               << "source=" << value.source() << ", "
               << "columns=" << util::print_support { value.columns() } << ", "
               << "keys=" << value.keys() << ", "
               << "condition=" << value.condition() << ")";
}

bool join_find::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<join_find>(other);
}

std::ostream& join_find::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation
