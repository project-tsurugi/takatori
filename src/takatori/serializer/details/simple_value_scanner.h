#pragma once

#include <takatori/value/primitive.h>
#include <takatori/value/decimal.h>
#include <takatori/value/character.h>
#include <takatori/value/octet.h>
#include <takatori/value/bit.h>
#include <takatori/value/date.h>
#include <takatori/value/time_of_day.h>
#include <takatori/value/time_point.h>
#include <takatori/value/datetime_interval.h>
#include <takatori/value/extension.h>

#include <takatori/serializer/object_acceptor.h>

namespace takatori::serializer::details {

class simple_value_scanner {
public:
    explicit simple_value_scanner(object_acceptor& acceptor) noexcept;
    void operator()(value::unknown const& element);
    void operator()(value::boolean const& element);
    void operator()(value::int4 const& element);
    void operator()(value::int8 const& element);
    void operator()(value::float4 const& element);
    void operator()(value::float8 const& element);
    void operator()(value::decimal const& element);
    void operator()(value::character const& element);
    void operator()(value::octet const& element);
    void operator()(value::bit const& element);
    void operator()(value::date const& element);
    void operator()(value::time_of_day const& element);
    void operator()(value::time_point const& element);
    void operator()(value::datetime_interval const& element);
    void operator()(value::extension const& element);

private:
    object_acceptor& acceptor_;
};

} // namespace takatori::serializer::details
