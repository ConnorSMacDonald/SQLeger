#ifndef SQLEGER_TYPE_ERASURE_HPP_INCLUDED
#define SQLEGER_TYPE_ERASURE_HPP_INCLUDED

#include <type_traits>


namespace sqleger {


class typeless {

public:
  constexpr typeless() noexcept = default;

  typeless(void* ptr) noexcept;

  template <typename T>
  typeless(T&& value) noexcept;

  template <typename T>
  T as() const noexcept;

  void* ptr() const noexcept { return ptr_; }

private:
  void* ptr_ = nullptr;
};


template <typename T>
void* erase_type(T&& value) noexcept;

template <typename T>
T infer_type(void* type_erased) noexcept;

template <typename T>
void delete_type_erased(void* type_erased) noexcept;


// =============================================================================


constexpr typeless::typeless(void* const ptr) noexcept : ptr_ {ptr} {}

template <typename T>
typeless::typeless(T&& value) noexcept :
  ptr_ {erase_type(std::forward<T>(value))}
{
}

template <typename T>
T typeless::as() const noexcept
{
  return infer_type<T>(ptr_);
}


template <typename T>
void* erase_type(T&& value) noexcept
{
  if constexpr (std::is_pointer_v<T>)
    return reinterpret_cast<void*>(value);
  else if constexpr (std::is_lvalue_reference_v<T>)
    return reinterpret_cast<void*>(&value);
}

template <typename T>
T infer_type(void* const ptr) noexcept
{
  if constexpr (std::is_pointer_v<T>)
    return reinterpret_cast<T>(ptr);
  else if constexpr (std::is_lvalue_reference_v<T>)
    return *reinterpret_cast<add_pointer_t<remove_reference_t<T>>>(ptr);
}

template <typename T>
void delete_type_erased(void* const ptr) noexcept
{
  delete reinterpret_cast<T*>(type_erased);
}


}; // namespace sqleger


#endif
