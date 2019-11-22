#pragma once

#include <unordered_map>

#include "takatori/type/data.h"
#include "takatori/type/int.h"

#include "takatori/value/data.h"
#include "takatori/value/int.h"

#include "takatori/descriptor/variable.h"
#include "takatori/descriptor/function.h"

#include "takatori/scalar/immediate.h"
#include "takatori/scalar/variable_reference.h"

namespace takatori::scalar {

template<class T>
class value_object : public util::object {
public:
    explicit value_object(T value) noexcept : value_(std::move(value)) {}
    T const& value() const noexcept { return value_; }

protected:
    bool equals(object const& other) const noexcept override {
        if (auto* ptr = util::downcast<value_object>(&other)) {
            return value_ == ptr->value_;
        }
        return false;
    }
    std::size_t hash() const noexcept override {
        return std::hash<T>{}(value_);
    }
    std::ostream& print_to(std::ostream& out) const override {
        return out << value_;
    }
private:
    T value_;
};

inline descriptor::variable vardesc(int v) {
    return descriptor::variable { std::make_shared<value_object<int>>(v) };
}

inline descriptor::function funcdesc(int v) {
    return descriptor::function { std::make_shared<value_object<int>>(v) };
}

inline immediate constant(int v, type::data&& type = type::int4()) {
    return immediate { value::int4(v), std::move(type) };
}

inline variable_reference varref(int v) {
    return variable_reference { vardesc(v) };
}

} // namespace takatori::scalar
