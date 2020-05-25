#include <takatori/document/basic_document.h>

#include <algorithm>

namespace takatori::document {

static inline constexpr char end_of_line = '\n';

basic_document::basic_document(std::string location, std::string contents)
    : location_(std::move(location))
    , contents_(std::move(contents))
{
    std::size_t start = 0;
    for (;;) {
        auto eol_position = contents_.find(end_of_line, start);
        if (eol_position == std::string::npos) {
            break;
        }
        eof_offsets_.emplace_back(eol_position);
        start = eol_position + 1;
    }
}

std::string_view basic_document::location() const noexcept {
    return location_;
}

std::size_t basic_document::size() const noexcept {
    return contents_.size();
}

std::string_view basic_document::contents(std::size_t offset, std::size_t length) const noexcept {
    if (offset >= contents_.size()) {
        return {};
    }
    return static_cast<std::string_view>(contents_).substr(offset, length);
}

document::position_type basic_document::position(std::size_t offset) const noexcept {
    // NOTE: allow offset of EOF position
    if (offset > contents_.size()) {
        return {};
    }
    auto iter = std::lower_bound(eof_offsets_.begin(), eof_offsets_.end(), offset);
    auto index = static_cast<std::size_t>(std::distance(eof_offsets_.begin(), iter));
    if (index == 0) {
        // the first line
        return {
                1,
                offset + 1,
        };
    }
    auto line_offset = eof_offsets_[index - 1] + 1;
    return {
            index + 1,
            offset - line_offset + 1,
    };
}

std::size_t basic_document::offset(document::position_type position) const noexcept {
    auto line = position.line_number();
    if (line == 0 || line - 1 > eof_offsets_.size()) {
        return npos;
    }
    std::size_t offset = line == 1 ? 0 : eof_offsets_[line - 2] + 1;
    if (auto column = position.column_number(); column > 0) {
        offset += column - 1;
    }
    if (offset > contents_.size()) {
        return npos;
    }
    return offset;
}

} // namespace takatori::document
