#pragma once

#include <functional>
#include <iostream>
#include <string_view>
#include <utility>

#include "memory_resource.h"
#include "standard_memory_resource.h"

namespace takatori::util {

class forwarding_memory_resource : public pmr::memory_resource {
public:
    enum class tag {
        allocate,
        deallocate,
    };

    explicit forwarding_memory_resource(
            std::function<void(tag, void*, std::size_t)> hook,
            pmr::memory_resource* delegate = get_standard_memory_resource()) noexcept
        : hook_(std::move(hook)), delegate_(delegate) {}

    ~forwarding_memory_resource() override = default;
    forwarding_memory_resource(forwarding_memory_resource const& other) = default;
    forwarding_memory_resource& operator=(forwarding_memory_resource const& other) = default;
    forwarding_memory_resource(forwarding_memory_resource&& other) noexcept = default;
    forwarding_memory_resource& operator=(forwarding_memory_resource&& other) noexcept = default;

protected:
    void* do_allocate(std::size_t bytes, std::size_t alignment) override {
        auto* result = delegate_->allocate(bytes, alignment);;
        if (result == nullptr) {
            return nullptr;
        }
        hook_(tag::allocate, result, bytes);
        return result;
    }

    void do_deallocate(void* p, std::size_t bytes, std::size_t alignment) override {
        hook_(tag::deallocate, p, bytes);
        delegate_->deallocate(p, bytes, alignment);
    }

    bool do_is_equal(const memory_resource& other) const noexcept override {
        if (auto* ptr = dynamic_cast<forwarding_memory_resource const*>(&other); ptr != nullptr) {
            return delegate_->is_equal(*ptr->delegate_);
        }
        return false;
    }

private:
    std::function<void(tag, void*, std::size_t)> hook_;
    pmr::memory_resource* delegate_;
};

constexpr inline std::string_view to_string_view(forwarding_memory_resource::tag value) noexcept {
    using tag = forwarding_memory_resource::tag;
    using namespace std::string_view_literals;
    switch (value) {
        case tag::allocate: return "allocate"sv;
        case tag::deallocate: return "deallocate"sv;
    }
    std::abort();
}

inline std::ostream& operator<<(std::ostream& out, forwarding_memory_resource::tag value) {
    return out << to_string_view(value);
}

} // namespace takatori::util
