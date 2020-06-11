#pragma once

#include <ostream>
#include <utility>
#include <optional>
#include <string_view>

#include "document.h"

#include <takatori/util/optional_ptr.h>

namespace takatori::document {

/**
 * @brief a region on the document.
 */
class region {
public:
    /// @brief absolute offset in documents.
    using size_type = std::size_t;

    /**
     * @brief creates a new instance, which represents "unknown region".
     */
    constexpr region() = default;

    /**
     * @brief creates a new instance.
     * @param source reference to the source document
     * @param first the beginning offset on the document (inclusive)
     * @param last the ending offset on the document (exclusive)
     */
    constexpr region(
            document const& source,
            std::optional<size_type> const& first,
            std::optional<size_type> const& last = {}) noexcept
        : source_(std::addressof(source))
        , first_(first.value_or(npos))
        , last_(last.value_or(npos))
    {}

    /**
     * @brief creates a new instance.
     * @param source reference to the source document
     * @param first the beginning position on the document (inclusive)
     * @param last the ending position on the document (exclusive)
     */
    region(
            document const& source,
            document::position_type first,
            document::position_type last = {}) noexcept;

    /**
     * @brief returns the source document.
     * @return the source document
     * @return empty if the region is undefined
     */
    [[nodiscard]] constexpr util::optional_ptr<document const> source() const noexcept {
        return util::optional_ptr<document const> { source_ };
    }

    /**
     * @brief return the source contents.
     * @return the source contents
     * @return empty if the region is undefined
     */
    [[nodiscard]] std::optional<std::string_view> contents() const noexcept;

    /**
     * @brief returns the region beginning offset on the source document.
     * @return the beginning offset (inclusive)
     * @return empty if it is not defined
     */
    [[nodiscard]] constexpr std::optional<size_type> first() const noexcept {
        if (first_ == npos) return {};
        return { first_ };
    }

    /**
     * @brief returns the region ending offset on the source document.
     * @return the ending offset (exclusive)
     * @return empty if it is not defined
     */
    [[nodiscard]] constexpr std::optional<size_type> last() const noexcept {
        if (last_ == npos) return {};
        return { last_ };
    }

    /**
     * @brief returns the offset gap from the first to the last.
     * @return the offset gap
     * @return empty if either the offset is not defined
     */
    [[nodiscard]] constexpr std::optional<size_type> gap() const noexcept {
        if (first_ == npos || last_ == npos || first_ > last_) {
            return {};
        }
        return { last_ - first_ };
    }

    /**
     * @brief returns the region beginning position on the source document.
     * @return the beginning position (inclusive)
     * @return invalid position if it is not defined
     */
    [[nodiscard]] document::position_type first_position() const noexcept;

    /**
     * @brief returns the region ending position on the source document.
     * @return the ending position (exclusive)
     * @return invalid position if it is not defined
     */
    [[nodiscard]] std::optional<document::position_type> last_position() const noexcept;

    /**
     * @brief returns whether or not the line number is known.
     * @return true if the line number is valid
     * @return false if the line number is unknown
     */
    [[nodiscard]] explicit constexpr operator bool() const noexcept {
        return source_ != nullptr;
    }

private:
    document const* source_ {};
    size_type first_ { npos };
    size_type last_ { npos };

    static inline constexpr size_type npos = document::npos;
};

/**
 * @brief returns whether or not the two regions are equivalent.
 * @param a the first region
 * @param b the second region
 * @return
 */
inline constexpr bool operator==(region const& a, region const& b) noexcept {
    return a.source().get() == b.source().get()
        && a.first() == b.first()
        && a.last() == b.last();
}

/**
 * @brief returns whether or not the two regions are different.
 * @param a the first region
 * @param b the second region
 * @return
 */
inline constexpr bool operator!=(region const& a, region const& b) noexcept {
    return !(a == b);
}

/**
 * @brief prints information of the given document into the output stream.
 * @param out the target output stream
 * @param value the target document
 * @return the written output stream
 */
std::ostream& operator<<(std::ostream& out, region const& value);

} // namespace takatori::document
