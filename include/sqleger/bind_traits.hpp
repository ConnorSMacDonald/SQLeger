#ifndef SQLEGER_BIND_TRAITS_HPP
#define SQLEGER_BIND_TRAITS_HPP

#include <sqleger/null.hpp>
#include <sqleger/parameter.hpp>


namespace sqleger {


template <typename T>
struct bind_traits;

template <>
struct bind_traits<user_blob> {
  static inline result bind(parameter p, const user_blob& value) noexcept;
};

template <>
struct bind_traits<double> {
  static inline result bind(parameter p, double value) noexcept;
};

template <>
struct bind_traits<int> {
  static inline result bind(parameter p, int value) noexcept;
};

template <>
struct bind_traits<int64> {
  static inline result bind(parameter p, int64 value) noexcept;
};

template <>
struct bind_traits<null_t> {
  static inline result bind(parameter p, null_t) noexcept;
};

template <>
struct bind_traits<user_text> {
  static inline result bind(parameter p, const user_text& value) noexcept;
};


// =============================================================================


result bind_traits<user_blob>::bind(parameter p,
                                    const user_blob& value) noexcept
{
  return p.bind_blob(value);
}

result bind_traits<double>::bind(parameter p, const double value) noexcept
{
  return p.bind_double(value);
}

result bind_traits<int>::bind(parameter p, const int value) noexcept
{
  return p.bind_int(value);
}

result bind_traits<int64>::bind(parameter p, const int64 value) noexcept
{
  return p.bind_int64(value);
}

result bind_traits<null_t>::bind(parameter p, null_t) noexcept
{
  return p.bind_null();
}

result bind_traits<user_text>::bind(parameter p,
                                    const user_text& value) noexcept
{
  return p.bind_text(value);
}


}; // namespace sqleger


#endif
