#pragma once

#include <ostream>
#include <string>
#include <string_view>
#include <vector>

#include "document.h"

namespace takatori::document {

/**
 * @brief represents a source document.
 */
class basic_document : public document {
public:
    /**
     * @brief creates a new instance.
     * @param location the document location
     * @param contents the document contents
     */
    basic_document(std::string location, std::string contents);

    [[nodiscard]] std::string_view location() const noexcept override;
    [[nodiscard]] std::size_t size() const noexcept override;
    [[nodiscard]] std::string_view contents(std::size_t offset, std::size_t length) const noexcept override;
    [[nodiscard]] position_type position(std::size_t offset) const noexcept override;
    [[nodiscard]] std::size_t offset(position_type position) const noexcept override;

private:
    std::string location_;
    std::string contents_;
    std::vector<std::size_t> eof_offsets_;
};

} // namespace takatori::document
