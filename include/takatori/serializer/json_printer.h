#pragma once

#include <memory>
#include <iostream>

#include "object_acceptor.h"

namespace takatori::serializer {

/**
 * @brief an implementation of object_acceptor that print data as JSON format.
 * @details If multiple values are passed to top level input,
 *      this will separate with a LF character.
 * @note This is designed for debugging.
 *      No pretty printing feature is provided, please use other JSON pretty printing tools.
 */
class json_printer final : public object_acceptor {
public:
    /// @brief the size type.
    using size_type = std::size_t;

    /**
     * @brief creates a new instance.
     * @param out the printing target
     */
    explicit json_printer(std::ostream& out);

    void string(std::string_view value) override;
    void integer(std::int64_t value) override;
    void unsigned_integer(std::uint64_t value) override;
    void binary_float(double value) override;
    void number(fpdecimal::Decimal value) override;
    void boolean(bool value) override;
    void pointer(void const* value) override;
    void struct_begin() override;
    void struct_end() override;
    void array_begin() override;
    void array_end() override;
    void property_begin(std::string_view value) override;
    void property_end() override;

    /**
     * @brief sets whether or not place `null` to absent properties.
     * @param enable whether or not the feature is enabled, the default value is `false`
     * @return this
     */
    json_printer& enable_null_if_absent(bool enable) noexcept;

    /**
     * @brief sets whether or not replace pointer() with its occurrence index.
     * @details If this feature is disabled,
     * @param enable whether or not the feature is enabled, the default value is `true`
     * @return this
     */
    json_printer& enable_pointer_index(bool enable) noexcept;

    /**
     * @brief returns the current JSON depth.
     * @note this is mainly designed for testing.
     * @return 0 if this is on top level
     * @return the JSON depth
     */
    [[nodiscard]] size_type depth() const noexcept;

private:
    class impl;
    std::shared_ptr<impl> impl_;
};

} // namespace takatori::serializer
