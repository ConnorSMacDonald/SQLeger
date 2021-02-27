#ifndef SQLEGER_STRING_HPP
#define SQLEGER_STRING_HPP

#include <string>
#include <string_view>
#include <vector>


namespace sqleger {


template <typename Char>
class basic_zstring_view {

public:
  using char_type = Char;
  using default_traits_type = typename std::char_traits<char_type>;
  using default_allocator_type = typename std::allocator<char_type>;

  constexpr basic_zstring_view(char_type const* c_str) noexcept;

  template <typename Traits, typename Allocator>
  basic_zstring_view(
    std::basic_string<char_type, Traits, Allocator> const& string) noexcept;

  constexpr int compare(basic_zstring_view other) const noexcept;

  constexpr char_type const* c_str() const noexcept { return c_str_; }

  template <typename Traits = default_traits_type,
            typename Allocator = default_allocator_type>
  std::basic_string<char_type, Traits, Allocator>
  to_std_string(Allocator const& allocator = Allocator()) const noexcept;

  template <typename Traits = default_traits_type>
  constexpr std::basic_string_view<char_type, Traits>
  to_std_string_view() const noexcept;

  constexpr operator char_type const *() const noexcept { return c_str(); }

  template <typename Traits, typename Allocator>
  operator std::basic_string<char_type, Traits, Allocator>() const noexcept;

  template <typename Traits>
  constexpr operator std::basic_string_view<char_type, Traits>() const noexcept;

private:
  char_type const* c_str_;
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


template <typename Traits = std::char_traits<char>,
          typename Allocator = std::allocator<char>>
std::basic_string<char, Traits, Allocator>
utf8_to_ascii(uzstring_view utf8, Allocator const& allocator = Allocator());


inline namespace literals {
inline namespace zstring_view_literals {

constexpr zstring_view operator""_zv(char const* c_str, std::size_t) noexcept;

}; // namespace zstring_view_literals
}; // namespace literals


template <typename Char>
class basic_string_span {

public:
  using char_type = Char;
  using default_traits_type = typename std::char_traits<char_type>;
  using default_allocator_type = typename std::allocator<char_type>;
  using size_type = int;

  static constexpr size_type zstring_length = -1;
  static constexpr size_type zstring_size = zstring_length;

  constexpr basic_string_span(char_type const* data,
                              size_type size = zstring_size) noexcept;

  constexpr basic_string_span(basic_zstring_view<char_type> view,
                              size_type size = zstring_size) noexcept;

  template <typename Traits, typename Allocator>
  basic_string_span(
    std::basic_string<char_type, Traits, Allocator> const& string) noexcept;

  template <typename Traits>
  constexpr basic_string_span(
    std::basic_string_view<char_type, Traits> const& string_view) noexcept;

  constexpr int compare(basic_string_span other) const noexcept;

  template <typename Traits = default_traits_type,
            typename Allocator = default_allocator_type>
  std::basic_string<char_type, Traits, Allocator>
  to_std_string(Allocator const& allocator = Allocator()) const;

  template <typename Traits = default_traits_type>
  constexpr std::basic_string_view<char_type, Traits>
  to_std_string_view() const noexcept;

  template <typename Traits, typename Allocator>
  operator std::basic_string<char_type, Traits, Allocator>() const;

  template <typename Traits>
  constexpr operator std::basic_string_view<char_type, Traits>() const noexcept;

  constexpr char_type const* data() const noexcept { return data_; }

  constexpr size_type size() const noexcept { return size_; }

  constexpr size_type length() const noexcept { return size(); }

private:
  char_type const* data_ = nullptr;
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
using ustring_span = basic_string_span<unsigned char>;


template <typename Traits = std::char_traits<char>,
          typename Allocator = std::allocator<char>>
std::basic_string<char, Traits, Allocator>
utf8_to_ascii(ustring_span utf8, Allocator const& allocator = Allocator());


inline namespace literals {
inline namespace string_span_literals {

constexpr string_span operator""_ss(char const* data,
                                    std::size_t length) noexcept;

}; // namespace string_span_literals
}; // namespace literals


template <typename Char>
constexpr int zlength(basic_zstring_view<Char> view) noexcept;


// =============================================================================


template <typename Char>
constexpr basic_zstring_view<Char>::basic_zstring_view(
  char_type const* const c_str) noexcept :
  c_str_ {c_str}
{
}

template <typename Char>
template <typename Traits, typename Allocator>
basic_zstring_view<Char>::basic_zstring_view(
  std::basic_string<char_type, Traits, Allocator> const& string) noexcept :
  zstring_view {string.c_str()}
{
}

template <typename Char>
constexpr int
basic_zstring_view<Char>::compare(basic_zstring_view const other) const noexcept
{
  return to_std_string_view().compare(other.to_std_string_view());
}

template <typename Char>
template <typename Traits, typename Allocator>
auto basic_zstring_view<Char>::to_std_string(Allocator const& allocator)
  const noexcept -> std::basic_string<char_type, Traits, Allocator>
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
basic_zstring_view<Char>::operator std::
  basic_string<char_type, Traits, Allocator>() const noexcept
{
  return to_std_string();
}

template <typename Char>
template <typename Traits>
constexpr basic_zstring_view<
  Char>::operator std::basic_string_view<char_type, Traits>() const noexcept
{
  return to_std_string_view();
}

template <typename Char>
constexpr bool operator==(basic_zstring_view<Char> const left,
                          basic_zstring_view<Char> const right) noexcept
{
  return left.compare(right) == 0;
}

template <typename Char>
constexpr bool operator!=(basic_zstring_view<Char> const left,
                          basic_zstring_view<Char> const right) noexcept
{
  return !(left == right);
}

template <typename Char>
constexpr bool operator<(basic_zstring_view<Char> const left,
                         basic_zstring_view<Char> const right) noexcept
{
  return left.compare(right) < 0;
}

template <typename Char>
constexpr bool operator<=(basic_zstring_view<Char> const left,
                          basic_zstring_view<Char> const right) noexcept
{
  return !(left > right);
}

template <typename Char>
constexpr bool operator>(basic_zstring_view<Char> const left,
                         basic_zstring_view<Char> const right) noexcept
{
  return right < left;
}

template <typename Char>
constexpr bool operator>=(basic_zstring_view<Char> const left,
                          basic_zstring_view<Char> const right) noexcept
{
  return !(left < right);
}

template <typename Traits, typename Allocator>
std::basic_string<char, Traits, Allocator>
utf8_to_ascii(uzstring_view const utf8, Allocator const& allocator)
{
  using std_string_type = std::basic_string<char, Traits, Allocator>;

  auto const length
    = static_cast<typename std_string_type::size_type>(zlength(utf8));

  auto ascii = std_string_type(length, '\0', allocator);

  std::transform(utf8.c_str(),
                 utf8.c_str() + length,
                 ascii.begin(),
                 [](unsigned char const c) { return static_cast<char>(c); });

  return ascii;
}

inline namespace literals {
inline namespace zstring_view_literals {

constexpr zstring_view operator""_zv(char const* const c_str,
                                     std::size_t) noexcept
{
  return c_str;
}

}; // namespace zstring_view_literals
}; // namespace literals


template <typename Char>
constexpr basic_string_span<Char>::basic_string_span(
  char_type const* const data,
  size_type const size) noexcept :
  data_ {data}, size_ {size}
{
}

template <typename Char>
constexpr basic_string_span<Char>::basic_string_span(
  basic_zstring_view<Char> const view,
  size_type const size) noexcept :
  basic_string_span {view.c_str(), size}
{
}

template <typename Char>
template <typename Traits, typename Allocator>
basic_string_span<Char>::basic_string_span(
  std::basic_string<char_type, Traits, Allocator> const& string) noexcept :
  basic_string_span {string.c_str(), static_cast<size_type>(string.size())}
{
}

template <typename Char>
template <typename Traits>
constexpr basic_string_span<Char>::basic_string_span(
  std::basic_string_view<char_type, Traits> const& string_view) noexcept :
  basic_string_span {string_view.data(),
                     static_cast<size_type>(string_view.size())}
{
}

template <typename Char>
constexpr int
basic_string_span<Char>::compare(basic_string_span const other) const noexcept
{
  return to_std_string_view().compare(other.to_std_string_view());
}

template <typename Char>
template <typename Traits, typename Allocator>
auto basic_string_span<Char>::to_std_string(Allocator const& allocator) const
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
basic_string_span<
  Char>::operator std::basic_string<char_type, Traits, Allocator>() const
{
  return to_std_string();
}

template <typename Char>
template <typename Traits>
constexpr basic_string_span<
  Char>::operator std::basic_string_view<char_type, Traits>() const noexcept
{
  return to_std_string_view();
}

template <typename Char>
constexpr bool operator==(basic_string_span<Char> const left,
                          basic_string_span<Char> const right) noexcept
{
  return left.compare(right) == 0;
}

template <typename Char>
constexpr bool operator!=(basic_string_span<Char> const left,
                          basic_string_span<Char> const right) noexcept
{
  return !(left == right);
}

template <typename Char>
constexpr bool operator<(basic_string_span<Char> const left,
                         basic_string_span<Char> const right) noexcept
{
  return left.compare(right) < 0;
}

template <typename Char>
constexpr bool operator<=(basic_string_span<Char> const left,
                          basic_string_span<Char> const right) noexcept
{
  return !(left > right);
}

template <typename Char>
constexpr bool operator>(basic_string_span<Char> const left,
                         basic_string_span<Char> const right) noexcept
{
  return right < left;
}

template <typename Char>
constexpr bool operator>=(basic_string_span<Char> const left,
                          basic_string_span<Char> const right) noexcept
{
  return !(left < right);
}

template <typename Traits, typename Allocator>
std::basic_string<char, Traits, Allocator>
utf8_to_ascii(ustring_span const utf8, Allocator const& allocator)
{
  using std_string_type = std::basic_string<char, Traits, Allocator>;

  auto const real_length = static_cast<typename std_string_type::size_type>(
    (utf8.length() == ustring_span::zstring_size)
      ? zlength(uzstring_view(utf8.data()))
      : utf8.length());

  auto ascii = std_string_type(real_length, '\0', allocator);

  std::transform(utf8.data(),
                 utf8.data() + real_length,
                 ascii.begin(),
                 [](unsigned char const c) { return static_cast<char>(c); });

  return ascii;
}

inline namespace literals {
inline namespace string_span_literals {

constexpr string_span operator""_ss(char const* const data,
                                    std::size_t const length) noexcept
{
  return {data, static_cast<int>(length)};
}

}; // namespace string_span_literals
}; // namespace literals

template <typename Char>
constexpr int zlength(basic_zstring_view<Char> const view) noexcept
{
  int length {};
  auto const* it = view.c_str();

  while (*it++ != static_cast<Char>('\0'))
    ++length;

  return length;
}


}; // namespace sqleger


#endif
