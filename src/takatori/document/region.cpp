#include <takatori/document/region.h>

namespace takatori::document {

region::region(
        document const& source,
        document::position_type first,
        document::position_type last) noexcept
    : source_(std::addressof(source))
    , first_(source.offset(first))
    , last_(source.offset(last))
{}

std::optional<std::string_view> region::contents() const noexcept {
    if (source_ == nullptr
            || first_ == npos
            || last_ == npos
            || first_ >= last_
            || last_ > source_->size()) {
        return {};
    }
    return source_->contents(first_, last_ - first_);
}

document::position_type region::first_position() const noexcept {
    if (source_ == nullptr || first_ == npos) return {};
    return source_->position(first_);
}

std::optional<document::position_type> region::last_position() const noexcept {
    if (source_ == nullptr || last_ == npos) return {};
    return source_->position(last_);
}

std::ostream& operator<<(std::ostream& out, region const& value) {
    if (auto source = value.source()) {
        out << source->location();
        if (auto first = value.first_position()) {
            out << ":" << first;
            if (auto gap = value.gap()) {
                out << "+" << gap.value();
            }
        }
    } else {
        out << "(unknown)";
    }
    return out;
}

} // namespace takatori::document
