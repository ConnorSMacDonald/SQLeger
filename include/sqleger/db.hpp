#ifndef SQLEGER_DB_HPP
#define SQLEGER_DB_HPP

#include <sqleger/db_decl.hpp>
#include <sqleger/stmt.hpp>
#include <sqleger/utility.hpp>

#include <utility>


namespace sqleger {


template <typename Impl>
result_t db_interface<Impl>::prepare_v2(const string_span& sql,
                                        stmt& result) noexcept
{
  return int_to_enum<result_t>(::sqlite3_prepare_v2(
    c_ptr(), sql.data(), sql.size(), result.c_ptr_out(), nullptr));
}

template <typename Impl>
zstring_view db_interface<Impl>::errmsg() const noexcept
{
  return ::sqlite3_errmsg(c_ptr());
}

template <typename Impl>
constexpr auto db_interface<Impl>::c_ptr() const noexcept -> c_type*
{
  return static_cast<const impl_type*>(this)->c_ptr();
}

result_t db::open(const zstring_view& filename, db& result) noexcept
{
  return int_to_enum<result_t>(
    ::sqlite3_open(filename.c_str(), &result.c_ptr_));
}

result_t db::open_v2(const zstring_view& filename,
                     db& result,
                     const open_t flags) noexcept
{
  return int_to_enum<result_t>(::sqlite3_open_v2(
    filename.c_str(), &result.c_ptr_, enum_to_int(flags), nullptr));
}

db::db(const zstring_view& filename)
{
  if (const auto r = open(filename, *this); is_error(r))
    throw open_exception(r, std::move(*this));
}

db::db(const zstring_view& filename, const open_t flags)
{
  if (const auto r = open_v2(filename, *this, flags); is_error(r))
    throw open_exception(r, std::move(*this));
}

constexpr db::db(c_type* const c_ptr) noexcept : c_ptr_ {c_ptr} {}

constexpr db::db(db&& other) noexcept : db {other.take_c_ptr()} {}

db& db::operator=(db&& other) noexcept
{
  if (this == &other)
    return *this;

  do_close_v2();

  c_ptr_ = other.take_c_ptr();

  return *this;
}

db::~db() noexcept
{
  do_close_v2();
}

result_t db::close() noexcept
{
  const auto r = int_to_enum<result_t>(::sqlite3_close(c_ptr_));

  if (r == result_t::ok)
    c_ptr_ = nullptr;

  return r;
}

result_t db::close_v2() noexcept
{
  const auto r = do_close_v2();

  c_ptr_ = nullptr;

  return r;
}

constexpr auto db::take_c_ptr() noexcept -> c_type*
{
  return exchange_nullptr(c_ptr_);
}

result_t db::do_close_v2() noexcept
{
  return int_to_enum<result_t>(::sqlite3_close_v2(c_ptr_));
}

open_exception::open_exception(const result_t code, db&& connection) noexcept :
  result_exception {code},
  db_ {std::move(connection)}
{
}

const char* open_exception::what() const noexcept
{
  return db_.errmsg();
}


}; // namespace sqleger


#endif
