#ifndef SQLEGER_TRANSACTION_HPP
#define SQLEGER_TRANSACTION_HPP

#include <sqleger/db_decl.hpp>
#include <sqleger/stmt.hpp>


namespace sqleger {


enum class transaction_behavior {
  deferred,
  immediate,
  exclusive,
};


class transaction {

public:
  inline transaction(db_ref db_handle);

  inline transaction(db_ref db_handle, transaction_behavior behavior);

  transaction(transaction&& other) noexcept = default;

  transaction& operator=(transaction&& other) noexcept = default;

  inline ~transaction() noexcept;

  inline void commit() noexcept;

  inline void rollback() noexcept;

  inline result commit_now() noexcept;

  inline result rollback_now() noexcept;

  db_ref db_handle() const noexcept { return db_; }

private:
  enum class outcome { commit, rollback };

  inline result do_commit() noexcept;

  inline result do_rollback() noexcept;

  db_ref db_;
  outcome outcome_ = outcome::rollback;
};


// =============================================================================


transaction::transaction(db_ref const db_handle) : db_ {db_handle}
{
  using namespace string_span_literals;

  auto begin_stmt = stmt(db_handle, "BEGIN"_ss);

  if (auto const r = begin_stmt.step(); r != result::done)
    throw result_exception(r);
}

transaction::transaction(db_ref const db_handle,
                         transaction_behavior const behavior) :
  db_ {db_handle}
{
  using namespace string_span_literals;

  auto const behavior_string = [&]() {
    switch (behavior)
    {
      case transaction_behavior::deferred:
        return "BEGIN DEFERRED"_ss;
      case transaction_behavior::exclusive:
        return "BEGIN EXCLUSIVE"_ss;
      case transaction_behavior::immediate:
        return "BEGIN IMMEDIATE"_ss;
    }

    return ""_ss;
  }();

  auto begin_stmt = stmt(db_, behavior_string);

  if (auto const r = begin_stmt.step(); is_error(r))
    throw result_exception(r);
}

transaction::~transaction() noexcept
{
  if (!db_)
    return;

  switch (outcome_)
  {
    case outcome::commit:
      do_commit();
      return;
    case outcome::rollback:
      do_rollback();
      return;
  }
}

void transaction::commit() noexcept
{
  outcome_ = outcome::commit;
}

void transaction::rollback() noexcept
{
  outcome_ = outcome::rollback;
}

result transaction::commit_now() noexcept
{
  auto const r = do_commit();
  db_ = db_ref();
  return r;
}

result transaction::rollback_now() noexcept
{
  auto const r = do_rollback();
  db_ = db_ref();
  return r;
}

result transaction::do_commit() noexcept
{
  using namespace string_span_literals;

  stmt s;

  if (auto const r = db_.prepare_v2("COMMIT"_ss, s); is_error(r))
    return r;

  return s.step();
}

result transaction::do_rollback() noexcept
{
  using namespace string_span_literals;

  stmt s;

  if (auto const r = db_.prepare_v2("ROLLBACK"_ss, s); is_error(r))
    return r;

  return s.step();
}


} // namespace sqleger


#endif
