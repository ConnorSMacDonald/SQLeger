#ifndef SQLEGER_DB_HPP
#define SQLEGER_DB_HPP

#include <sqleger/db_decl.hpp>
#include <sqleger/stmt.hpp>
#include <sqleger/utility.hpp>

#include <utility>


namespace sqleger {


template <typename Impl>
int db_interface<Impl>::changes() noexcept
{
  return ::sqlite3_changes(c_ptr());
}

template <typename Impl>
result db_interface<Impl>::prepare_v2(const string_span sql, stmt& s) noexcept
{
  return int_to_enum<result>(::sqlite3_prepare_v2(
    c_ptr(), sql.data(), sql.size(), s.c_ptr_out(), nullptr));
}

template <typename Impl>
int64 db_interface<Impl>::last_insert_rowid() noexcept
{
  return ::sqlite3_last_insert_rowid(c_ptr());
}

template <typename Impl>
zstring_view db_interface<Impl>::filename(const zstring_view db_name) noexcept
{
  return ::sqlite3_db_filename(c_ptr(), db_name.c_str());
}

template <typename Impl>
zstring_view db_interface<Impl>::errmsg() const noexcept
{
  return ::sqlite3_errmsg(c_ptr());
}

template <typename Impl>
constexpr db_interface<Impl>::operator bool() const noexcept
{
  return c_ptr() != nullptr;
}

template <typename Impl>
constexpr auto db_interface<Impl>::c_ptr() const noexcept -> c_type*
{
  return static_cast<const impl_type*>(this)->c_ptr();
}

result db::open(const zstring_view filename, db& d) noexcept
{
  return int_to_enum<result>(::sqlite3_open(filename.c_str(), &d.c_ptr_));
}

result
db::open_v2(const zstring_view filename, db& d, const enum open flags) noexcept
{
  return int_to_enum<result>(::sqlite3_open_v2(
    filename.c_str(), &d.c_ptr_, enum_to_int(flags), nullptr));
}

db::db(const zstring_view filename)
{
  if (const auto r = open(filename, *this); is_error(r))
    throw open_exception(r, std::move(*this));
}

db::db(const zstring_view filename, const enum open flags)
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

result db::close() noexcept
{
  const auto r = int_to_enum<result>(::sqlite3_close(c_ptr_));

  if (r == result::ok)
    c_ptr_ = nullptr;

  return r;
}

result db::close_v2() noexcept
{
  const auto r = do_close_v2();

  c_ptr_ = nullptr;

  return r;
}

constexpr auto db::take_c_ptr() noexcept -> c_type*
{
  return exchange_nullptr(c_ptr_);
}

constexpr db_ref db::ref() noexcept
{
  return db_ref(*this);
}

constexpr db::operator db_ref() noexcept
{
  return ref();
}

result db::do_close_v2() noexcept
{
  return int_to_enum<result>(::sqlite3_close_v2(c_ptr_));
}

open_exception::open_exception(const result code, db&& db_handle) noexcept :
  result_exception {code},
  db_ {std::move(db_handle)}
{
}

const char* open_exception::what() const noexcept
{
  return db_.errmsg();
}

constexpr db_ref::db_ref(c_type* const c_ptr) noexcept : c_ptr_ {c_ptr} {}

constexpr db_ref::db_ref(db& other) noexcept : db_ref {other.c_ptr()} {}


}; // namespace sqleger


#endif
