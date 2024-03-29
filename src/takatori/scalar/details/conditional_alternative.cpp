#include <takatori/scalar/details/conditional_alternative.h>
#include <takatori/scalar/conditional.h>

#include <takatori/tree/tree_element_util.h>
#include <takatori/tree/tree_element_forward.h>

namespace takatori::scalar::details {

conditional_alternative::conditional_alternative(
        std::unique_ptr<expression> condition,
        std::unique_ptr<expression> body) noexcept
    : condition_(std::move(condition))
    , body_(std::move(body))
{}

conditional_alternative::conditional_alternative(
        expression&& condition,
        expression&& body) noexcept
    : conditional_alternative(
            util::clone_unique(std::move(condition)),
            util::clone_unique(std::move(body)))
{}

conditional_alternative::conditional_alternative(util::clone_tag_t, conditional_alternative const& other)
    : conditional_alternative(
            tree::forward(other.condition_),
            tree::forward(other.body_))
{}

conditional_alternative::conditional_alternative(util::clone_tag_t, conditional_alternative&& other)
    : conditional_alternative(
            tree::forward(std::move(other.condition_)),
            tree::forward(std::move(other.body_)))
{}

conditional* conditional_alternative::parent_element() noexcept {
    return parent_;
}

conditional const* conditional_alternative::parent_element() const noexcept {
    return parent_;
}

void conditional_alternative::parent_element(conditional* parent) noexcept {
    parent_ = parent;
    tree::bless_element(parent_, condition_);
    tree::bless_element(parent_, body_);
}

expression& conditional_alternative::condition() noexcept {
    return *condition_;
}

expression const& conditional_alternative::condition() const noexcept {
    return *condition_;
}

util::optional_ptr<expression> conditional_alternative::optional_condition() noexcept {
    return util::optional_ptr { condition_.get() };
}

util::optional_ptr<expression const> conditional_alternative::optional_condition() const noexcept {
    return util::optional_ptr { condition_.get() };
}

conditional_alternative& conditional_alternative::condition(std::unique_ptr<expression> condition) noexcept {
    tree::assign_element_fragment(parent_, condition_, std::move(condition));
    return *this;
}

std::unique_ptr<expression> conditional_alternative::release_condition() noexcept {
    return tree::release_element(std::move(condition_));
}

util::ownership_reference<expression> conditional_alternative::ownership_condition() {
    return tree::ownership_element_fragment(parent_, condition_);
}

expression& conditional_alternative::body() noexcept {
    return *body_;
}

expression const& conditional_alternative::body() const noexcept {
    return *body_;
}

util::optional_ptr<expression> conditional_alternative::optional_body() noexcept {
    return util::optional_ptr { body_.get() };
}

util::optional_ptr<expression const> conditional_alternative::optional_body() const noexcept {
    return util::optional_ptr { body_.get() };
}

conditional_alternative& conditional_alternative::body(std::unique_ptr<expression> body) noexcept {
    tree::assign_element_fragment(parent_, body_, std::move(body));
    return *this;
}

std::unique_ptr<expression> conditional_alternative::release_body() noexcept {
    return tree::release_element(std::move(body_));
}

util::ownership_reference<expression> conditional_alternative::ownership_body() {
    return tree::ownership_element_fragment(parent_, body_);
}

bool operator==(conditional_alternative const& a, conditional_alternative const& b) noexcept {
    return a.optional_condition() == b.optional_condition()
        && a.optional_body() == b.optional_body();
}

bool operator!=(conditional_alternative const& a, conditional_alternative const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, conditional_alternative const& value) {
    return out << "alternative("
               << "condition=" << value.optional_condition() << ", "
               << "body=" << value.optional_body() << ")";
}

} // namespace takatori::scalar::details
