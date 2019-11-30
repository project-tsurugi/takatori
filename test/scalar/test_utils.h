#pragma once

#include <stdexcept>
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
    T& value() noexcept { return value_; }
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

template<class T, descriptor::descriptor_kind Kind>
inline T const& value_of(descriptor::element<Kind> const& desc) {
    if (auto p = util::dynamic_pointer_cast<value_object<T>>(desc.optional_entity())) {
        return p->value();
    }
    throw std::domain_error("unsupported element");
}

template<class T>
inline typename T::view_type value_of(expression const& expr) {
    if (auto e = util::dynamic_pointer_cast<immediate>(expr)) {
        if (auto v = util::dynamic_pointer_cast<T>(e->value())) {
            return v->get();
        }
        throw std::domain_error("inconsistent value type");
    }
    throw std::domain_error("unsupported expression");
}

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
