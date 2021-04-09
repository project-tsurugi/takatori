#pragma once

#include <stdexcept>
#include <utility>

#include <takatori/descriptor/binding_info.h>
#include <takatori/util/downcast.h>

namespace takatori::testing {

template<descriptor::descriptor_kind Kind, class T>
class mock_binding_info : public descriptor::binding_info<Kind> {
public:
    explicit mock_binding_info(T value) noexcept : value_ { std::move(value) } {}

    [[nodiscard]] std::size_t class_id() const noexcept override {
        return static_cast<std::size_t>(Kind);
    }

    [[nodiscard]] T& value() noexcept {
        return value_;
    }

    [[nodiscard]] T const& value() const noexcept {
        return value_;
    }

    [[nodiscard]] static T& extract(descriptor::binding_info<Kind>& object) {
        return util::downcast<mock_binding_info>(object).value();
    }

    [[nodiscard]] static T const& extract(descriptor::binding_info<Kind> const& object) {
        return util::downcast<mock_binding_info>(object).value();
    }

protected:
    [[nodiscard]] bool equals(descriptor::binding_info<Kind> const& other) const noexcept override {
        if (auto* ptr = util::downcast<mock_binding_info>(&other)) {
            return value_ == ptr->value_;
        }
        return false;
    }

    [[nodiscard]] std::size_t hash() const noexcept override {
        return std::hash<T>{}(value_);
    }

    std::ostream& print_to(std::ostream& out) const override {
        return out << value_;
    }

private:
    T value_;
};

} // namespace takatori::testing
