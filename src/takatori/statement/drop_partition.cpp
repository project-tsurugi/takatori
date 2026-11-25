#include <takatori/statement/drop_partition.h>

#include <takatori/util/downcast.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::statement {

drop_partition::drop_partition(
        descriptor::relation index,
        std::vector<details::partition_criteria> target) noexcept :
    index_ { std::move(index) },
    target_ { std::move(target) }
{}

drop_partition::drop_partition(util::clone_tag_t, drop_partition const& other) :
    drop_partition {
            other.index_,
            other.target_,
    }
{}

drop_partition::drop_partition(util::clone_tag_t, drop_partition&& other) noexcept :
    drop_partition {
            std::move(other.index_),
            std::move(other.target_),
    }
{}

statement_kind drop_partition::kind() const noexcept {
    return tag;
}

drop_partition* drop_partition::clone() const& {
    return new drop_partition(util::clone_tag, *this); // NOLINT
}

drop_partition* drop_partition::clone() && {
    return new drop_partition(util::clone_tag, std::move(*this)); // NOLINT;
}

descriptor::relation& drop_partition::index() noexcept {
    return index_;
}

descriptor::relation const& drop_partition::index() const noexcept {
    return index_;
}

std::vector<details::partition_criteria>& drop_partition::target() noexcept {
    return target_;
}

std::vector<details::partition_criteria> const& drop_partition::target() const noexcept {
    return target_;
}

bool operator==(drop_partition const& a, drop_partition const& b) noexcept {
    return a.index() == b.index()
            && a.target() == b.target();
}

bool operator!=(drop_partition const& a, drop_partition const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, drop_partition const& value) {
    using util::print_support;
    return out << value.kind() << "("
               << "index=" << value.index() << ", "
               << "target=" << print_support { value.target() } << ")";
}

bool drop_partition::equals(statement const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<drop_partition>(other);
}

std::ostream& drop_partition::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::statement
