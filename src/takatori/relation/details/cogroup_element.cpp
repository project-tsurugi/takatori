#include <takatori/relation/details/cogroup_element.h>

#include <utility>

#include <takatori/util/print_support.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::relation::details {

cogroup_element::cogroup_element(
        descriptor::relation source,
        std::vector<column> columns,
        bool mandatory) noexcept
    : source_(std::move(source))
    , columns_(std::move(columns))
    , mandatory_(mandatory)
{}

cogroup_element::cogroup_element(
        descriptor::relation source,
        std::initializer_list<column> columns,
        bool mandatory)
    : cogroup_element(
            std::move(source),
            { columns.begin(), columns.end() },
            mandatory)
{}

cogroup_element::cogroup_element(util::clone_tag_t, cogroup_element const& other)
    : cogroup_element(
            other.source_,
            { other.columns_ },
            other.mandatory_)
{}

cogroup_element::cogroup_element(util::clone_tag_t, cogroup_element&& other)
    : cogroup_element(
            other.source_,
            { std::move(other.columns_) },
            other.mandatory_)
{}

descriptor::relation& cogroup_element::source() noexcept {
    return source_;
}

descriptor::relation const& cogroup_element::source() const noexcept {
    return source_;
}

std::vector<cogroup_element::column>& cogroup_element::columns() noexcept {
    return columns_;
}

std::vector<cogroup_element::column> const& cogroup_element::columns() const noexcept {
    return columns_;
}

bool cogroup_element::mandatory() const noexcept {
    return mandatory_;
}

cogroup_element& cogroup_element::mandatory(bool mandatory) noexcept {
    mandatory_ = mandatory;
    return *this;
}

bool operator==(cogroup_element const& a, cogroup_element const& b) noexcept {
    return a.source() == b.source()
        && a.columns() == b.columns()
        && a.mandatory() == b.mandatory();
}

bool operator!=(cogroup_element const& a, cogroup_element const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, cogroup_element const& value) {
    return out << "group" << "("
               << "source=" << value.source() << ", "
               << "columns=" << util::print_support { value.columns() } << ", "
               << "mandatory=" << util::print_support { value.mandatory() } << ")";
}

} // namespace takatori::relation::details
