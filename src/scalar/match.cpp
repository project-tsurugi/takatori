#include "takatori/scalar/match.h"

#include "takatori/tree/tree_element_util.h"
#include "takatori/tree/tree_element_forward.h"

#include "takatori/util/clonable.h"
#include "takatori/util/downcast.h"

namespace takatori::scalar {

match::match(
        match::operator_kind_type operator_kind,
        util::unique_object_ptr<expression> input,
        util::unique_object_ptr<expression> pattern,
        util::unique_object_ptr<expression> escape) noexcept
    : operator_kind_(operator_kind)
    , input_(tree::bless_element(*this, std::move(input)))
    , pattern_(tree::bless_element(*this, std::move(pattern)))
    , escape_(tree::bless_element(*this, std::move(escape)))
{}

match::match(
        match::operator_kind_type operator_kind,
        expression&& input,
        expression&& pattern,
        expression&& escape) noexcept
    : match(
            operator_kind,
            util::clone_unique(std::move(input)),
            util::clone_unique(std::move(pattern)),
            util::clone_unique(std::move(escape)))
{}

match::match(match const& other, util::object_creator creator) noexcept
    : match(
            other.operator_kind_,
            tree::forward(creator, other.input_),
            tree::forward(creator, other.pattern_),
            tree::forward(creator, other.escape_))
{}

match::match(match&& other, util::object_creator creator) noexcept
    : match(
            other.operator_kind_,
            tree::forward(creator, std::move(other.input_)),
            tree::forward(creator, std::move(other.pattern_)),
            tree::forward(creator, std::move(other.escape_)))
{}

expression_kind match::kind() const noexcept {
    return tag;
}

match* match::clone(util::object_creator creator) const& {
    return creator.create_object<match>(*this, creator);
}

match* match::clone(util::object_creator creator) && {
    return creator.create_object<match>(std::move(*this), creator);
}

match::operator_kind_type match::operator_kind() const noexcept {
    return operator_kind_;
}

match& match::operator_kind(match::operator_kind_type operator_kind) noexcept {
    operator_kind_ = operator_kind;
    return *this;
}

expression& match::input() noexcept {
    return *input_;
}

expression const& match::input() const noexcept {
    return *input_;
}

util::optional_ptr<expression> match::optional_input() noexcept {
    return util::optional_ptr { input_.get() };
}

util::optional_ptr<expression const> match::optional_input() const noexcept {
    return util::optional_ptr { input_.get() };
}

util::unique_object_ptr<expression> match::release_input() noexcept {
    return tree::release_element(std::move(input_));
}

match& match::input(util::unique_object_ptr<expression> input) noexcept {
    return tree::assign_element(*this, input_, std::move(input));
}

expression& match::pattern() noexcept {
    return *pattern_;
}

expression const& match::pattern() const noexcept {
    return *pattern_;
}

util::optional_ptr<expression> match::optional_pattern() noexcept {
    return util::optional_ptr { pattern_.get() };
}

util::optional_ptr<expression const> match::optional_pattern() const noexcept {
    return util::optional_ptr { pattern_.get() };
}

util::unique_object_ptr<expression> match::release_pattern() noexcept {
    return tree::release_element(std::move(pattern_));
}

match& match::pattern(util::unique_object_ptr<expression> pattern) noexcept {
    return tree::assign_element(*this, pattern_, std::move(pattern));
}

expression& match::escape() noexcept {
    return *escape_;
}

expression const& match::escape() const noexcept {
    return *escape_;
}

util::optional_ptr<expression> match::optional_escape() noexcept {
    return util::optional_ptr { escape_.get() };
}

util::optional_ptr<expression const> match::optional_escape() const noexcept {
    return util::optional_ptr { escape_.get() };
}

util::unique_object_ptr<expression> match::release_escape() noexcept {
    return tree::release_element(std::move(escape_));
}

match& match::escape(util::unique_object_ptr<expression> escape) noexcept {
    return tree::assign_element(*this, escape_, std::move(escape));
}

bool operator==(match const& a, match const& b) noexcept {
    return a.operator_kind() == b.operator_kind()
        && a.optional_input() == b.optional_input()
        && a.optional_pattern() == b.optional_pattern()
        && a.optional_escape() == b.optional_escape();
}

bool operator!=(match const& a, match const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, match const& value) {
    return out << value.kind() << "("
               << "operator_kind=" << value.operator_kind() << ", "
               << "input=" << value.optional_input() << ", "
               << "pattern=" << value.optional_pattern() << ", "
               << "escape=" << value.optional_escape() << ")";
}

bool match::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<match>(other);
}

std::ostream& match::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::scalar
