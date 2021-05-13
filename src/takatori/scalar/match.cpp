#include <takatori/scalar/match.h>

#include <takatori/tree/tree_element_util.h>
#include <takatori/tree/tree_element_forward.h>

#include <takatori/util/clonable.h>
#include <takatori/util/downcast.h>

namespace takatori::scalar {

match::match(
        match::operator_kind_type operator_kind,
        std::unique_ptr<expression> input,
        std::unique_ptr<expression> pattern,
        std::unique_ptr<expression> escape) noexcept
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

match::match(util::clone_tag_t, match const& other) noexcept
    : match(
            other.operator_kind_,
            tree::forward(other.input_),
            tree::forward(other.pattern_),
            tree::forward(other.escape_))
{}

match::match(util::clone_tag_t, match&& other) noexcept
    : match(
            other.operator_kind_,
            tree::forward(std::move(other.input_)),
            tree::forward(std::move(other.pattern_)),
            tree::forward(std::move(other.escape_)))
{}

expression_kind match::kind() const noexcept {
    return tag;
}

match* match::clone() const& {
    return new match(util::clone_tag, *this); // NOLINT
}

match* match::clone() && {
    return new match(util::clone_tag, std::move(*this)); // NOLINT;
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

std::unique_ptr<expression> match::release_input() noexcept {
    return tree::release_element(std::move(input_));
}

match& match::input(std::unique_ptr<expression> input) noexcept {
    return tree::assign_element(*this, input_, std::move(input));
}

util::ownership_reference<expression> match::ownership_input() {
    return tree::ownership_element(*this, input_);
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

std::unique_ptr<expression> match::release_pattern() noexcept {
    return tree::release_element(std::move(pattern_));
}

match& match::pattern(std::unique_ptr<expression> pattern) noexcept {
    return tree::assign_element(*this, pattern_, std::move(pattern));
}

util::ownership_reference<expression> match::ownership_pattern() {
    return tree::ownership_element(*this, pattern_);
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

std::unique_ptr<expression> match::release_escape() noexcept {
    return tree::release_element(std::move(escape_));
}

match& match::escape(std::unique_ptr<expression> escape) noexcept {
    return tree::assign_element(*this, escape_, std::move(escape));
}

util::ownership_reference<expression> match::ownership_escape() {
    return tree::ownership_element(*this, escape_);
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
