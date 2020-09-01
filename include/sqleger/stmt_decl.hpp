#ifndef SQLEGER_STMT_DECL_HPP
#define SQLEGER_STMT_DECL_HPP

#include <sqleger/blob_data.hpp>
#include <sqleger/db_decl.hpp>
#include <sqleger/int.hpp>
#include <sqleger/user_blob.hpp>
#include <sqleger/user_text.hpp>
#include <sqleger/utility.hpp>
#include <sqleger/value.hpp>


namespace sqleger {


template <typename Impl>
class stmt_interface {

public:
  using impl_type = Impl;
  using c_type = ::sqlite3_stmt;

  result bind_blob(int index, const user_blob& data) noexcept;

  result bind_double(int index, double value) noexcept;

  result bind_int(int index, int value) noexcept;

  result bind_int64(int index, int64 value) noexcept;

  result bind_null(int index) noexcept;

  result bind_text(int index, const user_text& text) noexcept;

  blob_data column_blob(int index) noexcept;

  double column_double(int index) noexcept;

  int column_int(int index) noexcept;

  int64 column_int64(int index) noexcept;

  uzstring_view column_text(int index) noexcept;

  value_ref column_value(int index) noexcept;

  int column_bytes(int index) noexcept;

  datatype column_type(int index) noexcept;

  result step() noexcept;

  result reset() noexcept;

  result clear_bindings() noexcept;

  zstring_view sql() const noexcept;

  constexpr operator bool() const noexcept;

private:
  constexpr c_type* c_ptr() const noexcept;
};


class stmt_ref;


class stmt : public stmt_interface<stmt> {

public:
  using interface_type = stmt_interface<stmt>;

  constexpr stmt() noexcept = default;

  inline stmt(db_ref db_handle, string_span sql);

  explicit constexpr stmt(c_type* c_ptr) noexcept;

  constexpr stmt(stmt&& other) noexcept;

  inline stmt& operator=(stmt&& other) noexcept;

  inline ~stmt() noexcept;

  inline result finalize() noexcept;

  constexpr c_type* take_c_ptr() noexcept;

  constexpr c_type** c_ptr_out() noexcept { return &c_ptr_; }

  constexpr c_type* c_ptr() const noexcept { return c_ptr_; }

  constexpr stmt_ref ref() noexcept;

  constexpr operator stmt_ref() noexcept;

private:
  inline result do_finalize() noexcept;

  c_type* c_ptr_ = nullptr;
};


class stmt_ref : public stmt_interface<stmt_ref> {

public:
  using interface_type = stmt_interface<stmt_ref>;

  constexpr stmt_ref() noexcept = default;

  explicit constexpr stmt_ref(c_type* c_ptr) noexcept;

  explicit constexpr stmt_ref(stmt& other) noexcept;

  constexpr c_type* c_ptr() const noexcept { return c_ptr_; }

private:
  c_type* c_ptr_ = nullptr;
};


}; // namespace sqleger


#endif
