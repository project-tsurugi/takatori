#pragma once

#include <string>
#include <string_view>
#include <sstream>

#include <cstddef>
#include <cstdint>

#include <takatori/decimal/triple.h>

namespace takatori::serializer {

/**
 * @brief accepts structured elements in objects.
 * @details Each input must follow the production rule `input` as the mentioned below, which begins from `input`:
 *
 *      ```
 *      input ::= object*
 *
 *      object ::= value
 *              |  struct
 *              |  array
 *
 *      value ::= INTEGER
 *             |  BINARY_FLOAT
 *             |  NUMBER
 *             |  STRING
 *             |  BOOLEAN
 *
 *      struct ::= STRUCT_BEGIN property* STRUCT_END
 *
 *      array ::= ARRAY_BEGIN object* ARRAY_END
 *
 *      property ::= PROPERTY_BEGIN object? PROPERTY_END
 *      ```
 */
class object_acceptor {
public:
    /**
     * @brief creates a new instance.
     */
    object_acceptor() = default;

    /**
     * @brief destroys this instance.
     */
    virtual ~object_acceptor() = default;

    /**
     * @brief accepts a printable value.
     * @details This converts the given value into its string representation, and pass it into string_value().
     * @tparam T the value type
     * @param value the value
     */
    template<class T>
    void value(T const& value) {
        thread_local std::ostringstream buf;
        buf << value;
        string(buf.str());
        buf.str({});
        buf.clear();
    }

    /**
     * @brief accepts a value.
     * @param value the value
     */
    virtual void integer(std::int64_t value) = 0;

    /// @copydoc integer()
    virtual void unsigned_integer(std::uint64_t value) = 0;

    /// @copydoc integer()
    virtual void binary_float(double value) = 0;

    /// @copydoc integer()
    virtual void number(decimal::triple value) = 0;

    /**
     * @copydoc integer()
     * @details The given string will be consumed in this function call.
     */
    virtual void string(std::string_view value) = 0;

    /// @copydoc integer()
    virtual void boolean(bool value) = 0;

    /// @copydoc integer()
    virtual void pointer(void const* value) = 0;

    /**
     * @brief begins a struct.
     */
    virtual void struct_begin() = 0;

    /**
     * @brief ends the current struct.
     */
    virtual void struct_end() = 0;

    /**
     * @brief begins an array.
     */
    virtual void array_begin() = 0;

    /**
     * @brief ends the current array.
     */
    virtual void array_end() = 0;

    /**
     * @brief begins a property of the current struct.
     * @attention the given value must be valid until calling property_end() of this property.
     * @param value the property name
     */
    virtual void property_begin(std::string_view value) = 0;

    /**
     * @brief ends the current property.
     */
    virtual void property_end() = 0;
};

} // namespace takatori::serializer
