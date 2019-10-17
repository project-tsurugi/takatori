#include "model/scalar/conditional_alternative.h"
#include "model/scalar/conditional.h"

#include "model/tree_element_util.h"
#include "model/tree_element_forward.h"

namespace model::scalar {

static_assert(is_tree_fragment_v<conditional_alternative>);

conditional_alternative::conditional_alternative(
        util::unique_object_ptr<expression> condition,
        util::unique_object_ptr<expression> body) noexcept
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

conditional_alternative::conditional_alternative(
        conditional_alternative const& other,
        util::object_creator creator)
    : conditional_alternative(
            forward(creator, other.condition_),
            forward(creator, other.body_))
{}

conditional_alternative::conditional_alternative(
        conditional_alternative&& other,
        util::object_creator creator)
    : conditional_alternative(
            forward(creator, std::move(other.condition_)),
            forward(creator, std::move(other.body_)))
{}

conditional* conditional_alternative::parent_element() noexcept {
    return parent_;
}

conditional const* conditional_alternative::parent_element() const noexcept {
    return parent_;
}

void conditional_alternative::parent_element(conditional* parent) noexcept {
    parent_ = parent;
    fire_parent_element_changed();
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

conditional_alternative& conditional_alternative::condition(util::unique_object_ptr<expression> condition) noexcept {
    assign_element_fragment(parent_, condition_, std::move(condition));
    return *this;
}

util::unique_object_ptr<expression> conditional_alternative::release_condition() noexcept {
    return release_element(std::move(condition_));
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

conditional_alternative& conditional_alternative::body(util::unique_object_ptr<expression> body) noexcept {
    assign_element_fragment(parent_, body_, std::move(body));
    return *this;
}

util::unique_object_ptr<expression> conditional_alternative::release_body() noexcept {
    return release_element(std::move(body_));
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

void conditional_alternative::fire_parent_element_changed() noexcept {
    if (parent_ != nullptr) {
        bless_element(*parent_, condition_);
        bless_element(*parent_, body_);
    }
}

} // namespace model::scalar
