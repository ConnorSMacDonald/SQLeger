#ifndef SQLEGER_BLOB_DATA_HPP
#define SQLEGER_BLOB_DATA_HPP

#include <array>
#include <cstddef>
#include <vector>


namespace sqleger {


class blob_data {

public:
  inline blob_data(const void* ptr) noexcept;

  template <typename T>
  const T* as_pointer() const noexcept;

  template <typename T, typename Allocator = std::allocator<T>>
  std::vector<T, Allocator>
  as_vector(std::size_t size, const Allocator& allocator = Allocator()) const
    noexcept;

  template <typename T, std::size_t N>
  std::array<T, N> as_array() const noexcept;

  constexpr const std::byte* ptr() const noexcept { return ptr_; }

private:
  const std::byte* ptr_ = nullptr;
};


// =============================================================================


blob_data::blob_data(const void* const ptr) noexcept :
  ptr_ {reinterpret_cast<const std::byte*>(ptr)}
{
}

template <typename T>
const T* blob_data::as_pointer() const noexcept
{
  return reinterpret_cast<const T*>(ptr_);
}

template <typename T, typename Allocator>
std::vector<T, Allocator> blob_data::as_vector(const std::size_t size,
                                               const Allocator& allocator) const
  noexcept
{
  auto v = std::vector<T, Allocator>(size, allocator);
  std::memcpy(reinterpret_cast<void*>(v.data()),
              reinterpret_cast<const void*>(ptr_),
              size * sizeof(T));
  return v;
}

template <typename T, std::size_t N>
std::array<T, N> blob_data::as_array() const noexcept
{
  auto a = std::array<T, N>();
  std::memcpy(reinterpret_cast<void*>(a.data()),
              reinterpret_cast<const void*>(ptr_),
              N * sizeof(T));
  return a;
}


}; // namespace sqleger


#endif
