#ifndef SQLEGER_COLUMN_TRAITS_HPP
#define SQLEGER_COLUMN_TRAITS_HPP

#include <sqleger/null.hpp>
#include <sqleger/value.hpp>

#include <array>
#include <optional>
#include <string>
#include <string_view>
#include <vector>


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

template <typename T, typename Allocator>
struct column_traits<std::vector<T, Allocator>> {
  static inline std::vector<T, Allocator> from_value(value_ref v) noexcept;
};

template <typename T, std::size_t N>
struct column_traits<std::array<T, N>> {
  static inline std::array<T, N> from_value(value_ref v) noexcept;
};

template <>
struct column_traits<float> {
  static inline float from_value(value_ref v) noexcept;
};

template <>
struct column_traits<short> {
  static inline short from_value(value_ref v) noexcept;
};

template <>
struct column_traits<unsigned short> {
  static inline unsigned short from_value(value_ref v) noexcept;
};

template <>
struct column_traits<unsigned int> {
  static inline unsigned int from_value(value_ref v) noexcept;
};

template <>
struct column_traits<long> {
  static inline long from_value(value_ref v) noexcept;
};

template <>
struct column_traits<unsigned long> {
  static inline unsigned long from_value(value_ref v) noexcept;
};

template <>
struct column_traits<bool> {
  static inline bool from_value(value_ref v) noexcept;
};

template <typename T>
struct column_traits<std::optional<T>> {
  static inline std::optional<T> from_value(value_ref v) noexcept;
};

template <>
struct column_traits<char> {
  static inline char from_value(value_ref v) noexcept;
};

template <>
struct column_traits<std::string> {
  static inline std::string from_value(value_ref v) noexcept;
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

template <typename T, typename Allocator>
std::vector<T, Allocator>
column_traits<std::vector<T, Allocator>>::from_value(value_ref v) noexcept
{
  const auto data = v.blob();
  const auto size = static_cast<std::size_t>(v.bytes()) / sizeof(T);

  return data.as_vector<T, Allocator>(size);
}

template <typename T, std::size_t N>
std::array<T, N>
column_traits<std::array<T, N>>::from_value(value_ref v) noexcept
{
  return v.blob().as_array<T, N>();
}

float column_traits<float>::from_value(value_ref v) noexcept
{
  return static_cast<float>(v._double());
}

short column_traits<short>::from_value(value_ref v) noexcept
{
  return static_cast<short>(v._int());
}

unsigned short column_traits<unsigned short>::from_value(value_ref v) noexcept
{
  return static_cast<unsigned short>(v._int());
}

unsigned int column_traits<unsigned int>::from_value(value_ref v) noexcept
{
  return static_cast<unsigned int>(v.int64());
}

long column_traits<long>::from_value(value_ref v) noexcept
{
  return static_cast<long>(v.int64());
}

unsigned long column_traits<unsigned long>::from_value(value_ref v) noexcept
{
  return static_cast<unsigned long>(v.int64());
}

bool column_traits<bool>::from_value(value_ref v) noexcept
{
  return static_cast<bool>(v._int());
}

template <typename T>
std::optional<T>
column_traits<std::optional<T>>::from_value(value_ref v) noexcept
{
  if (v.type() == datatype::null)
    return std::nullopt;

  return column_traits<T>::from_value(v);
}

char column_traits<char>::from_value(value_ref v) noexcept
{
  return static_cast<char>(*v.text().c_str());
}

std::string column_traits<std::string>::from_value(value_ref v) noexcept
{
  return utf8_to_ascii(v.text());
}


}; // namespace sqleger


#endif
