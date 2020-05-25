#pragma once

#include <takatori/scalar/immediate.h>
#include <takatori/scalar/variable_reference.h>
#include <takatori/scalar/unary.h>
#include <takatori/scalar/cast.h>
#include <takatori/scalar/binary.h>
#include <takatori/scalar/compare.h>
#include <takatori/scalar/match.h>
#include <takatori/scalar/conditional.h>
#include <takatori/scalar/coalesce.h>
#include <takatori/scalar/let.h>
#include <takatori/scalar/function_call.h>
#include <takatori/scalar/extension.h>

#include <takatori/serializer/object_acceptor.h>
#include <takatori/serializer/object_scanner.h>

namespace takatori::serializer::details {

class scalar_expression_property_scanner {
public:
    explicit scalar_expression_property_scanner(object_scanner const& scanner, object_acceptor& acceptor) noexcept;

    void operator()(scalar::immediate const& element);
    void operator()(scalar::variable_reference const& element);
    void operator()(scalar::unary const& element);
    void operator()(scalar::cast const& element);
    void operator()(scalar::binary const& element);
    void operator()(scalar::compare const& element);
    void operator()(scalar::match const& element);
    void operator()(scalar::conditional const& element);
    void operator()(scalar::coalesce const& element);
    void operator()(scalar::let const& element);
    void operator()(scalar::function_call const& element);
    void operator()(scalar::extension const& element);

private:
    object_scanner const& scanner_;
    object_acceptor& acceptor_;

    template<class T>
    void accept(util::optional_ptr<T const> element);

    template<class T>
    void accept(T const& element);

    template<class T>
    void accept_enum(T const& element);
};

} // namespace takatori::serializer::details
