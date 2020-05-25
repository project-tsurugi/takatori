#pragma once

#include <takatori/statement/execute.h>
#include <takatori/statement/write.h>
#include <takatori/statement/extension.h>

#include <takatori/serializer/object_acceptor.h>
#include <takatori/serializer/object_scanner.h>

namespace takatori::serializer::details {

class statement_property_scanner {
public:
    explicit statement_property_scanner(object_scanner const& scanner, object_acceptor& acceptor) noexcept;

    void operator()(statement::execute const& element);
    void operator()(statement::write const& element);
    void operator()(statement::extension const& element);

private:
    object_scanner const& scanner_;
    object_acceptor& acceptor_;

    template<class T>
    void accept(util::optional_ptr<T const> element);

    template<class T>
    void accept(T const& element);

    template<class Sequence>
    void accept_foreach(Sequence&& sequence);

    void accept(statement::details::write_tuple const& element);
};

} // namespace takatori::serializer::details
