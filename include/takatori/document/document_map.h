#pragma once

#include <memory>
#include <unordered_map>
#include <variant>

#include <takatori/document/document.h>
#include <takatori/util/optional_ptr.h>
#include <takatori/util/maybe_shared_ptr.h>

namespace takatori::document {

/**
 * @brief provides documents.
 */
class document_map {
public:
    /**
     * @brief creates a new empty document map.
     */
    document_map() noexcept = default;

    /**
     * @brief creates a new document map which holds the given element.
     * @param entry the first entry
     * @attention This only holds a reference of the document. Please keep the object ownership until dispose this map
     */
    document_map(document const& entry); // NOLINT

    document_map(document&& entry) = delete;
    document_map(document const&& entry) = delete;

    /**
     * @brief creates a new document map which holds the given element.
     * @param entry the first entry
     */
    document_map(std::shared_ptr<document const> entry); // NOLINT

    /**
     * @brief adds a view of document into this map.
     * @details This may overwrites the existing entry if there is document with the same location.
     * @param entry the entry
     * @return this
     * @attention This only holds a reference of the document. Please keep the object ownership until dispose this map
     */
    document_map& add(document const& entry);

    document_map& add(document&& entry) = delete;
    document_map& add(document const&& entry) = delete;

    /**
     * @brief adds a view of document into this map.
     * @details This may overwrites the existing entry if there is document with the same location.
     * @param entry the entry
     * @return this
     */
    document_map& add(std::shared_ptr<document const> entry);

    /**
     * @brief returns a previously added document entry.
     * @param location the document location
     * @return the corresponded document
     * @return empty if there is no such the document
     */
    [[nodiscard]] util::optional_ptr<document const> find(std::string_view location) const;

    /**
     * @brief removes a previously added document entry.
     * @param location the document location
     * @return true if successfully removed
     * @return false if there is no such the document
     */
    bool erase(std::string_view location);

    /**
     * @brief reserves capacity of document entries.
     * @param capacity the number of documents
     */
    void reserve(std::size_t capacity);

    /**
     * @brief removes all document entries.
     */
    void clear();

private:
    std::unordered_map<std::string_view, util::maybe_shared_ptr<document const>> entry_map_;
};

} // namespace takatori::document

