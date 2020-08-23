#ifndef SQLEGER_COLUMN_TRAITS_HPP
#define SQLEGER_COLUMN_TRAITS_HPP

#include <sqleger/null.hpp>
#include <sqleger/value.hpp>


namespace sqleger {


template <typename T>
class column_traits;

template <>
struct column_traits<blob_data> {
  static inline blob_data from_value(value_ref v) noexcept;
};

template <>
struct column_traits<double> {
  static inline double from_value(value_ref v) noexcept;
};

template <>
struct column_traits<int> {
  static inline int from_value(value_ref v) noexcept;
};

template <>
struct column_traits<int64> {
  static inline int64 from_value(value_ref v) noexcept;
};

template <>
struct column_traits<null_t> {
  static inline null_t from_value(value_ref v) noexcept;
};

template <>
struct column_traits<uzstring_view> {
  static inline uzstring_view from_value(value_ref v) noexcept;
};


// =============================================================================


blob_data column_traits<blob_data>::from_value(value_ref v) noexcept
{
  return v.blob();
}

double column_traits<double>::from_value(value_ref v) noexcept
{
  return v._double();
}

int column_traits<int>::from_value(value_ref v) noexcept
{
  return v._int();
}

int64 column_traits<int64>::from_value(value_ref v) noexcept
{
  return v.int64();
}

null_t column_traits<null_t>::from_value(value_ref) noexcept
{
  return null;
}

uzstring_view column_traits<uzstring_view>::from_value(value_ref v) noexcept
{
  return v.text();
}


}; // namespace sqleger


#endif
