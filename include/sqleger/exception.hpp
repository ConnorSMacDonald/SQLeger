#ifndef SQLEGER_EXCEPTION_HPP
#define SQLEGER_EXCEPTION_HPP

#include <sqleger/constants.hpp>

#include <exception>


namespace sqleger {


class result_exception : public std::exception {

public:
  result_exception(result_t code) noexcept;

  const char* what() const noexcept override;

  constexpr result_t code() const noexcept { return code_; }

private:
  result_t code_;
};


// =============================================================================


result_exception::result_exception(const result_t code) noexcept : code_ {code}
{
}

const char* result_exception::what() const noexcept
{
  return errstr(code_);
}


}; // namespace sqleger


#endif
