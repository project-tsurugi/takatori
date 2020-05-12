#include <takatori/serializer/json_printer.h>

#include <vector>
#include <unordered_map>

#include <takatori/util/exception.h>

#include "../util/iomanip_util.h"

namespace takatori::serializer {

using ::takatori::util::throw_exception;

namespace {

enum class json_location {
    input,
    struct_,
    array,
    property,
};

class json_state {
public:
    constexpr json_state() = default;

    constexpr json_state(json_location location) noexcept // NOLINT
        : location_(location)
    {}

    constexpr json_state(std::string_view name) noexcept // NOLINT
        : location_(json_location::property)
        , property_name_(name)
    {}

    [[nodiscard]] constexpr json_location location() const noexcept {
        return location_;
    }

    [[nodiscard]] constexpr std::string_view property_name() const noexcept {
        return property_name_;
    }

    [[nodiscard]] constexpr bool empty() const noexcept {
        return entry_count_ == 0;
    }

    constexpr void increment_entry() noexcept {
        ++entry_count_;
    }

private:
    json_location location_ { json_location::input };
    std::string_view property_name_ {};
    std::size_t entry_count_ {};
};

} // namespace

class json_printer::impl {
public:
    explicit impl(std::ostream& out)
        : out_(out)
    {
        state_stack_.emplace_back();
    }

    void string(std::string_view value) {
        do_start_object();
        print_string(value);
        do_end_object();
    }

    template<class T>
    void raw(T value) {
        do_start_object();
        out_ << value;
        do_end_object();
    }

    void boolean(bool value) {
        do_start_object();
        if (value) {
            out_ << "true";
        } else {
            out_ << "false";
        }
        do_end_object();
    }

    void pointer(void const* value) {
        do_start_object();
        if (enable_pointer_table_) {
            auto [it, success] = pointers_.emplace(value, pointers_.size() + 1);
            (void) success;
            out_ << "\"@" << it->second << "\"";
        } else {
            out_ << '"' << value << '"';
        }
        do_end_object();
    }

    void struct_begin() {
        do_start_block(json_location::struct_);
        out_ << '{';
    }

    void struct_end() {
        out_ << '}';
        do_end_block(json_location::struct_);
    }

    void array_begin() {
        do_start_block(json_location::array);
        out_ << '[';
    }

    void array_end() {
        out_ << ']';
        do_end_block(json_location::array);
    }

    void property_begin(std::string_view value) {
        auto&& top = state_stack_.back();
        if (top.location() != json_location::struct_) {
            throw_exception(std::domain_error("property only can appear in structs directly"));
        }
        state_stack_.emplace_back(value);
    }

    void property_end() {
        auto&& top = validate_location(json_location::property);
        bool empty = top.empty();
        if (empty && enable_null_if_absent_) {
            // insert null as a value
            do_start_object();
            out_ << "null";
            empty = false;
        }
        state_stack_.pop_back();
        if (!empty) {
            state_stack_.back().increment_entry();
        }
    }

    void enable_null_if_absent(bool enable) noexcept {
        enable_null_if_absent_ = enable;
    }

    void enable_pointer_index(bool enable) noexcept {
        enable_pointer_table_ = enable;
    }

    [[nodiscard]] std::size_t depth() const noexcept {
        return state_stack_.size() - 1;
    }

private:
    std::ostream& out_;
    std::unordered_map<void const*, std::size_t> pointers_;
    std::vector<json_state> state_stack_;
    bool enable_null_if_absent_ { false };
    bool enable_pointer_table_ { true };

    void separate_if_continue(json_state const& state) {
        if (!state.empty()) {
            switch (state.location()) {
                case json_location::input:
                    out_ << '\n';
                    break;
                case json_location::struct_:
                case json_location::array:
                    out_ << ',';
                    break;
                case json_location::property:
                    throw_exception(std::domain_error("json property can have upto only one value"));
            }
        }
    }

    void do_start_object() {
        auto&& top = state_stack_.back();
        if (top.location() == json_location::struct_) {
            throw_exception(std::domain_error("struct can not include values directly"));
        }
        separate_if_continue(top);

        // actually starts the property when property value was present
        if (top.location() == json_location::property) {
            auto&& container = state_stack_[state_stack_.size() - 2];
            separate_if_continue(container);
            print_string(top.property_name());
            out_ << ':';
        }
    }

    void do_end_object() {
        state_stack_.back().increment_entry();
    }


    json_state& validate_location(json_location location) {
        auto&& top = state_stack_.back();
        if (top.location() != location) {
            throw_exception(std::domain_error("invalid JSON structure"));
        }
        return top;
    }

    void do_start_block(json_location location) {
        do_start_object();
        state_stack_.emplace_back(location);
    }

    void do_end_block(json_location location) {
        validate_location(location);
        state_stack_.pop_back();
        do_end_object();
    }

    void print_string(std::string_view value) {
        out_ << '"';
        for (auto c : value) {
            switch (c) {
                case '\n': out_ << "\\n"; break;
                case '\r': out_ << "\\r"; break;
                case '\t': out_ << "\\t"; break;
                case '\\': out_ << "\\\\"; break;
                case '"': out_ << "\\\""; break;
                default: {
                    if (c < 0x20) {
                        util::instant_fill filler { out_, '0' };
                        out_ << "\\x"
                             << std::hex << std::setw(2) << static_cast<unsigned char>(c)
                             << std::dec;
                    } else {
                        out_ << c; break;
                    }
                    break;
                }
            }
        }
        out_ << '"';
    }
};

json_printer::json_printer(std::ostream& out)
    : impl_(std::allocate_shared<impl>(std::allocator<impl> {}, out))
{}

void json_printer::string(std::string_view value) {
    impl_->string(value);
}

void json_printer::integer(std::int64_t value) {
    impl_->raw(value);
}

void json_printer::unsigned_integer(std::uint64_t  value) {
    impl_->raw(value);
}

void json_printer::binary_float(double value) {
    impl_->raw(value);
}

void json_printer::number(fpdecimal::Decimal value) {
    impl_->raw(value);
}

void json_printer::boolean(bool value) {
    impl_->boolean(value);
}

void json_printer::pointer(void const* value) {
    impl_->pointer(value);
}

void json_printer::struct_begin() {
    impl_->struct_begin();
}

void json_printer::struct_end() {
    impl_->struct_end();
}

void json_printer::array_begin() {
    impl_->array_begin();
}

void json_printer::array_end() {
    impl_->array_end();
}

void json_printer::property_begin(std::string_view value) {
    impl_->property_begin(value);
}

void json_printer::property_end() {
    impl_->property_end();
}

json_printer& json_printer::enable_null_if_absent(bool enable) noexcept {
    impl_->enable_null_if_absent(enable);
    return *this;
}

json_printer& json_printer::enable_pointer_index(bool enable) noexcept {
    impl_->enable_pointer_index(enable);
    return *this;
}

json_printer::size_type json_printer::depth() const noexcept {
    return impl_->depth();
}

} // namespace takatori::serializer
