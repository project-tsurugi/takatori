#pragma once

#include <iostream>
#include <string_view>
#include <utility>

#include "position.h"

namespace takatori::document {

/**
 * @brief represents a source document.
 */
class document {
public:
    /// @brief the invalid offset.
    static inline constexpr std::size_t npos = static_cast<std::size_t>(-1);

    /// @brief the document position type.
    using position_type = class position;

    /**
     * @brief creates a new instance.
     */
    document() = default;

    /**
     * @brief destroys this object.
     */
    virtual ~document() = default;

    /**
     * @brief creates a new instance.
     * @param other the source object
     */
    document(document const& other) = delete;

    /**
     * @brief assigns the given object.
     * @param other the source object
     * @return this
     */
    document& operator=(document const& other) = delete;

    /**
     * @brief creates a new instance.
     * @param other the source object
     */
    document(document&& other) noexcept = default;

    /**
     * @brief assigns the given object.
     * @param other the source object
     * @return this
     */
    document& operator=(document&& other) noexcept = default;

    /**
     * @brief returns the document location.
     * @return the document location
     */
    [[nodiscard]] virtual std::string_view location() const noexcept = 0;

    /**
     * @brief returns the contents size.
     * @details each contents offset must be less than the contents size.
     * @return the contents size
     */
    [[nodiscard]] virtual std::size_t size() const noexcept = 0;

    /**
     * @brief returns contents in the document.
     * @param offset contents offset
     * @param length the contents length
     * @return the contents
     */
    [[nodiscard]] virtual std::string_view contents(std::size_t offset, std::size_t length) const noexcept = 0;

    /**
     * @brief returns the document position of the contents offset.
     * @param offset the contents offset
     * @return the document position
     */
    [[nodiscard]] virtual position_type position(std::size_t offset) const noexcept = 0;

    /**
     * @brief returns the document offset of the content position.
     * @param position the content position in the document
     * @return the corresponded document offset
     * @return npos if the position is not valid for this document
     */
    [[nodiscard]] virtual std::size_t offset(position_type position) const noexcept = 0;

    /**
     * @brief prints information of the given document into the output stream.
     * @param out the target output stream
     * @param value the target document
     * @return the written output stream
     */
    friend std::ostream& operator<<(std::ostream& out, document const& value) {
        return value.print_to(out);
    }

protected:
    /**
     * @brief prints information of this document into the given output stream.
     * @param out the target output stream
     * @return the written output stream
     */
    virtual std::ostream& print_to(std::ostream& out) const {
        return out << location();
    }
};

} // namespace takatori::document
