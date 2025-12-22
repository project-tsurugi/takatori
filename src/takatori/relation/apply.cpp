#include <takatori/relation/apply.h>

#include <takatori/tree/tree_element_vector_forward.h>

#include <takatori/util/downcast.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::relation {

apply::apply(
        operator_kind_type operator_kind,
        descriptor::function function,
        util::reference_vector<scalar::expression> arguments,
        std::vector<column_type> columns) noexcept:
    input_ { *this, 0 },
    output_ { *this, 0 },
    operator_kind_ { operator_kind },
    function_ { std::move(function) },
    arguments_ { *this, std::move(arguments) },
    columns_ { std::move(columns) }
{}

apply::apply(
        descriptor::function function,
        util::rvalue_initializer_list<scalar::expression> arguments,
        std::initializer_list<column_type> columns,
        operator_kind_type operator_kind):
    apply {
            operator_kind,
            std::move(function),
            { arguments.begin(), arguments.end() },
            { columns.begin(), columns.end() }
    }
{}

apply::apply(util::clone_tag_t, apply const& other):
    apply {
            other.operator_kind_,
            other.function_,
            tree::forward(other.arguments_),
            other.columns_,
    }
{}

apply::apply(util::clone_tag_t, apply&& other):
    apply {
            other.operator_kind_,
            std::move(other.function_),
            tree::forward(std::move(other.arguments_)),
            std::move(other.columns_),
    }
{}

expression_kind apply::kind() const noexcept {
    return tag;
}

util::sequence_view<apply::input_port_type> apply::input_ports() noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<apply::input_port_type const> apply::input_ports() const noexcept {
    return util::sequence_view { std::addressof(input_) };
}

util::sequence_view<apply::output_port_type> apply::output_ports() noexcept {
    return util::sequence_view { std::addressof(output_) };
}

util::sequence_view<apply::output_port_type const> apply::output_ports() const noexcept {
    return util::sequence_view { std::addressof(output_) };
}

apply* apply::clone() const& {
    return new apply(util::clone_tag, *this); // NOLINT
}

apply* apply::clone()&& {
    return new apply(util::clone_tag, std::move(*this)); // NOLINT;
}

expression::input_port_type& apply::input() noexcept {
    return input_;
}

expression::input_port_type const& apply::input() const noexcept {
    return input_;
}

expression::output_port_type& apply::output() noexcept {
    return output_;
}

expression::output_port_type const& apply::output() const noexcept {
    return output_;
}

apply::operator_kind_type apply::operator_kind() const noexcept {
    return operator_kind_;
}

apply& apply::operator_kind(operator_kind_type kind) noexcept {
    operator_kind_ = kind;
    return *this;
}

descriptor::function& apply::function() noexcept {
    return function_;
}

descriptor::function const& apply::function() const noexcept {
    return function_;
}

tree::tree_element_vector<scalar::expression>& apply::arguments() noexcept {
    return arguments_;
}

tree::tree_element_vector<scalar::expression> const& apply::arguments() const noexcept {
    return arguments_;
}

std::vector<apply::column_type>& apply::columns() noexcept {
    return columns_;
}

std::vector<apply::column_type> const& apply::columns() const noexcept {
    return columns_;
}

bool operator==(apply const& a, apply const& b) noexcept {
    return a.operator_kind_ == b.operator_kind_ &&
           a.function_ == b.function_ &&
           a.arguments_ == b.arguments_ &&
           a.columns_ == b.columns_;
}

bool operator!=(apply const& a, apply const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, apply const& value) {
    return out << value.kind() << "("
               << "operator_kind=" << value.operator_kind_ << ", "
               << "function=" << value.function_ << ", "
               << "arguments=" << value.arguments_ << ", "
               << "columns=" << util::print_support { value.columns_ } << ")";
}

bool apply::equals(expression const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<apply>(other);
}

std::ostream& apply::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::relation
