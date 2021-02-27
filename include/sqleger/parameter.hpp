#ifndef SQLEGER_PARAMETER_HPP
#define SQLEGER_PARAMETER_HPP

#include <sqleger/stmt.hpp>


namespace sqleger {


class parameter {

public:
  constexpr parameter(stmt_ref statement, int index) noexcept;

  inline result bind_blob(user_blob const& value) noexcept;

  inline result bind_double(double value) noexcept;

  inline result bind_int(int value) noexcept;

  inline result bind_int64(int64 value) noexcept;

  inline result bind_null() noexcept;

  inline result bind_text(user_text const& value) noexcept;

  constexpr stmt_ref get_stmt_ref() const noexcept { return stmt_ref_; }

  constexpr int index() const noexcept { return index_; }

private:
  stmt_ref stmt_ref_;
  int index_;
};


// =============================================================================


constexpr parameter::parameter(stmt_ref const statement,
                               int const index) noexcept :
  stmt_ref_ {statement}, index_ {index}
{
}

result parameter::bind_blob(user_blob const& value) noexcept
{
  return stmt_ref_.bind_blob(index_, value);
}

result parameter::bind_double(double const value) noexcept
{
  return stmt_ref_.bind_double(index_, value);
}

result parameter::bind_int(int const value) noexcept
{
  return stmt_ref_.bind_int(index_, value);
}

result parameter::bind_int64(int64 const value) noexcept
{
  return stmt_ref_.bind_int64(index_, value);
}

result parameter::bind_null() noexcept
{
  return stmt_ref_.bind_null(index_);
}

result parameter::bind_text(user_text const& value) noexcept
{
  return stmt_ref_.bind_text(index_, value);
}


} // namespace sqleger


#endif
