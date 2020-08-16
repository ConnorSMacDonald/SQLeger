#ifndef SQLEGER_STMT_HPP
#define SQLEGER_STMT_HPP

#include <sqleger/db_decl.hpp>
#include <sqleger/int.hpp>
#include <sqleger/result_exception.hpp>
#include <sqleger/user_blob.hpp>
#include <sqleger/user_text.hpp>
#include <sqleger/utility.hpp>

#include <sqlite3.h>


namespace sqleger {


template <typename Impl>
class stmt_interface {

public:
  using impl_type = Impl;
  using c_type = ::sqlite3_stmt;

  result_t bind_blob(int index, const user_blob& data) noexcept;

  result_t bind_double(int index, double value) noexcept;

  result_t bind_int(int index, int value) noexcept;

  result_t bind_int64(int index, int64 value) noexcept;

  result_t bind_null(int index) noexcept;

  result_t bind_text(int index, const user_text& text) noexcept;

  const void* column_blob(int index) noexcept;

  double column_double(int index) noexcept;

  int column_int(int index) noexcept;

  int64 column_int64(int index) noexcept;

  uzstring_view column_text(int index) noexcept;

  int column_bytes(int index) noexcept;

  datatype_t column_type(int index) noexcept;

  result_t step() noexcept;

  result_t reset() noexcept;

  result_t clear_bindings() noexcept;

  zstring_view sql() const noexcept;

private:
  c_type* c_ptr() const noexcept;
};


class stmt : public stmt_interface<stmt> {

public:
  using interface_type = stmt_interface<stmt>;

  constexpr stmt() noexcept = default;

  stmt(db_ref connection, string_span sql);

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
result_t stmt_interface<Impl>::bind_blob(const int index,
                                         const user_blob& data) noexcept
{
  return int_to_enum<result_t>(::sqlite3_bind_blob(
    c_ptr(), index, data.data(), data.size_bytes(), data.destructor()));
}

template <typename Impl>
result_t stmt_interface<Impl>::bind_double(const int index,
                                           const double value) noexcept
{
  return int_to_enum<result_t>(::sqlite3_bind_double(c_ptr(), index, value));
}

template <typename Impl>
result_t stmt_interface<Impl>::bind_int(const int index,
                                        const int value) noexcept
{
  return int_to_enum<result_t>(::sqlite3_bind_int(c_ptr(), index, value));
}

template <typename Impl>
result_t stmt_interface<Impl>::bind_int64(const int index,
                                          const int64 value) noexcept
{
  return int_to_enum<result_t>(::sqlite3_bind_int64(c_ptr(), index, value));
}

template <typename Impl>
result_t stmt_interface<Impl>::bind_null(const int index) noexcept
{
  return int_to_enum<result_t>(::sqlite3_bind_null(c_ptr(), index));
}

template <typename Impl>
result_t stmt_interface<Impl>::bind_text(const int index,
                                         const user_text& text) noexcept
{
  return int_to_enum<result_t>(::sqlite3_bind_text(
    c_ptr(), index, text.data(), text.size_bytes(), text.destructor()));
}

template <typename Impl>
const void* stmt_interface<Impl>::column_blob(const int index) noexcept
{
  return ::sqlite3_column_blob(c_ptr(), index);
}

template <typename Impl>
double stmt_interface<Impl>::column_double(const int index) noexcept
{
  return ::sqlite3_column_double(c_ptr(), index);
}

template <typename Impl>
int stmt_interface<Impl>::column_int(const int index) noexcept
{
  return ::sqlite3_column_int(c_ptr(), index);
}

template <typename Impl>
int64 stmt_interface<Impl>::column_int64(const int index) noexcept
{
  return ::sqlite3_column_int64(c_ptr(), index);
}

template <typename Impl>
uzstring_view stmt_interface<Impl>::column_text(const int index) noexcept
{
  return ::sqlite3_column_text(c_ptr(), index);
}

template <typename Impl>
int stmt_interface<Impl>::column_bytes(const int index) noexcept
{
  return ::sqlite3_column_bytes(c_ptr(), index);
}

template <typename Impl>
datatype_t stmt_interface<Impl>::column_type(const int index) noexcept
{
  return int_to_enum<datatype_t>(::sqlite3_column_type(c_ptr(), index));
}

template <typename Impl>
result_t stmt_interface<Impl>::step() noexcept
{
  return int_to_enum<result_t>(::sqlite3_step(c_ptr()));
}

template <typename Impl>
result_t stmt_interface<Impl>::reset() noexcept
{
  return int_to_enum<result_t>(::sqlite3_reset(c_ptr()));
}

template <typename Impl>
result_t stmt_interface<Impl>::clear_bindings() noexcept
{
  return int_to_enum<result_t>(::sqlite3_clear_bindings(c_ptr()));
}

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

stmt::stmt(db_ref connection, const string_span sql)
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
  return exchange_nullptr(c_ptr_);
}

result_t stmt::do_finalize() noexcept
{
  return int_to_enum<result_t>(::sqlite3_finalize(c_ptr_));
}


}; // namespace sqleger


#endif
