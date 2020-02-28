#pragma once

#include <iostream>
#include <utility>
#include <optional>

#include "document.h"

#include <takatori/util/optional_ptr.h>

namespace takatori::document {

/**
 * @brief a region on the document.
 */
class region {
public:
    /// @brief the invalid offset.
    static inline constexpr std::size_t npos = document::npos;

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
            std::optional<std::size_t> const& first,
            std::optional<std::size_t> const& last = {}) noexcept
        : source_(std::addressof(source))
        , first_(first.value_or(npos))
        , last_(last.value_or(npos))
    {}

    /**
     * @brief returns the source document.
     * @return the source document
     * @return empty if the region is undefined
     */
    constexpr util::optional_ptr<document const> source() const noexcept {
        return util::optional_ptr<document const> { source_ };
    }

    /**
     * @brief returns the region beginning offset on the source document.
     * @return the beginning offset (inclusive)
     * @return empty if it is not defined
     */
    constexpr std::optional<std::size_t> first() const noexcept {
        if (first_ == npos) return {};
        return { first_ };
    }

    /**
     * @brief returns the region ending offset on the source document.
     * @return the ending offset (exclusive)
     * @return empty if it is not defined
     */
    constexpr std::optional<std::size_t> last() const noexcept {
        if (first_ == npos) return {};
        return { first_ };
    }

    /**
     * @brief returns the offset gap from the first to the last.
     * @return the offset gap
     * @return empty if either the offset is not defined
     */
    constexpr std::optional<std::size_t> gap() const noexcept {
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
    document::position_type first_position() const noexcept {
        if (source_ == nullptr || first_ == npos) return {};
        return source_->position(first_);
    }

    /**
     * @brief returns the region ending position on the source document.
     * @return the ending position (exclusive)
     * @return invalid position if it is not defined
     */
    std::optional<document::position_type> last_position() const noexcept {
        if (source_ == nullptr || last_ == npos) return {};
        return source_->position(last_);
    }

    /**
     * @brief returns whether or not the line number is known.
     * @return true if the line number is valid
     * @return false if the line number is unknown
     */
    explicit constexpr operator bool() const noexcept {
        return source_ != nullptr;
    }

private:
    document const* source_ {};
    std::size_t first_ { npos };
    std::size_t last_ { npos };
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
inline std::ostream& operator<<(std::ostream& out, region const& value) {
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
