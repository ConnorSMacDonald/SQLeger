#ifndef SQLEGER_ZSTRING_HPP_INCLUDED
#define SQLEGER_ZSTRING_HPP_INCLUDED

#include <string>


namespace sqleger {


class zstring_view {

public:
  using char_type = char;

  constexpr zstring_view() noexcept = default;

  constexpr zstring_view(std::nullptr_t) noexcept {}

  constexpr zstring_view(const char_type* c_str) noexcept;

  template <typename Traits, typename Allocator>
  zstring_view(
    const std::basic_string<char_type, Traits, Allocator>& string) noexcept;

  constexpr const char_type* c_str() const noexcept { return c_str_; }

  constexpr operator const char_type*() const noexcept { return c_str(); }

private:
  const char_type* c_str_ = nullptr;
};


class zstring_view16 {

public:
  using char_type = char16_t;

  constexpr zstring_view16() noexcept = default;

  constexpr zstring_view16(std::nullptr_t) noexcept {}

  constexpr zstring_view16(const char_type* c_str) noexcept;

  constexpr zstring_view16(const void* void_c_str) noexcept;

  template <typename Traits, typename Allocator>
  zstring_view16(
    const std::basic_string<char_type, Traits, Allocator>& string) noexcept;

  constexpr const char_type* c_str() const noexcept { return c_str_; }

  constexpr operator const char_type*() const noexcept { return c_str(); }

  inline const void* void_c_str() const noexcept;

  operator const void*() const noexcept { return void_c_str(); }

private:
  const char_type* c_str_ = nullptr;
};


// =============================================================================


constexpr zstring_view::zstring_view(const char_type* const c_str) noexcept :
  c_str_ {c_str}
{
}

template <typename Traits, typename Allocator>
zstring_view::zstring_view(
  const std::basic_string<char_type, Traits, Allocator>& string) noexcept :
  zstring_view {string.c_str()}
{
}

constexpr zstring_view16::zstring_view16(
  const char_type* const c_str) noexcept :
  c_str_ {c_str}
{
}

constexpr zstring_view16::zstring_view16(
  const void* const void_c_str) noexcept :
  zstring_view16 {reinterpret_cast<const char_type*>(void_c_str)}
{
}

template <typename Traits, typename Allocator>
zstring_view16::zstring_view16(
  const std::basic_string<char_type, Traits, Allocator>& string) noexcept :
  zstring_view16 {string.c_str()}
{
}

const void* zstring_view16::void_c_str() const noexcept
{
  return reinterpret_cast<const void*>(c_str_);
}


}; // namespace sqleger


#endif
