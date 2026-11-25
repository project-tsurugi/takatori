#include <takatori/statement/details/partition_key_element.h>

#include <takatori/util/clonable.h>

namespace takatori::statement::details {

partition_key_element::partition_key_element(
        descriptor::variable column,
        name_type column_name,
        std::vector<predicate_type> predicates) noexcept :
    column_ { std::move(column) },
    column_name_ { std::move(column_name) },
    predicates_ { std::move(predicates) }
{}

partition_key_element::partition_key_element(
        descriptor::variable column,
        name_type column_name,
        std::initializer_list<std::pair<
                scalar::comparison_operator,
                util::rvalue_reference_wrapper<value::data>>> predicates) :
    partition_key_element {
            std::move(column),
            std::move(column_name),
            std::vector<predicate_type> {},
    }
{
    predicates_.reserve(predicates.size());
    for (auto&& [op, literal] : predicates) {
        predicates_.emplace_back(op, util::clone_shared(literal.get()));
    }
}

descriptor::variable& partition_key_element::column() noexcept {
    return column_;
}

descriptor::variable const& partition_key_element::column() const noexcept {
    return column_;
}

partition_key_element::name_type& partition_key_element::column_name() noexcept {
    return column_name_;
}

partition_key_element::name_type const& partition_key_element::column_name() const noexcept {
    return column_name_;
}

std::vector<partition_key_element::predicate_type>& partition_key_element::predicates() noexcept {
    return predicates_;
}

std::vector<partition_key_element::predicate_type> const& partition_key_element::predicates() const noexcept {
    return predicates_;
}

namespace {

bool equals(
        partition_key_element::predicate_type const& a,
        partition_key_element::predicate_type const& b) noexcept {
    if (a.first != b.first) {
        return false;
    }
    if (!a.second || !b.second) {
        return !a.second && !b.second;
    }
    return *a.second == *b.second;
}

} // namespace

bool operator==(partition_key_element const& a, partition_key_element const& b) noexcept {
    if (a.column() != b.column() || a.column_name() != b.column_name()) {
        return false;
    }
    auto&& ap = a.predicates();
    auto&& bp = b.predicates();
    if (ap.size() != bp.size()) {
        return false;
    }
    for (std::size_t i = 0, n = ap.size(); i < n; ++i) {
        if (!equals(ap[i], bp[i])) {
            return false;
        }
    }
    return true;
}

bool operator!=(partition_key_element const& a, partition_key_element const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, partition_key_element const& value) {
    out << "partition_key_element("
        << "column=" << value.column() << ", "
        << "column_name=" << value.column_name() << ", "
        << "predicates=[";
    bool first = true;
    for (auto&& [op, literal] : value.predicates()) {
        if (!first) {
            out << ", ";
        }
        first = false;
        out << "(" << op << ", ";
        if (literal) {
            out << *literal;
        } else {
            out << "(null)";
        }
        out << ")";
    }
    return out << "])";
}

} // namespace takatori::statement::details
