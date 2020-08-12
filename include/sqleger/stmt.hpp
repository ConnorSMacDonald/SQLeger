#ifndef SQLEGER_STMT_HPP
#define SQLEGER_STMT_HPP

#include <sqleger/db_decl.hpp>
#include <sqleger/result_exception.hpp>

#include <sqlite3.h>


namespace sqleger {


template <typename Impl>
class stmt_interface {

public:
  using impl_type = Impl;
  using c_type = ::sqlite3_stmt;

  zstring_view sql() const noexcept;

private:
  c_type* c_ptr() const noexcept;
};


class stmt : public stmt_interface<stmt> {

public:
  using interface_type = stmt_interface<stmt>;

  constexpr stmt() noexcept = default;

  template <typename DbImpl>
  stmt(db_interface<DbImpl>& connection, const string_span& sql);

  explicit constexpr stmt(c_type* c_ptr) noexcept;

  constexpr stmt(stmt&& other) noexcept;

  stmt& operator=(stmt&& other) noexcept;

  inline ~stmt() noexcept;

  inline result_t finalize() noexcept;

  constexpr c_type* take_c_ptr() noexcept;

  constexpr c_type** c_ptr_out() noexcept { return &c_ptr_; }

  constexpr c_type* c_ptr() const noexcept { return c_ptr_; }

private:
  inline result_t do_finalize() noexcept;

  c_type* c_ptr_ = nullptr;
};


// =============================================================================


template <typename Impl>
zstring_view stmt_interface<Impl>::sql() const noexcept
{
  return ::sqlite3_sql(c_ptr());
}

template <typename Impl>
auto stmt_interface<Impl>::c_ptr() const noexcept -> c_type*
{
  return static_cast<const impl_type*>(this)->c_ptr();
}

template <typename DbImpl>
stmt::stmt(db_interface<DbImpl>& connection, const string_span& sql)
{
  if (const auto r = connection.prepare_v2(sql, *this); is_error(r))
    throw result_exception(r);
}

constexpr stmt::stmt(c_type* const c_ptr) noexcept : c_ptr_ {c_ptr} {}

constexpr stmt::stmt(stmt&& other) noexcept : c_ptr_ {other.take_c_ptr()} {}

stmt& stmt::operator=(stmt&& other) noexcept
{
  if (this == &other)
    return *this;

  do_finalize();

  c_ptr_ = other.take_c_ptr();

  return *this;
}

stmt::~stmt() noexcept
{
  do_finalize();
}

result_t stmt::finalize() noexcept
{
  const auto r = do_finalize();

  c_ptr_ = nullptr;

  return r;
}

constexpr auto stmt::take_c_ptr() noexcept -> c_type*
{
  auto* const result = c_ptr_;
  c_ptr_ = nullptr;

  return result;
}

result_t stmt::do_finalize() noexcept
{
  return int_to_enum<result_t>(::sqlite3_finalize(c_ptr_));
}


}; // namespace sqleger


#endif
