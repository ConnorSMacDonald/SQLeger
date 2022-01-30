#ifndef SQLEGER_USER_TEXT_HPP
#define SQLEGER_USER_TEXT_HPP

#include <sqleger/destructor.hpp>
#include <sqleger/string.hpp>

#include <string>
#include <string_view>


namespace sqleger {


template <typename Char>
class basic_user_text {

public:
  using char_type = Char;
  using size_type = int;
  using string_span_type = basic_string_span<Char>;

  static constexpr auto zstring_size = string_span_type::zstring_size;

  basic_user_text(string_span_type span,
                  destructor_type destructor = transient) noexcept;

  constexpr char_type const* data() const noexcept { return data_; }

  constexpr size_type size_bytes() const noexcept { return size_bytes_; }

  constexpr destructor_type destructor() const noexcept { return destructor_; }

private:
  char_type const* data_ = nullptr;
  size_type size_bytes_ = {};
  destructor_type destructor_ = transient;
};

using user_text = basic_user_text<char>;


// =============================================================================


template <typename Char>
basic_user_text<Char>::basic_user_text(
  string_span_type const span,
  destructor_type const destructor) noexcept :
  data_ {span.data()},
  size_bytes_ {static_cast<size_type>(span.length() * sizeof(Char))},
  destructor_ {destructor}
{
}


} // namespace sqleger


#endif
