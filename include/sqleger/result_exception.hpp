#ifndef SQLEGER_RESULT_EXCEPTION_HPP
#define SQLEGER_RESULT_EXCEPTION_HPP

#include <sqleger/constants.hpp>

#include <exception>


namespace sqleger {


class result_exception : public std::exception {

public:
  inline result_exception(result code) noexcept;

  result_exception(result_exception const& other) noexcept = default;

  result_exception(result_exception&& other) noexcept = default;

  inline char const* what() const noexcept override;

  constexpr result code() const noexcept { return code_; }

private:
  result const code_;
};


// =============================================================================


result_exception::result_exception(result const code) noexcept : code_ {code} {}

char const* result_exception::what() const noexcept
{
  return errstr(code_);
}


} // namespace sqleger


#endif
