#include <takatori/value/extension.h>

#include <takatori/util/downcast.h>
#include <takatori/util/hash.h>

namespace takatori::value {

class dummy_extension : public extension {
public:
    static constexpr extension_id_type extension_tag = minimum_user_extension_id + 12'345;

    explicit dummy_extension(std::string value) noexcept : value_(std::move(value)) {}

    [[nodiscard]] dummy_extension* clone(util::object_creator creator) const& override {
        return creator.create_object<dummy_extension>(value_);
    }
    [[nodiscard]] dummy_extension* clone(util::object_creator creator) && override {
        return creator.create_object<dummy_extension>(std::move(value_));
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
    [[nodiscard]] std::size_t hash() const noexcept override {
        return util::hash(extension_tag, value_);
    }
    std::ostream& print_to(std::ostream& out) const override {
        return out << "testing(" << value_ << ")";
    }

private:
    std::string value_;
};

} // takatori::value
