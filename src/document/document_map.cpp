#include <takatori/document/document_map.h>

namespace takatori::document {

document_map::document_map(document const& entry)
    : document_map()
{
    add(entry);
}

document_map::document_map(std::shared_ptr<document const> entry)
    : document_map()
{
    add(std::move(entry));
}

document_map& document_map::add(document const& entry) {
    entry_map_.insert_or_assign(entry.location(), std::addressof(entry));
    return *this;
}

document_map& document_map::add(std::shared_ptr<document const> entry) {
    entry_map_.insert_or_assign(entry->location(), std::move(entry));
    return *this;
}

util::optional_ptr<document const> document_map::find(std::string_view location) const {
    if (auto it = entry_map_.find(location); it != entry_map_.end()) {
        return util::optional_ptr { it->second.get() };
    }
    return {};
}

bool document_map::erase(std::string_view location) {
    return entry_map_.erase(location) > 0;
}

void document_map::reserve(std::size_t capacity) {
    entry_map_.reserve(capacity);
}

void document_map::clear() {
    entry_map_.clear();
}

} // namespace takatori::document
