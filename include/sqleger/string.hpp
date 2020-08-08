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
const Char** to_c_str_ptr(basic_zstring_view<Char>* view) noexcept;

template <typename Char>
const Char** to_c_str_ptr(basic_zstring_view<Char>& view) noexcept;


using zstring_view = basic_zstring_view<char>;


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
const Char** to_c_str_ptr(basic_zstring_view<Char>* const view) noexcept
{
  return reinterpret_cast<const Char**>(view);
}

template <typename Char>
const Char** to_c_str_ptr(basic_zstring_view<Char>& view) noexcept
{
  return to_c_str_ptr(&view);
}


}; // namespace sqleger


#endif
