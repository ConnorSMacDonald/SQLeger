#ifndef SQLEGER_BOOL_OUT_HPP_INCLUDED
#define SQLEGER_BOOL_OUT_HPP_INCLUDED


namespace sqleger {


class bool_out {

public:
  static inline int* to_int_ptr(bool_out* ptr) noexcept;

  constexpr bool_out() noexcept = default;

  constexpr bool_out(bool value) noexcept;

  constexpr operator bool() const noexcept;

private:
  int value_ = static_cast<int>(false);
};


// ============================================================================


int* bool_out::to_int_ptr(bool_out* const ptr) noexcept
{
  return reinterpret_cast<int*>(ptr);
}

constexpr bool_out::bool_out(const bool value) noexcept :
  value_ {static_cast<int>(value)}
{
}

constexpr bool_out::operator bool() const noexcept
{
  return static_cast<bool>(value_);
}


}; // namespace sqleger


#endif
