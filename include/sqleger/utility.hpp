#ifndef SQLEGER_UTILITY_HPP
#define SQLEGER_UTILITY_HPP


namespace sqleger {


template <typename T>
constexpr T* exchange_nullptr(T*& ptr) noexcept;


// =============================================================================


template <typename T>
constexpr T* exchange_nullptr(T*& ptr) noexcept
{
  T* const old = ptr;
  ptr = nullptr;
  return old;
}


} // namespace sqleger


#endif
