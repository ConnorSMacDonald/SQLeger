#ifndef SQLEGER_STMT_HPP
#define SQLEGER_STMT_HPP

#include <sqleger/result_exception.hpp>
#include <sqleger/stmt_decl.hpp>

#include <sqlite3.h>


namespace sqleger {


template <typename Impl>
result stmt_interface<Impl>::bind_blob(const int index,
                                       const user_blob& data) noexcept
{
  return int_to_enum<result>(::sqlite3_bind_blob(
    c_ptr(), index, data.data(), data.size_bytes(), data.destructor()));
}

template <typename Impl>
result stmt_interface<Impl>::bind_double(const int index,
                                         const double value) noexcept
{
  return int_to_enum<result>(::sqlite3_bind_double(c_ptr(), index, value));
}

template <typename Impl>
result stmt_interface<Impl>::bind_int(const int index, const int value) noexcept
{
  return int_to_enum<result>(::sqlite3_bind_int(c_ptr(), index, value));
}

template <typename Impl>
result stmt_interface<Impl>::bind_int64(const int index,
                                        const int64 value) noexcept
{
  return int_to_enum<result>(::sqlite3_bind_int64(c_ptr(), index, value));
}

template <typename Impl>
result stmt_interface<Impl>::bind_null(const int index) noexcept
{
  return int_to_enum<result>(::sqlite3_bind_null(c_ptr(), index));
}

template <typename Impl>
result stmt_interface<Impl>::bind_text(const int index,
                                       const user_text& text) noexcept
{
  return int_to_enum<result>(::sqlite3_bind_text(
    c_ptr(), index, text.data(), text.size_bytes(), text.destructor()));
}

template <typename Impl>
blob_data stmt_interface<Impl>::column_blob(const int index) noexcept
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
value_ref stmt_interface<Impl>::column_value(const int index) noexcept
{
  return value_ref(::sqlite3_column_value(c_ptr(), index));
}

template <typename Impl>
int stmt_interface<Impl>::column_bytes(const int index) noexcept
{
  return ::sqlite3_column_bytes(c_ptr(), index);
}

template <typename Impl>
datatype stmt_interface<Impl>::column_type(const int index) noexcept
{
  return int_to_enum<datatype>(::sqlite3_column_type(c_ptr(), index));
}

template <typename Impl>
result stmt_interface<Impl>::step() noexcept
{
  return int_to_enum<result>(::sqlite3_step(c_ptr()));
}

template <typename Impl>
result stmt_interface<Impl>::reset() noexcept
{
  return int_to_enum<result>(::sqlite3_reset(c_ptr()));
}

template <typename Impl>
result stmt_interface<Impl>::clear_bindings() noexcept
{
  return int_to_enum<result>(::sqlite3_clear_bindings(c_ptr()));
}

template <typename Impl>
zstring_view stmt_interface<Impl>::sql() const noexcept
{
  return ::sqlite3_sql(c_ptr());
}

template <typename Impl>
constexpr stmt_interface<Impl>::operator bool() const noexcept
{
  return c_ptr() != nullptr;
}

template <typename Impl>
constexpr auto stmt_interface<Impl>::c_ptr() const noexcept -> c_type*
{
  return static_cast<const impl_type*>(this)->c_ptr();
}

stmt::stmt(db_ref db_handle, const string_span sql)
{
  if (const auto r = db_handle.prepare_v2(sql, *this); is_error(r))
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

result stmt::finalize() noexcept
{
  const auto r = do_finalize();

  c_ptr_ = nullptr;

  return r;
}

constexpr auto stmt::take_c_ptr() noexcept -> c_type*
{
  return exchange_nullptr(c_ptr_);
}

constexpr stmt_ref stmt::ref() noexcept
{
  return stmt_ref(*this);
}

constexpr stmt::operator stmt_ref() noexcept
{
  return ref();
}

constexpr stmt_ref::stmt_ref(c_type* const c_ptr) noexcept : c_ptr_ {c_ptr} {}

constexpr stmt_ref::stmt_ref(stmt& other) noexcept : stmt_ref {other.c_ptr()} {}

result stmt::do_finalize() noexcept
{
  return int_to_enum<result>(::sqlite3_finalize(c_ptr_));
}


}; // namespace sqleger


#endif
