#ifndef SQLEGER_USER_VALUE_TRAITS_HPP
#define SQLEGER_USER_VALUE_TRAITS_HPP

#include <sqleger/parameter.hpp>


namespace sqleger {


struct null {
};


template <typename T>
struct user_value_traits;

template <>
struct user_value_traits<user_blob> {
  static inline result_t bind(parameter& p, const user_blob& value) noexcept;
};

template <>
struct user_value_traits<double> {
  static inline result_t bind(parameter& p, double value) noexcept;
};

template <>
struct user_value_traits<int> {
  static inline result_t bind(parameter& p, int value) noexcept;
};

template <>
struct user_value_traits<int64> {
  static inline result_t bind(parameter& p, int64 value) noexcept;
};

template <>
struct user_value_traits<null> {
  static inline result_t bind(parameter& p, null) noexcept;
};

template <>
struct user_value_traits<user_text> {
  static inline result_t bind(parameter& p, const user_text& value) noexcept;
};


// =============================================================================


result_t user_value_traits<user_blob>::bind(parameter& p,
                                            const user_blob& value) noexcept
{
  return p.bind_blob(value);
}

result_t user_value_traits<double>::bind(parameter& p,
                                         const double value) noexcept
{
  return p.bind_double(value);
}

result_t user_value_traits<int>::bind(parameter& p, const int value) noexcept
{
  return p.bind_int(value);
}

result_t user_value_traits<int64>::bind(parameter& p,
                                        const int64 value) noexcept
{
  return p.bind_int64(value);
}

result_t user_value_traits<null>::bind(parameter& p, null) noexcept
{
  return p.bind_null();
}

result_t user_value_traits<user_text>::bind(parameter& p,
                                            const user_text& value) noexcept
{
  return p.bind_text(value);
}


}; // namespace sqleger


#endif
