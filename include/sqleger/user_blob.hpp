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
  basic_user_blob(const std::array<T, N>& data,
                  destructor_type destructor = transient) noexcept;

  template <typename T>
  basic_user_blob(const std::vector<T>& data,
                  destructor_type destructor = transient) noexcept;

  template <typename ContiguousItr>
  basic_user_blob(ContiguousItr begin,
                  ContiguousItr end,
                  destructor_type destructor = transient) noexcept;

  template <typename T>
  basic_user_blob(const T* data,
                  size_type size,
                  destructor_type destructor = transient) noexcept;

  constexpr const void* data() const noexcept { return data_; }

  constexpr size_type size_bytes() const noexcept { return size_bytes_; }

  constexpr destructor_type destructor() const noexcept { return destructor_; }

private:
  const void* data_ = nullptr;
  size_type size_bytes_ = {};
  destructor_type destructor_ = transient;
};

using user_blob = basic_user_blob<int>;


// =============================================================================


template <typename Size>
template <typename T, auto N>
basic_user_blob<Size>::basic_user_blob(
  const std::array<T, N>& data,
  const destructor_type destructor) noexcept :
  data_ {data.data()},
  size_bytes_ {static_cast<size_type>(N * sizeof(T))},
  destructor_ {destructor}
{
}

template <typename Size>
template <typename T>
basic_user_blob<Size>::basic_user_blob(
  const std::vector<T>& data,
  const destructor_type destructor) noexcept :
  data_ {reinterpret_cast<const void*>(data.data())},
  size_bytes_ {static_cast<size_type>(data.size() * sizeof(T))},
  destructor_ {destructor}
{
}

template <typename Size>
template <typename ContiguousItr>
basic_user_blob<Size>::basic_user_blob(
  const ContiguousItr begin,
  const ContiguousItr end,
  const destructor_type destructor) noexcept :
  data_ {reinterpret_cast<const void*>(std::addressof(*begin))},
  size_bytes_ {static_cast<size_type>(
    std::distance(begin, end)
    * sizeof(typename std::iterator_traits<ContiguousItr>::value_type))},
  destructor_ {destructor}
{
}

template <typename Size>
template <typename T>
basic_user_blob<Size>::basic_user_blob(
  const T* const data,
  const size_type size,
  const destructor_type destructor) noexcept :
  data_ {reinterpret_cast<const void*>(data)},
  size_bytes_ {static_cast<size_type>(size * sizeof(T))},
  destructor_ {destructor}
{
}


}; // namespace sqleger


#endif
