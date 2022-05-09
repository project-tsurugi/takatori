#include <takatori/serializer/value_input_exception.h>

#include <takatori/util/exception.h>
#include <takatori/util/string_builder.h>

namespace takatori::serializer {

using ::takatori::util::string_builder;
using ::takatori::util::throw_exception;

value_input_exception::value_input_exception(value_input_exception::reason_code reason, char const* message) :
        std::runtime_error { message },
        reason_ { reason }
{}

value_input_exception::value_input_exception(value_input_exception::reason_code reason, std::string const& message) :
        std::runtime_error { message },
        reason_ { reason }
{}

value_input_exception::reason_code value_input_exception::reason() const noexcept {
    return reason_;
}

std::ostream& operator<<(std::ostream& out, value_input_exception::reason_code value) {
    return out << to_string_view(value);
}


void throw_buffer_underflow() {
    throw_exception(value_input_exception {
            value_input_exception::reason_code::buffer_underflow,
            "input buffer is underflow",
    });
}

void throw_unrecognized_entry(std::uint32_t header) {
    throw_exception(value_input_exception {
            value_input_exception::reason_code::unrecognized_entry_type,
            string_builder {}
                    << "unrecognized entry type: " << header
                    << string_builder::to_string,
    });
}

void throw_unsupported_entry(std::uint32_t header) {
    throw_exception(value_input_exception {
            value_input_exception::reason_code::unrecognized_entry_type,
            string_builder {}
                    << "unsupported entry type: " << header
                    << string_builder::to_string,
    });
}

void throw_size_out_of_range(std::uint64_t size, std::uint64_t limit) {
    throw_exception(value_input_exception {
            value_input_exception::reason_code::unrecognized_entry_type,
            string_builder {}
                    << "too large size: " << size << ", "
                    << "must be less than" << limit
                    << string_builder::to_string,
    });
}

} // namespace takatori::serializer
