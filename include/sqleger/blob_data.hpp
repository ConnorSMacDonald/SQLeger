#ifndef SQLEGER_BLOB_DATA_HPP
#define SQLEGER_BLOB_DATA_HPP

#include <array>
#include <cstddef>
#include <vector>


namespace sqleger {


class blob_data {

public:
  inline blob_data(void const* ptr) noexcept;

  template <typename T>
  T const* as_pointer() const noexcept;

  template <typename T, typename Allocator = std::allocator<T>>
  std::vector<T, Allocator> as_vector(std::size_t size,
                                      Allocator const& allocator
                                      = Allocator()) const noexcept;

  template <typename T, std::size_t N>
  std::array<T, N> as_array() const noexcept;

  constexpr void const* ptr() const noexcept { return ptr_; }

private:
  void const* ptr_ = nullptr;
};


// =============================================================================


blob_data::blob_data(void const* const ptr) noexcept : ptr_ {ptr} {}

template <typename T>
T const* blob_data::as_pointer() const noexcept
{
  return static_cast<T const*>(ptr_);
}

template <typename T, typename Allocator>
std::vector<T, Allocator>
blob_data::as_vector(std::size_t const size,
                     Allocator const& allocator) const noexcept
{
  auto v = std::vector<T, Allocator>(size, allocator);

  std::memcpy(static_cast<void*>(v.data()), ptr_, size * sizeof(T));

  return v;
}

template <typename T, std::size_t N>
std::array<T, N> blob_data::as_array() const noexcept
{
  auto a = std::array<T, N>();

  std::memcpy(static_cast<void*>(a.data()), ptr_, N * sizeof(T));

  return a;
}


} // namespace sqleger


#endif
