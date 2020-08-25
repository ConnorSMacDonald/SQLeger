#ifndef SQLEGER_BINDER_HPP
#define SQLEGER_BINDER_HPP

#include <sqleger/bind_traits.hpp>

#include <tuple>
#include <utility>


namespace sqleger {


struct bind_result {
  result code;
  int index;
};

constexpr bool operator==(bind_result left, bind_result right) noexcept;
constexpr bool operator!=(bind_result left, bind_result right) noexcept;


class binder {

public:
  constexpr binder(stmt_ref statement) noexcept;

  template <typename... UserValues>
  bind_result operator()(UserValues&&... user_values) noexcept;

  stmt_ref get_stmt_ref() const noexcept { return stmt_ref_; }

  int index() const noexcept { return index_; }

  result code() const noexcept { return code_; }

private:
  template <std::size_t I, typename... UserValues>
  bind_result bind(const std::tuple<UserValues...>& user_values) noexcept;

  stmt_ref stmt_ref_;

  result code_ = result::ok;
  int index_ = 0;
};

template <typename UserValue>
binder& operator<<(binder& b, const UserValue& user_value) noexcept;


template <typename... UserValues>
bind_result bind(stmt_ref statement, UserValues&&... user_values) noexcept;


// =============================================================================


constexpr bool operator==(const bind_result left,
                          const bind_result right) noexcept
{
  return left.code == right.code && left.index == right.index;
}

constexpr bool operator!=(const bind_result left,
                          const bind_result right) noexcept
{
  return !(left == right);
}

constexpr binder::binder(const stmt_ref statement) noexcept :
  stmt_ref_ {statement}
{
}

template <typename... UserValues>
bind_result binder::operator()(UserValues&&... user_values) noexcept
{
  return bind<0>(std::forward_as_tuple(user_values...));
}

template <std::size_t I, typename... UserValues>
bind_result binder::bind(const std::tuple<UserValues...>& user_values) noexcept
{
  if constexpr (I >= sizeof...(UserValues))
    return {code_, index_};
  else
  {
    using tuple_element_t = std::tuple_element_t<I, std::tuple<UserValues...>>;
    using user_value_t
      = std::remove_cv_t<std::remove_reference_t<tuple_element_t>>;
    using bind_traits_t = bind_traits<user_value_t>;

    auto param = parameter(stmt_ref_, ++index_);

    code_ = bind_traits_t::bind(param, std::get<I>(user_values));

    if (is_error(code_))
      return {code_, index_};

    return bind<I + 1>(user_values);
  }
}

template <typename UserValue>
binder& operator<<(binder& b, const UserValue& user_value) noexcept
{
  b(user_value);
  return b;
}

template <typename... UserValues>
bind_result bind(const stmt_ref statement, UserValues&&... user_values) noexcept
{
  return binder(statement)(std::forward<UserValues>(user_values)...);
}


}; // namespace sqleger


#endif
