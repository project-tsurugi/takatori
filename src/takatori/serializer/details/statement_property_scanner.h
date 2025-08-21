#pragma once

#include <takatori/statement/execute.h>
#include <takatori/statement/write.h>
#include <takatori/statement/create_table.h>
#include <takatori/statement/drop_table.h>
#include <takatori/statement/create_index.h>
#include <takatori/statement/drop_index.h>
#include <takatori/statement/grant_table.h>
#include <takatori/statement/revoke_table.h>
#include <takatori/statement/empty.h>
#include <takatori/statement/extension.h>

#include <takatori/serializer/object_acceptor.h>
#include <takatori/serializer/object_scanner.h>

namespace takatori::serializer::details {

class statement_property_scanner {
public:
    explicit statement_property_scanner(object_scanner const& scanner, object_acceptor& acceptor) noexcept;

    void operator()(statement::execute const& element);
    void operator()(statement::write const& element);
    void operator()(statement::create_table const& element);
    void operator()(statement::drop_table const& element);
    void operator()(statement::create_index const& element);
    void operator()(statement::drop_index const& element);
    void operator()(statement::grant_table const& element);
    void operator()(statement::revoke_table const& element);
    void operator()(statement::empty const& element);
    void operator()(statement::extension const& element);

private:
    object_scanner const& scanner_;
    object_acceptor& acceptor_;

    template<class T>
    void accept(std::optional<T> const& element);

    template<class T>
    void accept(util::optional_ptr<T const> element);

    template<class T>
    void accept(T const& element);

    template<class Sequence>
    void accept_foreach(Sequence&& sequence);

    void accept(statement::details::write_tuple const& element);

    void accept(statement::details::table_privilege_element const& element);

    void accept(statement::details::table_authorization_entry const& element);

    void accept(statement::details::table_privilege_action const& element);
};

} // namespace takatori::serializer::details
