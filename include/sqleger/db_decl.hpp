#ifndef SQLEGER_DB_DECL_HPP
#define SQLEGER_DB_DECL_HPP

#include <sqleger/constants.hpp>
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

  result_t prepare_v2(const string_span& sql, stmt& result) noexcept;

  zstring_view errmsg() const noexcept;

private:
  c_type* c_ptr() const noexcept;
};


class db : public db_interface<db> {

public:
  using interface_type = db_interface<db>;

  static inline result_t open(const zstring_view& filename,
                              db& result) noexcept;

  static inline result_t
  open_v2(const zstring_view& filename, db& result, open_t flags) noexcept;

  constexpr db() noexcept = default;

  inline db(const zstring_view& filename);

  inline db(const zstring_view& filename, open_t flags);

  explicit constexpr db(c_type* c_ptr) noexcept;

  constexpr db(db&& other) noexcept;

  inline db& operator=(db&& other) noexcept;

  inline ~db() noexcept;

  inline result_t close() noexcept;

  inline result_t close_v2() noexcept;

  constexpr c_type* take_c_ptr() noexcept;

  c_type* c_ptr() const noexcept { return c_ptr_; }

private:
  inline result_t do_close_v2() noexcept;

  c_type* c_ptr_ = nullptr;
};


class open_exception : public result_exception {

public:
  inline open_exception(result_t code, db&& connection) noexcept;

  open_exception(open_exception&& other) noexcept = default;

  inline const char* what() const noexcept override;

private:
  db db_;
};


}; // namespace sqleger


#endif
