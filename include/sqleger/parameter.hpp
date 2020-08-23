#ifndef SQLEGER_PARAMETER_HPP
#define SQLEGER_PARAMETER_HPP

#include <sqleger/stmt.hpp>


namespace sqleger {


class parameter {

public:
  constexpr parameter(stmt_ref statement, int index) noexcept;

  inline result bind_blob(const user_blob& value) noexcept;

  inline result bind_double(double value) noexcept;

  inline result bind_int(int value) noexcept;

  inline result bind_int64(int64 value) noexcept;

  inline result bind_null() noexcept;

  inline result bind_text(const user_text& value) noexcept;

  constexpr stmt_ref get_stmt_ref() const noexcept { return stmt_ref_; }

  constexpr int index() const noexcept { return index_; }

private:
  stmt_ref stmt_ref_;
  int index_;
};


// =============================================================================


constexpr parameter::parameter(const stmt_ref statement,
                               const int index) noexcept :
  stmt_ref_ {statement},
  index_ {index}
{
}

result parameter::bind_blob(const user_blob& value) noexcept
{
  return stmt_ref_.bind_blob(index_, value);
}

result parameter::bind_double(const double value) noexcept
{
  return stmt_ref_.bind_double(index_, value);
}

result parameter::bind_int(const int value) noexcept
{
  return stmt_ref_.bind_int(index_, value);
}

result parameter::bind_int64(const int64 value) noexcept
{
  return stmt_ref_.bind_int64(index_, value);
}

result parameter::bind_null() noexcept
{
  return stmt_ref_.bind_null(index_);
}

result parameter::bind_text(const user_text& value) noexcept
{
  return stmt_ref_.bind_text(index_, value);
}


}; // namespace sqleger


#endif
