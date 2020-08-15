#ifndef SQLEGER_USER_TEXT_HPP
#define SQLEGER_USER_TEXT_HPP

#include <sqleger/destructor.hpp>

#include <string>
#include <string_view>


namespace sqleger {


template <typename Char>
class basic_user_text {

public:
  using char_type = Char;
  using size_type = int;

  basic_user_text(const char_type* c_str,
                  destructor_type destructor = transient) noexcept;

  basic_user_text(const char_type* data,
                  size_type length,
                  destructor_type destructor = transient) noexcept;

  template <typename Traits, typename Allocator>
  basic_user_text(const std::basic_string<char_type, Traits, Allocator>& string,
                  destructor_type destructor = transient) noexcept;

  template <typename Traits>
  basic_user_text(const std::basic_string_view<char_type, Traits>& string_view,
                  destructor_type destructor = transient) noexcept;

  constexpr const char_type* data() const noexcept { return data_; }

  constexpr size_type size_bytes() const noexcept { return size_bytes_; }

  constexpr destructor_type destructor() const noexcept { return destructor_; }

private:
  const char_type* data_ = nullptr;
  size_type size_bytes_ = {};
  destructor_type destructor_ = transient;
};

using user_text = basic_user_text<char>;


// =============================================================================


template <typename Char>
basic_user_text<Char>::basic_user_text(
  const char_type* const c_str,
  const destructor_type destructor) noexcept :
  basic_user_text {c_str, -1, destructor}
{
}

template <typename Char>
basic_user_text<Char>::basic_user_text(
  const char_type* const data,
  const size_type length,
  const destructor_type destructor) noexcept :
  data_ {data},
  size_bytes_ {static_cast<size_type>(length * sizeof(Char))},
  destructor_ {destructor}
{
}

template <typename Char>
template <typename Traits, typename Allocator>
basic_user_text<Char>::basic_user_text(
  const std::basic_string<char_type, Traits, Allocator>& string,
  const destructor_type destructor) noexcept :
  basic_user_text {string.c_str(),
                   static_cast<size_type>(string.length()),
                   destructor}
{
}

template <typename Char>
template <typename Traits>
basic_user_text<Char>::basic_user_text(
  const std::basic_string_view<char_type, Traits>& string_view,
  const destructor_type destructor) noexcept :
  basic_user_text {string_view.data(),
                   static_cast<size_type>(string_view.length()),
                   destructor}
{
}


}; // namespace sqleger


#endif
