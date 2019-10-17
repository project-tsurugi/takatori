#pragma once

#include <iostream>
#include <vector>

namespace util {

template<class T>
class print_support {
public:
    explicit print_support(T const& element) noexcept : element_(element) {}
    T const& element() const noexcept { return element_; }

private:
    T const& element_;
};

template<class E, class A>
inline std::ostream& operator<<(std::ostream& out, print_support<std::vector<E, A>> const& support) {
    auto& element = support.element();
    out << "{";
    bool cont = false;
    for (auto&& e : element) {
        if (cont) out << ", ";
        cont = true;
        out << e;
    }
    out << "}";
    return out;
}

} // namespace util
