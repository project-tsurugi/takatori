#pragma once

#include <new>

#include "memory_resource.h"

namespace takatori::util {

/**
 * @brief an implementation of memory_resource which use global new/delete operator..
 * Note that, this cannot handle external array objects managed new[] or delete[].
 */
class standard_memory_resource : public pmr::memory_resource {
protected:
    /**
     * @brief allocates a new buffer.
     * @param bytes the required buffer size
     * @param alignment the alignment size of the head of buffer
     * @return pointer to the allocated buffer
     */
    void* do_allocate(std::size_t bytes, std::size_t alignment) override {
        return ::operator new(bytes, static_cast<std::align_val_t>(alignment));
    }

    /**
     * @brief deallocates the buffer previously allocated by this resource.
     * @param p pointer to the buffer to be deallocated
     */
    void do_deallocate(void* p, std::size_t, std::size_t) override {
        ::operator delete(p);
    }

    /**
     * @brief returns whether or not the given memory resource is equivalent to this.
     * @param other the target memory resource
     * @return true if the both are equivalent
     * @return false otherwise
     */
    bool do_is_equal(memory_resource const& other) const noexcept override {
        return dynamic_cast<standard_memory_resource const*>(&other) != nullptr;
    }
};

/**
 * @brief returns a standard_memory_resource instance.
 * @return a standard_memory_resource instance
 */
inline standard_memory_resource* get_standard_memory_resource() noexcept {
    static standard_memory_resource instance;
    return &instance;
}

} // namespace takatori::util
