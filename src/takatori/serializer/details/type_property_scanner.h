#pragma once

#include <takatori/type/primitive.h>
#include <takatori/type/decimal.h>
#include <takatori/type/character.h>
#include <takatori/type/octet.h>
#include <takatori/type/bit.h>
#include <takatori/type/date.h>
#include <takatori/type/time_of_day.h>
#include <takatori/type/time_point.h>
#include <takatori/type/datetime_interval.h>
#include <takatori/type/row_id.h>
#include <takatori/type/declared.h>
#include <takatori/type/extension.h>

#include <takatori/serializer/object_acceptor.h>
#include <takatori/serializer/object_scanner.h>

namespace takatori::serializer::details {

class type_property_scanner {
public:
    explicit type_property_scanner(object_scanner const& scanner, object_acceptor& acceptor) noexcept;

    void operator()(type::boolean const& element);
    void operator()(type::int1 const& element);
    void operator()(type::int2 const& element);
    void operator()(type::int4 const& element);
    void operator()(type::int8 const& element);
    void operator()(type::float4 const& element);
    void operator()(type::float8 const& element);
    void operator()(type::decimal const& element);
    void operator()(type::character const& element);
    void operator()(type::octet const& element);
    void operator()(type::bit const& element);
    void operator()(type::date const& element);
    void operator()(type::time_of_day const& element);
    void operator()(type::time_point const& element);
    void operator()(type::datetime_interval const& element);
    void operator()(type::unknown const& element);
    void operator()(type::row_id const& element);
    void operator()(type::declared const& element);
    void operator()(type::extension const& element);

private:
    object_scanner const& scanner_;
    object_acceptor& acceptor_;

    void scan(datetime::time_zone const& tz);
};

} // namespace takatori::serializer::details
