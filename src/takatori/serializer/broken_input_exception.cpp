#include <takatori/serializer/broken_input_exception.h>

namespace takatori::serializer {


broken_input_exception::broken_input_exception(broken_input_exception::reason_code reason, char const* message) :
        std::runtime_error { message },
        reason_ { reason }
{}

broken_input_exception::broken_input_exception(broken_input_exception::reason_code reason, std::string const& message) :
        std::runtime_error { message },
        reason_ { reason }
{}

broken_input_exception::reason_code broken_input_exception::reason() const noexcept {
    return reason_;
}

std::ostream& operator<<(std::ostream& out, broken_input_exception::reason_code value) {
    return out << to_string_view(value);
}


} // namespace takatori::serializer
