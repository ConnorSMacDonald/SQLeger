#ifndef SQLEGER_EXCEPTION_HPP
#define SQLEGER_EXCEPTION_HPP

#include <sqleger/constants.hpp>

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
  inline open_exception(result_t code, const zstring_view& message) noexcept;

  open_exception(const open_exception& other) noexcept = default;

  open_exception(open_exception&& other) noexcept = default;

  inline const char* what() const noexcept override;

private:
  const zstring_view message_;
};


// =============================================================================


result_exception::result_exception(const result_t code) noexcept : code_ {code}
{
}

const char* result_exception::what() const noexcept
{
  return errstr(code_);
}

open_exception::open_exception(const result_t code,
                               const zstring_view& message) noexcept :
  result_exception {code},
  message_ {message}
{
}

const char* open_exception::what() const noexcept
{
  return message_;
}


}; // namespace sqleger


#endif
