#ifndef SQLEGER_USER_BLOB_HPP
#define SQLEGER_USER_BLOB_HPP

#include <sqleger/destructor.hpp>
#include <sqleger/int.hpp>

#include <array>
#include <iterator>
#include <vector>


namespace sqleger {


template <typename Size>
class basic_user_blob {

public:
  using size_type = Size;

  template <typename T, auto N>
  basic_user_blob(std::array<T, N> const& data,
                  destructor_type destructor = transient) noexcept;

  template <typename T, typename Allocator>
  basic_user_blob(std::vector<T, Allocator> const& data,
                  destructor_type destructor = transient) noexcept;

  template <typename ContiguousItr>
  basic_user_blob(ContiguousItr begin,
                  ContiguousItr end,
                  destructor_type destructor = transient) noexcept;

  template <typename T>
  basic_user_blob(T const* data,
                  size_type size,
                  destructor_type destructor = transient) noexcept;

  constexpr void const* data() const noexcept { return data_; }

  constexpr size_type size_bytes() const noexcept { return size_bytes_; }

  constexpr destructor_type destructor() const noexcept { return destructor_; }

private:
  void const* data_ = nullptr;
  size_type size_bytes_ = {};
  destructor_type destructor_ = transient;
};

using user_blob = basic_user_blob<int>;


// =============================================================================


template <typename Size>
template <typename T, auto N>
basic_user_blob<Size>::basic_user_blob(
  std::array<T, N> const& data,
  destructor_type const destructor) noexcept :
  data_ {data.data()},
  size_bytes_ {static_cast<size_type>(N * sizeof(T))},
  destructor_ {destructor}
{
}

template <typename Size>
template <typename T, typename Allocator>
basic_user_blob<Size>::basic_user_blob(
  std::vector<T, Allocator> const& data,
  destructor_type const destructor) noexcept :
  data_ {reinterpret_cast<void const*>(data.data())},
  size_bytes_ {static_cast<size_type>(data.size() * sizeof(T))},
  destructor_ {destructor}
{
}

template <typename Size>
template <typename ContiguousItr>
basic_user_blob<Size>::basic_user_blob(
  ContiguousItr const begin,
  ContiguousItr const end,
  destructor_type const destructor) noexcept :
  data_ {reinterpret_cast<void const*>(std::addressof(*begin))},
  size_bytes_ {static_cast<size_type>(
    std::distance(begin, end)
    * sizeof(typename std::iterator_traits<ContiguousItr>::value_type))},
  destructor_ {destructor}
{
}

template <typename Size>
template <typename T>
basic_user_blob<Size>::basic_user_blob(
  T const* const data,
  size_type const size,
  destructor_type const destructor) noexcept :
  data_ {reinterpret_cast<void const*>(data)},
  size_bytes_ {static_cast<size_type>(size * sizeof(T))},
  destructor_ {destructor}
{
}


}; // namespace sqleger


#endif
