#pragma once

#include <functional>
#include <iostream>
#include <type_traits>
#include <utility>

#include "object_creator.h"
#include "optional_ptr.h"

namespace takatori::util {

/**
 * @brief reference of properties.
 * @details THis is designed for modifying polymorphic class objects in the same way.
 * @tparam T the property type
 */
template<class T>
class ownership_reference {
public:
    /// @brief the property value type.
    using element_type = T;

    /// @brief the property reference type.
    using reference = std::add_lvalue_reference_t<element_type>;

    /// @brief the property pointer type.
    using pointer = std::add_pointer_t<element_type>;

    /// @brief the property std::unique_ptr type.
    using unique_pointer = unique_object_ptr<element_type>;

    /**
     * @brief creates a new instance.
     * @param getter the property getter
     * @param setter the property setter
     */
    ownership_reference(
            std::function<pointer()> getter,
            std::function<void(unique_pointer value)> setter) noexcept
        : getter_(std::move(getter))
        , setter_(std::move(setter))
    {}

    /**
     * @brief creates a new instance from the target property.
     * @param property reference to the target property
     */
    ownership_reference(unique_pointer& property) noexcept // NOLINT
        : getter_([&]() {
            return property.get();
        })
        , setter_([&](unique_pointer replacement) {
            property = std::move(replacement);
        })
    {}

    /**
     * @brief returns whether or not the the target property is absent.
     * @return true if the property is absent
     * @return false otherwise
     */
    bool empty() const {
        return find().empty();
    }

    /**
     * @brief returns whether or not the target property is present.
     * @return true if the property is present
     * @return false otherwise
     */
    bool has_value() const {
        return find().has_value();
    }

    /// @copydoc has_value()
    explicit operator bool() const {
        return has_value();
    }

    /**
     * @brief returns the property value.
     * @return the property value
     * @return empty if it is absent
     */
    optional_ptr<element_type> find() const {
        return optional_ptr<element_type> { getter_() };
    }

    /**
     * @brief returns the property value.
     * @return the property value
     * @throws std::bad_optional_access if this reference is empty
     */
    reference get() const {
        return find().value();
    }

    /// @copydoc get()
    reference value() const {
        return get();
    }

    /// @copydoc get()
    reference operator*() const {
        return get();
    }

    /**
     * @brief sets a new value into this property.
     * @param value the value
     */
    void set(unique_pointer value) {
        setter_(std::move(value));
    }

    /**
     * @copydoc set()
     * @return this
     */
    ownership_reference& operator=(unique_pointer value) {
        set(std::move(value));
        return *this;
    }

    /**
     * @brief appends string representation of the given value.
     * @param out the output stream
     * @param value the target value
     * @return the output stream
     */
    friend std::ostream& operator<<(std::ostream& out, ownership_reference const& value) {
        return out << value.find();
    }

private:
    std::function<pointer()> getter_;
    std::function<void(unique_pointer value)> setter_;
};


} // namespace takatori::util
