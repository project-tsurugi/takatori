#pragma once

#include <takatori/relation/find.h>
#include <takatori/relation/scan.h>
#include <takatori/relation/join_find.h>
#include <takatori/relation/join_scan.h>
#include <takatori/relation/project.h>
#include <takatori/relation/filter.h>
#include <takatori/relation/buffer.h>
#include <takatori/relation/emit.h>
#include <takatori/relation/write.h>

#include <takatori/relation/intermediate/join.h>
#include <takatori/relation/intermediate/aggregate.h>
#include <takatori/relation/intermediate/distinct.h>
#include <takatori/relation/intermediate/limit.h>
#include <takatori/relation/intermediate/union.h>
#include <takatori/relation/intermediate/intersection.h>
#include <takatori/relation/intermediate/difference.h>
#include <takatori/relation/intermediate/escape.h>

#include <takatori/relation/step/join.h>
#include <takatori/relation/step/aggregate.h>
#include <takatori/relation/step/intersection.h>
#include <takatori/relation/step/difference.h>
#include <takatori/relation/step/flatten.h>
#include <takatori/relation/step/take_flat.h>
#include <takatori/relation/step/take_group.h>
#include <takatori/relation/step/take_cogroup.h>
#include <takatori/relation/step/offer.h>

#include <takatori/serializer/object_acceptor.h>
#include <takatori/serializer/object_scanner.h>

namespace takatori::serializer::details {

class relation_expression_property_scanner {
public:
    explicit relation_expression_property_scanner(object_scanner const& scanner, object_acceptor& acceptor) noexcept;

    void operator()(relation::find const& element);
    void operator()(relation::scan const& element);
    void operator()(relation::join_find const& element);
    void operator()(relation::join_scan const& element);
    void operator()(relation::project const& element);
    void operator()(relation::filter const& element);
    void operator()(relation::buffer const& element);
    void operator()(relation::emit const& element);
    void operator()(relation::write const& element);
    void operator()(relation::intermediate::join const& element);
    void operator()(relation::intermediate::aggregate const& element);
    void operator()(relation::intermediate::distinct const& element);
    void operator()(relation::intermediate::limit const& element);
    void operator()(relation::intermediate::union_ const& element);
    void operator()(relation::intermediate::intersection const& element);
    void operator()(relation::intermediate::difference const& element);
    void operator()(relation::intermediate::escape const& element);
    void operator()(relation::step::join const& element);
    void operator()(relation::step::aggregate const& element);
    void operator()(relation::step::intersection const& element);
    void operator()(relation::step::difference const& element);
    void operator()(relation::step::flatten const& element);
    void operator()(relation::step::take_flat const& element);
    void operator()(relation::step::take_group const& element);
    void operator()(relation::step::take_cogroup const& element);
    void operator()(relation::step::offer const& element);

private:
    object_scanner const& scanner_;
    object_acceptor& acceptor_;

    template<class T>
    void accept(util::optional_ptr<T const> element);

    template<class T>
    void accept(T const& element);

    template<class Sequence>
    void accept_foreach(Sequence&& sequence);

    void accept(relation::details::mapping_element const& element);

    template<class T>
    void accept(relation::details::search_key_element<T> const& element);

    template<class T, class U>
    void accept(relation::details::range_endpoint<T, U> const& element);

    template<class T>
    void accept(scalar::details::variable_declarator<T> const& element);

    void accept(relation::details::key_pair_element const& element);

    void accept(relation::details::sort_key_element const& element);

    void accept(relation::details::emit_element const& element);

    void accept(relation::details::aggregate_element const& element);

    void accept(relation::details::union_element const& element);

    void accept(relation::details::cogroup_element const& element);
};

} // namespace takatori::serializer::details
