#pragma once

#include <takatori/relation/intermediate/extension.h>

namespace takatori::relation::intermediate {

class dummy_extension : public extension {
public:

    static constexpr extension_id_type extension_tag = minimum_user_extension_id + 12'345;

    explicit dummy_extension(std::string value) noexcept : value_(std::move(value)) {}

    [[nodiscard]] util::sequence_view<input_port_type> input_ports() noexcept override {
        return {};
    }

    [[nodiscard]] util::sequence_view<input_port_type const> input_ports() const noexcept override {
        return {};
    }

    [[nodiscard]] util::sequence_view<output_port_type> output_ports() noexcept override {
        return {};
    }

    [[nodiscard]] util::sequence_view<output_port_type const> output_ports() const noexcept override {
        return {};
    }

    [[nodiscard]] dummy_extension* clone() const& override {
        return new dummy_extension(value_); // NOLINT
    }

    [[nodiscard]] dummy_extension* clone() && override {
        return new dummy_extension(std::move(value_)); // NOLINT
    }

    [[nodiscard]] extension_id_type extension_id() const noexcept override {
        return extension_tag;
    }

    [[nodiscard]] std::string& value() noexcept {
        return value_;
    }

protected:
    [[nodiscard]] bool equals(extension const& other) const noexcept override {
        return extension_tag == other.extension_id()
                && value_ == util::unsafe_downcast<dummy_extension>(other).value_;
    }
    std::ostream& print_to(std::ostream& out) const override {
        return out << "testing(" << value_ << ")";
    }

private:
    std::string value_;
};

} // takatori::relation::intermediate