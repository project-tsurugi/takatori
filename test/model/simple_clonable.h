#pragma once

#include "util/object_creator.h"

namespace model {

class clonable {
public:
    explicit clonable(int count = 100) noexcept : count_(count) {}
    ~clonable() = default;

    int count() const noexcept { return count_; }

    clonable* clone(util::object_creator creator) const& {
        return creator.create_object<clonable>(count_ * 2);
    }

    clonable* clone(util::object_creator creator) && {
        return creator.create_object<clonable>(count_ + 1);
    }

    void parent_element(int* parent) { parent_ = parent; }
    int* parent() const noexcept { return parent_; }

private:
    int count_;
    int* parent_ {};
};

} // namespace model
