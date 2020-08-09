#ifndef SQLEGER_DB_HPP
#define SQLEGER_DB_HPP

#include <sqleger/db_decl.hpp>

#include <utility>


namespace sqleger {


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

constexpr db::db(c_type* const c_ptr) noexcept : c_ptr_ {c_ptr} {}

constexpr db::db(db&& other) noexcept : db {other.take_c_ptr()} {}

constexpr db& db::operator=(db&& other) noexcept
{
  if (this == &other)
    return *this;

  c_ptr_ = other.take_c_ptr();

  return *this;
}

db::~db() noexcept
{
  close_v2();
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
  return int_to_enum<result_t>(::sqlite3_close_v2(take_c_ptr()));
}

constexpr auto db::take_c_ptr() noexcept -> c_type*
{
  // TODO: in C++20, use constexpr exchange
  auto* const result = c_ptr_;
  c_ptr_ = nullptr;

  return result;
}


}; // namespace sqleger


#endif
