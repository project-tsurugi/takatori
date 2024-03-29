#include <takatori/relation/join_scan.h>

#include <takatori/tree/tree_element_util.h>
#include <takatori/tree/tree_element_forward.h>
#include <takatori/tree/tree_fragment_vector_forward.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>

#include <takatori/util/vector_print_support.h>

namespace takatori::relation {

join_scan::join_scan(
        operator_kind_type operator_kind,
        descriptor::relation source,
        std::vector<column> columns,
        endpoint lower,
        endpoint upper,
        std::unique_ptr<scalar::expression> condition) noexcept
    : left_(*this, 0)
    , output_(*this, 0)
    , operator_kind_(operator_kind)
    , source_(std::move(source))
    , columns_(std::move(columns))
    , lower_(tree::bless_element(*this, std::move(lower)))
    , upper_(tree::bless_element(*this, std::move(upper)))
    , condition_(tree::bless_element(*this, std::move(condition)))
{}

join_scan::join_scan(
        operator_kind_type operator_kind,
        descriptor::relation source,
        std::initializer_list<column> columns,
        endpoint lower,
        endpoint upper,
        util::rvalue_ptr<scalar::expression> condition) noexcept
    : join_scan(
            operator_kind,
            std::move(source),
            { columns.begin(), columns.end() },
            std::move(lower),
            std::move(upper),
            util::clone_unique(condition))
{}

join_scan::join_scan(util::clone_tag_t, join_scan const& other)
    : join_scan(
            other.operator_kind_,
            other.source_,
            { other.columns_ },
            endpoint { util::clone_tag, other.lower_ },
            endpoint { util::clone_tag, other.upper_ },
            tree::forward(other.condition_))
{}

join_scan::join_scan(util::clone_tag_t, join_scan&& other)
    : join_scan(
            other.operator_kind_,
            std::move(other.source_),
            { std::move(other.columns_) },
            endpoint { util::clone_tag, std::move(other.lower_) },
            endpoint { util::clone_tag, std::move(other.upper_) },
            tree::forward(std::move(other.condition_)))
{}

expression_kind join_scan::kind() const noexcept {
    return tag;
}

util::sequence_view<join_scan::input_port_type> join_scan::input_ports() noexcept {
    return util::sequence_view { std::addressof(left_) };
}

util::sequence_view<join_scan::input_port_type const> join_scan::input_ports() const noexcept {
    return util::sequence_view { std::addressof(left_) };
}

util::sequence_view<join_scan::output_port_type> join_scan::output_ports() noexcept {
    return util::sequence_view { std::addressof(output_) };
}

util::sequence_view<join_scan::output_port_type const> join_scan::output_ports() const noexcept {
    return util::sequence_view { std::addressof(output_) };
}

join_scan* join_scan::clone() const& {
    return new join_scan(util::clone_tag, *this); // NOLINT
}

join_scan* join_scan::clone()&& {
    return new join_scan(util::clone_tag, std::move(*this)); // NOLINT;
}

join_scan::input_port_type& join_scan::left() noexcept {
    return left_;
}

join_scan::input_port_type const& join_scan::left() const noexcept {
    return left_;
}

join_scan::output_port_type& join_scan::output() noexcept {
    return output_;
}

join_scan::output_port_type const& join_scan::output() const noexcept {
    return output_;
}

join_scan::operator_kind_type join_scan::operator_kind() const noexcept {
    return operator_kind_;
}

join_scan& join_scan::operator_kind(join_scan::operator_kind_type operator_kind) noexcept {
    operator_kind_ = operator_kind;
    return *this;
}

descriptor::relation& join_scan::source() noexcept {
    return source_;
}

descriptor::relation const& join_scan::source() const noexcept {
    return source_;
}

std::vector<join_scan::column>& join_scan::columns() noexcept {
    return columns_;
}

std::vector<join_scan::column> const& join_scan::columns() const noexcept {
    return columns_;
}

join_scan::endpoint& join_scan::lower() noexcept {
    return lower_;
}

join_scan::endpoint const& join_scan::lower() const noexcept {
    return lower_;
}

join_scan::endpoint& join_scan::upper() noexcept {
    return upper_;
}

join_scan::endpoint const& join_scan::upper() const noexcept {
    return upper_;
}

util::optional_ptr<scalar::expression> join_scan::condition() noexcept {
    return util::optional_ptr { condition_.get() };
}

util::optional_ptr<scalar::expression const> join_scan::condition() const noexcept {
    return util::optional_ptr { condition_.get() };
}

std::unique_ptr<scalar::expression> join_scan::release_condition() noexcept {
    return tree::release_element(std::move(condition_));
}

join_scan& join_scan::condition(std::unique_ptr<scalar::expression> condition) noexcept {
    return tree::assign_element(*this, condition_, std::move(condition));
}

util::ownership_reference<scalar::expression> join_scan::ownership_condition() noexcept {
    return tree::ownership_element(*this, condition_);
}

bool operator==(join_scan const& a, join_scan const& b) noexcept {
    return a.operator_kind() == b.operator_kind()
        && a.source() == b.source()
        && a.columns() == b.columns()
        && a.lower() == b.lower()
        && a.upper() == b.upper()
        && a.condition() == b.condition();
}

bool operator!=(join_scan const& a, join_scan const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, join_scan const& value) {
    return out << value.kind() << "("
               << "operator_kind=" << value.operator_kind() << ", "
               << "source=" << value.source() << ", "
               << "columns=" << util::print_support { value.columns() } << ", "
               << "lower=" << value.lower() << ", "
               << "upper=" << value.upper() << ", "
               << "condition=" << value.condition() << ")";
}

bool join_scan::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<join_scan>(other);
}

std::ostream& join_scan::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation
