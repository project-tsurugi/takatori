#pragma once


namespace takatori::tree {

class clonable {
public:
    explicit clonable(int count = 100) noexcept : count_(count) {}
    ~clonable() = default;

    int count() const noexcept { return count_; }

    clonable* clone() const& {
         return new clonable(count_ * 2); // NOLINT
    }

    clonable* clone() && {
         return new clonable(count_ + 1); // NOLINT
    }

    void parent_element(int* parent) { parent_ = parent; }
    int* parent() const noexcept { return parent_; }

private:
    int count_;
    int* parent_ {};
};

} // namespace takatori::util
