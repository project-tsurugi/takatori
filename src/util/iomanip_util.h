#pragma once

#include <iomanip>
#include <iostream>

namespace takatori::util {

class instant_fill {
public:
    instant_fill(std::ostream& out, char fill)
        : out_(out)
        , last_fill_(out.fill())
    {
        out_ << std::setfill(fill);
    }

    ~instant_fill() {
        out_ << std::setfill(last_fill_);
    }

private:
    std::ostream& out_;
    char last_fill_;
};

} // namespace takatori::util

