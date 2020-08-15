#ifndef SQLEGER_STRING_HPP
#define SQLEGER_STRING_HPP

#include <string>
#include <string_view>


namespace sqleger {


template <typename Char>
class basic_zstring_view {

public:
  using char_type = Char;
  using default_traits_type = typename std::char_traits<char_type>;
  using default_allocator_type = typename std::allocator<char_type>;

  constexpr basic_zstring_view(const char_type* c_str) noexcept;

  template <typename Traits, typename Allocator>
  basic_zstring_view(
    const std::basic_string<char_type, Traits, Allocator>& string) noexcept;

  constexpr int compare(basic_zstring_view other) const noexcept;

  constexpr const char_type* c_str() const noexcept { return c_str_; }

  template <typename Traits = default_traits_type,
            typename Allocator = default_allocator_type>
  std::basic_string<char_type, Traits, Allocator>
  to_std_string(const Allocator& allocator = Allocator()) const noexcept;

  template <typename Traits = default_traits_type>
  constexpr std::basic_string_view<char_type, Traits> to_std_string_view() const
    noexcept;

  constexpr operator const char_type*() const noexcept { return c_str(); }

  template <typename Traits, typename Allocator>
  operator std::basic_string<char_type, Traits, Allocator>() const noexcept;

  template <typename Traits>
  constexpr operator std::basic_string_view<char_type, Traits>() const noexcept;

private:
  const char_type* c_str_;
};

template <typename Char>
constexpr bool operator==(basic_zstring_view<Char> left,
                          basic_zstring_view<Char> right) noexcept;

template <typename Char>
constexpr bool operator!=(basic_zstring_view<Char> left,
                          basic_zstring_view<Char> right) noexcept;

template <typename Char>
constexpr bool operator<(basic_zstring_view<Char> left,
                         basic_zstring_view<Char> right) noexcept;

template <typename Char>
constexpr bool operator<=(basic_zstring_view<Char> left,
                          basic_zstring_view<Char> right) noexcept;

template <typename Char>
constexpr bool operator>(basic_zstring_view<Char> left,
                         basic_zstring_view<Char> right) noexcept;

template <typename Char>
constexpr bool operator>=(basic_zstring_view<Char> left,
                          basic_zstring_view<Char> right) noexcept;


using zstring_view = basic_zstring_view<char>;
using uzstring_view = basic_zstring_view<unsigned char>;


inline namespace literals {
inline namespace zstring_view_literals {

constexpr zstring_view operator""_zv(const char* c_str, std::size_t) noexcept;

}; // namespace zstring_view_literals
}; // namespace literals


template <typename Char>
class basic_string_span {

public:
  using char_type = Char;
  using default_traits_type = typename std::char_traits<char_type>;
  using default_allocator_type = typename std::allocator<char_type>;
  using size_type = int;

  static constexpr size_type zstring_size = -1;

  constexpr basic_string_span(const char_type* data,
                              size_type size = zstring_size) noexcept;

  constexpr basic_string_span(basic_zstring_view<char_type> view,
                              size_type size = zstring_size) noexcept;

  template <typename Traits, typename Allocator>
  basic_string_span(
    const std::basic_string<char_type, Traits, Allocator>& string) noexcept;

  template <typename Traits>
  constexpr basic_string_span(
    const std::basic_string_view<char_type, Traits>& string_view) noexcept;

  constexpr int compare(basic_string_span other) const noexcept;

  template <typename Traits = default_traits_type,
            typename Allocator = default_allocator_type>
  std::basic_string<char_type, Traits, Allocator>
  to_std_string(const Allocator& allocator = Allocator()) const;

  template <typename Traits = default_traits_type>
  constexpr std::basic_string_view<char_type, Traits> to_std_string_view() const
    noexcept;

  template <typename Traits, typename Allocator>
  operator std::basic_string<char_type, Traits, Allocator>() const;

  template <typename Traits>
  constexpr operator std::basic_string_view<char_type, Traits>() const noexcept;

  constexpr const char_type* data() const noexcept { return data_; }

  constexpr size_type size() const noexcept { return size_; }

  constexpr size_type length() const noexcept { return size(); }

private:
  const char_type* data_ = nullptr;
  size_type size_ = zstring_size;
};

template <typename Char>
constexpr bool operator==(basic_string_span<Char> left,
                          basic_string_span<Char> right) noexcept;

template <typename Char>
constexpr bool operator!=(basic_string_span<Char> left,
                          basic_string_span<Char> right) noexcept;

template <typename Char>
constexpr bool operator<(basic_string_span<Char> left,
                         basic_string_span<Char> right) noexcept;

template <typename Char>
constexpr bool operator<=(basic_string_span<Char> left,
                          basic_string_span<Char> right) noexcept;

template <typename Char>
constexpr bool operator>(basic_string_span<Char> left,
                         basic_string_span<Char> right) noexcept;

template <typename Char>
constexpr bool operator>=(basic_string_span<Char> left,
                          basic_string_span<Char> right) noexcept;


using string_span = basic_string_span<char>;


inline namespace literals {
inline namespace string_span_literals {

constexpr string_span operator""_ss(const char* data,
                                    std::size_t length) noexcept;

}; // namespace string_span_literals
}; // namespace literals


// =============================================================================


template <typename Char>
constexpr basic_zstring_view<Char>::basic_zstring_view(
  const char_type* const c_str) noexcept :
  c_str_ {c_str}
{
}

template <typename Char>
template <typename Traits, typename Allocator>
basic_zstring_view<Char>::basic_zstring_view(
  const std::basic_string<char_type, Traits, Allocator>& string) noexcept :
  zstring_view {string.c_str()}
{
}

template <typename Char>
constexpr int
basic_zstring_view<Char>::compare(const basic_zstring_view other) const noexcept
{
  return to_std_string_view().compare(other.to_std_string_view());
}

template <typename Char>
template <typename Traits, typename Allocator>
auto basic_zstring_view<Char>::to_std_string(const Allocator& allocator) const
  noexcept -> std::basic_string<char_type, Traits, Allocator>
{
  return std::basic_string<char_type, Traits, Allocator>(c_str_);
}

template <typename Char>
template <typename Traits>
constexpr auto basic_zstring_view<Char>::to_std_string_view() const noexcept
  -> std::basic_string_view<char_type, Traits>
{
  return std::basic_string_view<char_type, Traits>(c_str_);
}

template <typename Char>
template <typename Traits, typename Allocator>
basic_zstring_view<Char>::
operator std::basic_string<char_type, Traits, Allocator>() const noexcept
{
  return to_std_string();
}

template <typename Char>
template <typename Traits>
constexpr basic_zstring_view<Char>::
operator std::basic_string_view<char_type, Traits>() const noexcept
{
  return to_std_string_view();
}

template <typename Char>
constexpr bool operator==(const basic_zstring_view<Char> left,
                          const basic_zstring_view<Char> right) noexcept
{
  return left.compare(right) == 0;
}

template <typename Char>
constexpr bool operator!=(const basic_zstring_view<Char> left,
                          const basic_zstring_view<Char> right) noexcept
{
  return !(left == right);
}

template <typename Char>
constexpr bool operator<(const basic_zstring_view<Char> left,
                         const basic_zstring_view<Char> right) noexcept
{
  return left.compare(right) < 0;
}

template <typename Char>
constexpr bool operator<=(const basic_zstring_view<Char> left,
                          const basic_zstring_view<Char> right) noexcept
{
  return !(left > right);
}

template <typename Char>
constexpr bool operator>(const basic_zstring_view<Char> left,
                         const basic_zstring_view<Char> right) noexcept
{
  return right < left;
}

template <typename Char>
constexpr bool operator>=(const basic_zstring_view<Char> left,
                          const basic_zstring_view<Char> right) noexcept
{
  return !(left < right);
}

inline namespace literals {
inline namespace zstring_view_literals {

constexpr zstring_view operator""_zv(const char* const c_str,
                                     std::size_t) noexcept
{
  return c_str;
}

}; // namespace zstring_view_literals
}; // namespace literals


template <typename Char>
constexpr basic_string_span<Char>::basic_string_span(
  const char_type* const data,
  const size_type size) noexcept :
  data_ {data},
  size_ {size}
{
}

template <typename Char>
constexpr basic_string_span<Char>::basic_string_span(
  const basic_zstring_view<Char> view,
  const size_type size) noexcept :
  basic_string_span {view.c_str(), size}
{
}

template <typename Char>
template <typename Traits, typename Allocator>
basic_string_span<Char>::basic_string_span(
  const std::basic_string<char_type, Traits, Allocator>& string) noexcept :
  basic_string_span {string.c_str(), static_cast<size_type>(string.size())}
{
}

template <typename Char>
template <typename Traits>
constexpr basic_string_span<Char>::basic_string_span(
  const std::basic_string_view<char_type, Traits>& string_view) noexcept :
  basic_string_span {string_view.data(),
                     static_cast<size_type>(string_view.size())}
{
}

template <typename Char>
constexpr int
basic_string_span<Char>::compare(const basic_string_span other) const noexcept
{
  return to_std_string_view().compare(other.to_std_string_view());
}

template <typename Char>
template <typename Traits, typename Allocator>
auto basic_string_span<Char>::to_std_string(const Allocator& allocator) const
  -> std::basic_string<char_type, Traits, Allocator>
{
  using result_type = std::basic_string<char_type, Traits, Allocator>;

  return (size_ == zstring_size)
           ? result_type(data_)
           : result_type(data_,
                         static_cast<typename result_type::size_type>(size_));
}

template <typename Char>
template <typename Traits>
constexpr auto basic_string_span<Char>::to_std_string_view() const noexcept
  -> std::basic_string_view<char_type, Traits>
{
  using result_type = std::basic_string_view<char_type, Traits>;

  return (size_ == zstring_size)
           ? result_type(data_)
           : result_type(data_,
                         static_cast<typename result_type::size_type>(size_));
}

template <typename Char>
template <typename Traits, typename Allocator>
basic_string_span<Char>::
operator std::basic_string<char_type, Traits, Allocator>() const
{
  return to_std_string();
}

template <typename Char>
template <typename Traits>
constexpr basic_string_span<Char>::
operator std::basic_string_view<char_type, Traits>() const noexcept
{
  return to_std_string_view();
}

template <typename Char>
constexpr bool operator==(const basic_string_span<Char> left,
                          const basic_string_span<Char> right) noexcept
{
  return left.compare(right) == 0;
}

template <typename Char>
constexpr bool operator!=(const basic_string_span<Char> left,
                          const basic_string_span<Char> right) noexcept
{
  return !(left == right);
}

template <typename Char>
constexpr bool operator<(const basic_string_span<Char> left,
                         const basic_string_span<Char> right) noexcept
{
  return left.compare(right) < 0;
}

template <typename Char>
constexpr bool operator<=(const basic_string_span<Char> left,
                          const basic_string_span<Char> right) noexcept
{
  return !(left > right);
}

template <typename Char>
constexpr bool operator>(const basic_string_span<Char> left,
                         const basic_string_span<Char> right) noexcept
{
  return right < left;
}

template <typename Char>
constexpr bool operator>=(const basic_string_span<Char> left,
                          const basic_string_span<Char> right) noexcept
{
  return !(left < right);
}

inline namespace literals {
inline namespace string_span_literals {

constexpr string_span operator""_ss(const char* const data,
                                    const std::size_t length) noexcept
{
  return {data, static_cast<int>(length)};
}

}; // namespace string_span_literals
}; // namespace literals


}; // namespace sqleger


#endif
