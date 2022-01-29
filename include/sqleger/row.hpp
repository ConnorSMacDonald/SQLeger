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

  inline int data_count() noexcept;

  constexpr operator bool() const noexcept;

  constexpr stmt_ref get_stmt_ref() const noexcept { return stmt_ref_; }

  class iterator;

  inline iterator begin() noexcept;
  inline iterator end() noexcept;

private:
  stmt_ref stmt_ref_;
};


class row::iterator {

public:
  using reference = value&;
  using pointer = value*;

  reference operator*() noexcept { return value_; }

  pointer operator->() noexcept { return &value_; }

  iterator& operator++() noexcept;
  iterator operator++(int) noexcept;

private:
  friend row;

  inline iterator(row r, int start_index) noexcept;

  void read();

  friend bool operator==(iterator const& l, iterator const& r) noexcept;
  friend bool operator!=(iterator const& l, iterator const& r) noexcept;

  row row_;
  int index_ = 0;
  value value_;
};

bool operator==(row::iterator const& l, row::iterator const& r) noexcept;
bool operator!=(row::iterator const& l, row::iterator const& r) noexcept;


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

int row::data_count() noexcept
{
  return stmt_ref_.data_count();
}

constexpr row::operator bool() const noexcept
{
  return static_cast<bool>(stmt_ref_);
}

row::iterator row::begin() noexcept
{
  return iterator(*this, 0);
}

row::iterator row::end() noexcept
{
  return iterator(*this, data_count());
}

row::iterator& row::iterator::operator++() noexcept
{
  ++index_;

  read();

  return *this;
}

row::iterator row::iterator::operator++(int) noexcept
{
  auto const result = *this;

  ++(*this);

  return result;
}

row::iterator::iterator(row r, int const start_index) noexcept :
  row_ {r}, index_ {start_index}
{
  read();
}

void row::iterator::read()
{
  if (index_ < row_.data_count())
    value_ = row_.column_value(index_).dup();
}

bool operator==(row::iterator const& l, row::iterator const& r) noexcept
{
  return l.row_.get_stmt_ref().c_ptr() == r.row_.get_stmt_ref().c_ptr()
         && l.index_ == r.index_;
}

bool operator!=(row::iterator const& l, row::iterator const& r) noexcept
{
  return !(l == r);
}


} // namespace sqleger


#endif
