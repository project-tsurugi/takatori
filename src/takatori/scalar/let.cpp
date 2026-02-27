#include <takatori/scalar/let.h>

#include <takatori/tree/tree_element_util.h>
#include <takatori/tree/tree_element_forward.h>
#include <takatori/tree/tree_fragment_vector_forward.h>

#include <takatori/util/downcast.h>

namespace takatori::scalar {

let::let(
        std::vector<declarator> variables,
        std::unique_ptr<expression> body) noexcept :
    variables_ {
            *this,
            std::move(variables),
    },
    body_ {
            tree::bless_element(*this, std::move(body))
    }
{}

let::let(
        declarator&& variable,
        expression&& body) :
    let {
            { util::rvalue_reference_wrapper { std::move(variable) } },
            std::move(body),
    }
{}

let::let(
        std::initializer_list<util::rvalue_reference_wrapper<declarator>> variables,
        expression&& body) :
    let {
            { variables.begin(), variables.end() },
            util::clone_unique(std::move(body)),
    }
{}

let::let(util::clone_tag_t, let const& other):
    let {
            tree::forward(other.variables_),
            tree::forward(other.body_),
    }
{}

let::let(util::clone_tag_t, let&& other) :
    let {
            tree::forward(std::move(other.variables_)),
            tree::forward(std::move(other.body_)),
    }
{}

expression_kind let::kind() const noexcept {
    return tag;
}

let* let::clone() const& {
    return new let(util::clone_tag, *this); // NOLINT
}

let* let::clone() && {
    return new let(util::clone_tag, std::move(*this)); // NOLINT;
}

tree::tree_fragment_vector<let::declarator>& let::variables() noexcept {
    return variables_;
}

tree::tree_fragment_vector<let::declarator> const& let::variables() const noexcept {
    return variables_;
}

expression& let::body() noexcept {
    return *body_;
}

expression const& let::body() const noexcept {
    return *body_;
}

util::optional_ptr<expression> let::optional_body() noexcept {
    return util::optional_ptr { body_.get() };
}

util::optional_ptr<expression const> let::optional_body() const noexcept {
    return util::optional_ptr { body_.get() };
}

std::unique_ptr<expression> let::release_body() noexcept {
    return tree::release_element(std::move(body_));
}

let& let::body(std::unique_ptr<expression> body) noexcept {
    return tree::assign_element(*this, body_, std::move(body));
}

util::ownership_reference<expression> let::ownership_body() noexcept {
    return tree::ownership_element(*this, body_);
}

bool operator==(let const& a, let const& b) noexcept {
    return a.variables() == b.variables()
        && a.body() == b.body();
}

bool operator!=(let const& a, let const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, let const& value) {
    return out << value.kind() << "("
               << "variables=" << value.variables() << ", "
               << "body=" << value.body() << ")";
}

bool let::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<let>(other);
}

std::ostream& let::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::scalar
