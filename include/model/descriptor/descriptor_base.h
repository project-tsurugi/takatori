#pragma once

#include <memory>
#include <utility>

namespace model::descriptor {

/**
 * @brief a
 */
class descriptor_base {
public:
    /**
     * @brief creates a new instance.
     * @param entity the entity
     */
    explicit descriptor_base(std::shared_ptr<void*> entity) noexcept : entity_(std::move(entity)) {}

    /**
     * @brief returns the descriptor entity.
     * @return the descriptor entity
     */
    std::shared_ptr<void*> const& entity() const noexcept { return entity_; }

private:
    std::shared_ptr<void*> entity_;
};

} // namespace model::descriptor
