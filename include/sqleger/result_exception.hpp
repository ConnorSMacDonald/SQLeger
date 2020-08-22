#ifndef SQLEGER_RESULT_EXCEPTION_HPP
#define SQLEGER_RESULT_EXCEPTION_HPP

#include <sqleger/constants.hpp>

#include <exception>


namespace sqleger {


class result_exception : public std::exception {

public:
  inline result_exception(result code) noexcept;

  result_exception(const result_exception& other) noexcept = default;

  result_exception(result_exception&& other) noexcept = default;

  inline const char* what() const noexcept override;

  constexpr result code() const noexcept { return code_; }

private:
  const result code_;
};


// =============================================================================


result_exception::result_exception(const result code) noexcept : code_ {code} {}

const char* result_exception::what() const noexcept
{
  return errstr(code_);
}


}; // namespace sqleger


#endif
