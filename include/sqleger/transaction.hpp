#ifndef SQLEGER_TRANSACTION_HPP
#define SQLEGER_TRANSACTION_HPP

#include <sqleger/db_decl.hpp>
#include <sqleger/stmt.hpp>


namespace sqleger {


enum class transaction_t {
  deferred,
  immediate,
  exclusive,
};


class transaction {

public:
  inline transaction(db_ref db_handle);

  inline transaction(db_ref db_handle, transaction_t type);

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


transaction::transaction(const db_ref db_handle) : db_ {db_handle}
{
  using namespace string_span_literals;

  auto begin_stmt = stmt(db_handle, "BEGIN TRANSACTION"_ss);

  if (const auto r = begin_stmt.step(); r != result::done)
    throw result_exception(r);
}

transaction::transaction(const db_ref db_handle, const transaction_t type) :
  db_ {db_handle}
{
  using namespace std::string_literals;

  const auto type_string = [&]() {
    switch (type)
    {
      case transaction_t::deferred:
        return "DEFERRED"s;
      case transaction_t::exclusive:
        return "EXCLUSIVE"s;
      case transaction_t::immediate:
        return "IMMEDIATE"s;
    }

    return ""s;
  }();

  auto begin_stmt = stmt(db_, "BEGIN "s + type_string + " TRANSACTION"s);

  if (const auto r = begin_stmt.step(); is_error(r))
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
  const auto r = do_commit();
  db_ = db_ref();
  return r;
}

result transaction::rollback_now() noexcept
{
  const auto r = do_rollback();
  db_ = db_ref();
  return r;
}

result transaction::do_commit() noexcept
{
  using namespace string_span_literals;

  stmt s;

  if (const auto r = db_.prepare_v2("COMMIT TRANSACTION"_ss, s); is_error(r))
    return r;

  return s.step();
}

result transaction::do_rollback() noexcept
{
  using namespace string_span_literals;

  stmt s;

  if (const auto r = db_.prepare_v2("ROLLBACK TRANSACTION"_ss, s); is_error(r))
    return r;

  return s.step();
}


}; // namespace sqleger


#endif
