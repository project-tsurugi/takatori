#pragma once

#include <functional>
#include <ostream>
#include <string_view>
#include <utility>

#include "memory_resource.h"
#include "standard_memory_resource.h"

namespace takatori::util {

/**
 * @brief an implementation of memory_resource which just forward its operation,
 * and can additionally manipulate allocated or deallocating buffers.
 */
class forwarding_memory_resource : public pmr::memory_resource {
public:
    /**
     * @brief an operation tag.
     */
    enum class tag {
        /// @brief operation for allocated buffer.
        allocate,
        /// @brief operation for deallocating buffer.
        deallocate,
    };

    /**
     * @brief creates a new object.
     * @param hook hook for buffers
     * @param delegate the operation forwarding target
     */
    explicit forwarding_memory_resource(
            std::function<void(tag, void*, std::size_t)> hook,
            pmr::memory_resource* delegate = get_standard_memory_resource()) noexcept
        : hook_(std::move(hook)), delegate_(delegate)
    {}

protected:
    /**
     * @brief allocates a new buffer.
     * @param bytes the required buffer size in bytes
     * @param alignment the alignment size of the head of buffer
     * @return pointer to the allocated buffer
     */
    [[nodiscard]] void* do_allocate(std::size_t bytes, std::size_t alignment) override {
        auto* result = delegate_->allocate(bytes, alignment);;
        if (result == nullptr) {
            return nullptr;
        }
        hook_(tag::allocate, result, bytes);
        return result;
    }

    /**
     * @brief deallocates the buffer previously allocated by this resource.
     * @param p pointer to the buffer to be deallocated
     * @param bytes the buffer size in bytes
     * @param alignment the alignment size of the head of buffer
     */
    void do_deallocate(void* p, std::size_t bytes, std::size_t alignment) override {
        hook_(tag::deallocate, p, bytes);
        delegate_->deallocate(p, bytes, alignment);
    }

    /**
     * @brief returns whether or not the given memory resource is equivalent to this.
     * @param other the target memory resource
     * @return true if the both are equivalent
     * @return false otherwise
     */
    [[nodiscard]] bool do_is_equal(const memory_resource& other) const noexcept override {
        if (auto* ptr = dynamic_cast<forwarding_memory_resource const*>(&other); ptr != nullptr) {
            return delegate_->is_equal(*ptr->delegate_);
        }
        return false;
    }

private:
    std::function<void(tag, void*, std::size_t)> hook_;
    pmr::memory_resource* delegate_;
};

/**
 * @brief returns string representation of the value.
 * @param value the target value
 * @return the corresponded string representation
 */
constexpr inline std::string_view to_string_view(forwarding_memory_resource::tag value) noexcept {
    using tag = forwarding_memory_resource::tag;
    using namespace std::string_view_literals;
    switch (value) {
        case tag::allocate: return "allocate"sv;
        case tag::deallocate: return "deallocate"sv;
    }
    std::abort();
}

/**
 * @brief appends string representation of the given value.
 * @param out the target output
 * @param value the target value
 * @return the output
 */
inline std::ostream& operator<<(std::ostream& out, forwarding_memory_resource::tag value) {
    return out << to_string_view(value);
}

} // namespace takatori::util
