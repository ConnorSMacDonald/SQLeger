#ifndef SQLEGER_TRANSACTION_HPP
#define SQLEGER_TRANSACTION_HPP

#include <sqleger/db_decl.hpp>
#include <sqleger/stmt.hpp>


namespace sqleger {


class transaction {

public:
  inline transaction(db_ref connection);

  transaction(transaction&& other) noexcept = default;

  transaction& operator=(transaction&& other) noexcept = default;

  inline ~transaction() noexcept;

  inline void commit(bool enable = true) noexcept;

private:
  stmt commit_stmt_;
  stmt rollback_stmt_;

  bool commit_ = false;
};


// =============================================================================


transaction::transaction(const db_ref connection) :
  commit_stmt_ {connection, "COMMIT TRANSACTION"},
  rollback_stmt_ {connection, "ROLLBACK TRANSACTION"}
{
  auto begin_stmt = stmt(connection, "BEGIN TRANSACTION");

  if (const auto r = begin_stmt.step(); r != result_t::done)
    throw result_exception(r);
}

transaction::~transaction() noexcept
{
  if (commit_)
    commit_stmt_.step();
  else
    rollback_stmt_.step();
}

void transaction::commit(const bool enable) noexcept
{
  commit_ = enable;
}


}; // namespace sqleger


#endif
