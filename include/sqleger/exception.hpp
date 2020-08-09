#ifndef SQLEGER_EXCEPTION_HPP
#define SQLEGER_EXCEPTION_HPP

#include <sqleger/constants.hpp>
#include <sqleger/db_decl.hpp>

#include <exception>


namespace sqleger {


class result_exception : public std::exception {

public:
  inline result_exception(result_t code) noexcept;

  result_exception(const result_exception& other) noexcept = default;

  result_exception(result_exception&& other) noexcept = default;

  inline const char* what() const noexcept override;

  constexpr result_t code() const noexcept { return code_; }

private:
  const result_t code_;
};


class open_exception : public result_exception {

public:
  inline open_exception(result_t code, db&& connection) noexcept;

  open_exception(open_exception&& other) noexcept = default;

  inline const char* what() const noexcept override;

private:
  db db_;
};


// =============================================================================


result_exception::result_exception(const result_t code) noexcept : code_ {code}
{
}

const char* result_exception::what() const noexcept
{
  return errstr(code_);
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


#include <sqleger/db.hpp>


#endif
