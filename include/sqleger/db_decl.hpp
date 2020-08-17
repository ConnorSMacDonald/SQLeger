#ifndef SQLEGER_DB_DECL_HPP
#define SQLEGER_DB_DECL_HPP

#include <sqleger/constants.hpp>
#include <sqleger/int.hpp>
#include <sqleger/result_exception.hpp>
#include <sqleger/string.hpp>

#include <sqlite3.h>


namespace sqleger {


class stmt;


template <typename Impl>
class db_interface {

public:
  using impl_type = Impl;
  using c_type = ::sqlite3;

  result_t prepare_v2(string_span sql, stmt& result) noexcept;

  int64 last_insert_rowid() noexcept;

  zstring_view errmsg() const noexcept;

private:
  constexpr c_type* c_ptr() const noexcept;
};


class db_ref;


class db : public db_interface<db> {

public:
  using interface_type = db_interface<db>;

  static inline result_t open(zstring_view filename, db& result) noexcept;

  static inline result_t
  open_v2(zstring_view filename, db& result, open_t flags) noexcept;

  constexpr db() noexcept = default;

  inline db(zstring_view filename);

  inline db(zstring_view filename, open_t flags);

  explicit constexpr db(c_type* c_ptr) noexcept;

  constexpr db(db&& other) noexcept;

  inline db& operator=(db&& other) noexcept;

  inline ~db() noexcept;

  inline result_t close() noexcept;

  inline result_t close_v2() noexcept;

  constexpr c_type* take_c_ptr() noexcept;

  constexpr c_type* c_ptr() const noexcept { return c_ptr_; }

  constexpr db_ref ref() noexcept;

  constexpr operator db_ref() noexcept;

private:
  inline result_t do_close_v2() noexcept;

  c_type* c_ptr_ = nullptr;
};


class open_exception : public result_exception {

public:
  inline open_exception(result_t code, db&& db_handle) noexcept;

  open_exception(open_exception&& other) noexcept = default;

  inline const char* what() const noexcept override;

private:
  db db_;
};


class db_ref : public db_interface<db_ref> {

public:
  using interface_type = db_interface<db_ref>;

  constexpr db_ref() noexcept = default;

  explicit constexpr db_ref(c_type* c_ptr) noexcept;

  explicit constexpr db_ref(db& other) noexcept;

  constexpr c_type* c_ptr() const noexcept { return c_ptr_; }

private:
  c_type* c_ptr_ = nullptr;
};


}; // namespace sqleger


#endif
