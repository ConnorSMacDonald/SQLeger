#ifndef SQLEGER_TYPE_ERASURE_HPP_INCLUDED
#define SQLEGER_TYPE_ERASURE_HPP_INCLUDED

#include <memory>
#include <type_traits>


namespace sqleger {


class typeless {

public:
  constexpr typeless() noexcept = default;

  template <typename T>
  typeless(T* ptr) noexcept;

  template <typename T>
  typeless(T& ref) noexcept;

  constexpr typeless(void* ptr) noexcept;

  template <typename T>
  T as() const noexcept;

  void* ptr() const noexcept { return ptr_; }

private:
  void* ptr_ = nullptr;
};


template <typename T>
void* erase_type(T* ptr) noexcept;

template <typename T>
void* erase_type(T& ref) noexcept;

template <typename T>
T infer_type(void* type_erased) noexcept;


// =============================================================================


template <typename T>
typeless::typeless(T* const ptr) noexcept : ptr_ {erase_type(ptr)}
{
}

template <typename T>
typeless::typeless(T& ref) noexcept : ptr_ {erase_type(ref)}
{
}

constexpr typeless::typeless(void* const ptr) noexcept : ptr_ {ptr} {}

template <typename T>
T typeless::as() const noexcept
{
  return infer_type<T>(ptr_);
}

template <typename T>
void* erase_type(T* const ptr) noexcept
{
  return reinterpret_cast<void*>(ptr);
}

template <typename T>
void* erase_type(T& ref) noexcept
{
  return erase_type(std::addressof(ref));
}

template <typename T>
T infer_type(void* const ptr) noexcept
{
  if constexpr (std::is_pointer_v<T>)
    return reinterpret_cast<T>(ptr);
  else if constexpr (std::is_lvalue_reference_v<T>)
    return *reinterpret_cast<std::add_pointer_t<T>>(ptr);
}


}; // namespace sqleger


#endif
