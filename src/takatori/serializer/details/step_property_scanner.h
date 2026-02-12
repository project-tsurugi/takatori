#pragma once

#include <takatori/plan/process.h>
#include <takatori/plan/forward.h>
#include <takatori/plan/group.h>
#include <takatori/plan/aggregate.h>
#include <takatori/plan/broadcast.h>
#include <takatori/plan/discard.h>

#include <takatori/serializer/object_acceptor.h>
#include <takatori/serializer/object_scanner.h>

namespace takatori::serializer::details {

class step_property_scanner {
public:
    explicit step_property_scanner(object_scanner const& scanner, object_acceptor& acceptor) noexcept;

    void operator()(plan::process const& element);
    void operator()(plan::forward const& element);
    void operator()(plan::group const& element);
    void operator()(plan::aggregate const& element);
    void operator()(plan::broadcast const& element);
    void operator()(plan::discard const& element);

private:
    object_scanner const& scanner_;
    object_acceptor& acceptor_;

    template<class T>
    void accept(T const& element);

    template<class Sequence>
    void accept_foreach(Sequence&& sequence);

    void accept(relation::details::sort_key_element const& element);

    void accept(relation::details::aggregate_element const& element);

    void accept(relation::details::row_slice const& element);

    void accept(relation::details::constant_value<std::size_t> const& element);
};

} // namespace takatori::serializer::details
