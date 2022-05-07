#include <takatori/serializer/value_input_exception.h>

namespace takatori::serializer {


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


} // namespace takatori::serializer
