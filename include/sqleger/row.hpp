#ifndef SQLEGER_ROW_HPP
#define SQLEGER_ROW_HPP

#include <sqleger/db.hpp>


namespace sqleger {


class row {

public:
  constexpr row() noexcept = default;

  explicit constexpr row(stmt_ref s) noexcept;

  inline blob_data column_blob(int index) noexcept;

  inline double column_double(int index) noexcept;

  inline int column_int(int index) noexcept;

  inline int64 column_int64(int index) noexcept;

  inline uzstring_view column_text(int index) noexcept;

  inline value_ref column_value(int index) noexcept;

  inline int column_bytes(int index) noexcept;

  inline datatype column_type(int index) noexcept;

  constexpr operator bool() const noexcept;

  constexpr stmt_ref get_stmt_ref() const noexcept { return stmt_ref_; }

private:
  stmt_ref stmt_ref_;
};


// =============================================================================


constexpr row::row(stmt_ref const s) noexcept : stmt_ref_ {s} {}

blob_data row::column_blob(int const index) noexcept
{
  return stmt_ref_.column_blob(index);
}

double row::column_double(int const index) noexcept
{
  return stmt_ref_.column_double(index);
}

int row::column_int(int const index) noexcept
{
  return stmt_ref_.column_int(index);
}

int64 row::column_int64(int const index) noexcept
{
  return stmt_ref_.column_int64(index);
}

uzstring_view row::column_text(int const index) noexcept
{
  return stmt_ref_.column_text(index);
}

value_ref row::column_value(int const index) noexcept
{
  return stmt_ref_.column_value(index);
}

int row::column_bytes(int const index) noexcept
{
  return stmt_ref_.column_bytes(index);
}

datatype row::column_type(int const index) noexcept
{
  return stmt_ref_.column_type(index);
}

constexpr row::operator bool() const noexcept
{
  return static_cast<bool>(stmt_ref_);
}


} // namespace sqleger


#endif
