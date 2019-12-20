#include <takatori/util/object_creator.h>

namespace takatori::util {

object_creator standard_object_creator() noexcept {
    return object_creator(get_standard_memory_resource());
}

bool operator==(object_creator a, object_creator b) noexcept {
    return a.resource_->is_equal(*b.resource_);
}

bool operator==(object_deleter a, object_deleter b) noexcept {
    return a.creator_ == b.creator_;
}

} // namespace takatori::util
