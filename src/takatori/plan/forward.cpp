#include <takatori/plan/forward.h>

#include <memory>

#include <takatori/util/downcast.h>
#include <takatori/util/vector_print_support.h>

namespace takatori::plan {

forward::forward(
        std::vector<descriptor::variable> columns,
        row_slice_type row_slice,
        ordering_type ordering) noexcept :
    columns_ { std::move(columns) },
    row_slice_ { std::move(row_slice) },
    ordering_ { ordering }
{}

forward::forward(
        row_slice_type row_slice,
        ordering_type ordering) noexcept :
    forward {
            decltype(columns_) {},
            std::move(row_slice),
            ordering,
    }
{}

forward::forward(
        std::initializer_list<descriptor::variable> columns,
        row_slice_type row_slice,
        ordering_type ordering) noexcept :
    forward {
            decltype(columns_) { columns.begin(), columns.end() },
            std::move(row_slice),
            ordering,
    }
{}

forward::forward(util::clone_tag_t, forward const& other) :
    forward {
            { other.columns_ },
            other.row_slice_,
            other.ordering_,
    }
{}

forward::forward(util::clone_tag_t, forward&& other) :
    forward {
            { std::move(other.columns_) },
            std::move(other.row_slice_),
            other.ordering_,
    }
{}

step_kind forward::kind() const noexcept {
    return tag;
}

forward* forward::clone() const& {
    return new forward(util::clone_tag, *this); // NOLINT
}

forward* forward::clone() && {
    return new forward(util::clone_tag, std::move(*this)); // NOLINT;
}

util::sequence_view<descriptor::variable const> forward::input_columns() const noexcept {
    return util::sequence_view { columns_ };
}

util::sequence_view<descriptor::variable const> forward::output_columns() const noexcept {
    return util::sequence_view { columns_ };
}

std::vector<descriptor::variable>& forward::columns() noexcept {
    return columns_;
}

std::vector<descriptor::variable> const& forward::columns() const noexcept {
    return columns_;
}

forward::row_slice_type& forward::row_slice() noexcept {
    return row_slice_;
}

forward::row_slice_type const& forward::row_slice() const noexcept {
    return row_slice_;
}

forward::ordering_type& forward::ordering() noexcept {
    return ordering_;
}

forward::ordering_type const& forward::ordering() const noexcept {
    return ordering_;
}

std::optional<forward::size_type> forward::limit() const {
    return relation::details::get_compatible_row_count(row_slice_);
}

forward& forward::limit(std::optional<size_type> limit) noexcept {
    relation::details::set_compatible_row_count(row_slice_, limit);
    return *this;
}

bool operator==(forward const& a, forward const& b) noexcept {
    if (std::addressof(a) == std::addressof(b)) {
        return true;
    }
    return a.columns() == b.columns()
        && a.row_slice() == b.row_slice()
        && a.ordering() == b.ordering();
}

bool operator!=(forward const& a, forward const& b) noexcept {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& out, forward const& value) {
    return out << value.kind() << "("
               << "columns=" << util::print_support { value.columns() } << ", "
               << "row_slice=" << value.row_slice() << ", "
               << "ordering=" << value.ordering() << ")";
}

bool forward::equals(step const& other) const noexcept {
    return tag == other.kind() && *this == util::unsafe_downcast<forward>(other);
}

std::ostream& forward::print_to(std::ostream& out) const {
    return out << *this;
}

} // namespace takatori::plan
