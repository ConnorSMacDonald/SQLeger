#ifndef SQLEGER_ZSTRING_HPP_INCLUDED
#define SQLEGER_ZSTRING_HPP_INCLUDED

#include <string>
#include <string_view>


namespace sqleger {


template <typename Char>
class basic_zstring_view {

public:
  using char_type = Char;
  using default_traits_type = typename std::char_traits<char_type>;
  using default_allocator_type = typename std::allocator<char_type>;

  constexpr basic_zstring_view() noexcept = default;

  constexpr basic_zstring_view(std::nullptr_t) noexcept {}

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
  operator std::basic_string_view<char_type, Traits>() const noexcept;

  constexpr operator bool() const noexcept { return c_str_ != nullptr; }

private:
  const char_type* c_str_ = nullptr;
};

template <typename Char>
constexpr bool operator!=(const basic_zstring_view<Char>& left,
                          std::nullptr_t) noexcept;

template <typename Char>
constexpr bool operator!=(std::nullptr_t,
                          const basic_zstring_view<Char>& right) noexcept;


using zstring_view = basic_zstring_view<char>;
using zstring16_view = basic_zstring_view<char16_t>;


inline const void* to_void_ptr(const zstring16_view& view) noexcept;

inline zstring16_view to_zstring16_view(const void* void_ptr) noexcept;


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
  basic_zstring_view {string.c_str()}
{
}

template <typename Char>
template <typename Traits, typename Allocator>
auto basic_zstring_view<Char>::to_std_string(const Allocator& allocator) const
  noexcept -> std::basic_string<char_type, Traits, Allocator>
{
  return std::basic_string<char_type, Traits, Allocator>(c_str_, allocator);
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
  return to_std_string<Traits, Allocator>();
}

template <typename Char>
template <typename Traits>
basic_zstring_view<Char>::
operator std::basic_string_view<char_type, Traits>() const noexcept
{
  return to_std_string_view<Traits>();
}

template <typename Char>
constexpr bool operator==(const basic_zstring_view<Char>& left,
                          std::nullptr_t) noexcept
{
  return !left;
}

template <typename Char>
constexpr bool operator==(std::nullptr_t,
                          const basic_zstring_view<Char>& right) noexcept
{
  return right == nullptr;
}

template <typename Char>
constexpr bool operator!=(const basic_zstring_view<Char>& left,
                          std::nullptr_t) noexcept
{
  return !(left == nullptr);
}

template <typename Char>
constexpr bool operator!=(std::nullptr_t,
                          const basic_zstring_view<Char>& right) noexcept
{
  return !(nullptr == right);
}

const void* to_void_ptr(const zstring16_view& view) noexcept
{
  return reinterpret_cast<const void*>(view.c_str());
}

zstring16_view to_zstring16_view(const void* const void_ptr) noexcept
{
  return reinterpret_cast<const char16_t*>(void_ptr);
}


}; // namespace sqleger


#endif
