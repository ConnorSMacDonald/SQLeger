#ifndef SQLEGER_VALUE_HPP
#define SQLEGER_VALUE_HPP

#include <sqleger/blob_data.hpp>
#include <sqleger/constants.hpp>
#include <sqleger/int.hpp>
#include <sqleger/string.hpp>
#include <sqleger/utility.hpp>

#include <sqlite3.h>


namespace sqleger {


class value;


template <typename Impl>
class value_interface {

public:
  using impl_type = Impl;
  using c_type = ::sqlite3_value;

  blob_data blob() noexcept;

  double _double() noexcept;

  int _int() noexcept;

  int64 int64() noexcept;

  uzstring_view text() noexcept;

  int bytes() noexcept;

  datatype type() noexcept;

  value dup() const noexcept;

  constexpr operator bool() const noexcept;

private:
  constexpr c_type* c_ptr() const noexcept;
};


class value_ref;


class value : public value_interface<value> {

public:
  using interface_type = value_interface<value>;

  constexpr value() noexcept = default;

  explicit constexpr value(c_type* c_ptr) noexcept;

  constexpr value(value&& other) noexcept;

  inline value& operator=(value&& other) noexcept;

  inline ~value() noexcept;

  inline void free() noexcept;

  constexpr c_type* take_c_ptr() noexcept;

  constexpr c_type* c_ptr() const noexcept { return c_ptr_; }

  constexpr value_ref ref() noexcept;

  constexpr operator value_ref() noexcept;

private:
  inline void do_free() noexcept;

  c_type* c_ptr_ = nullptr;
};


class value_ref : public value_interface<value_ref> {

public:
  using interface_type = value_interface<value_ref>;

  constexpr value_ref() noexcept = default;

  explicit constexpr value_ref(c_type* c_ptr) noexcept;

  explicit constexpr value_ref(value& other) noexcept;

  constexpr c_type* c_ptr() const noexcept { return c_ptr_; }

private:
  c_type* c_ptr_ = nullptr;
};


// =============================================================================


template <typename Impl>
blob_data value_interface<Impl>::blob() noexcept
{
  return ::sqlite3_value_blob(c_ptr());
}

template <typename Impl>
double value_interface<Impl>::_double() noexcept
{
  return ::sqlite3_value_double(c_ptr());
}

template <typename Impl>
int value_interface<Impl>::_int() noexcept
{
  return ::sqlite3_value_int(c_ptr());
}

template <typename Impl>
int64 value_interface<Impl>::int64() noexcept
{
  return ::sqlite3_value_int64(c_ptr());
}

template <typename Impl>
uzstring_view value_interface<Impl>::text() noexcept
{
  return ::sqlite3_value_text(c_ptr());
}

template <typename Impl>
int value_interface<Impl>::bytes() noexcept
{
  return ::sqlite3_value_bytes(c_ptr());
}

template <typename Impl>
datatype value_interface<Impl>::type() noexcept
{
  return int_to_enum<datatype>(::sqlite3_value_type(c_ptr()));
}

template <typename Impl>
value value_interface<Impl>::dup() const noexcept
{
  return value(::sqlite3_value_dup(c_ptr()));
}

template <typename Impl>
constexpr value_interface<Impl>::operator bool() const noexcept
{
  return c_ptr() != nullptr;
}

template <typename Impl>
constexpr auto value_interface<Impl>::c_ptr() const noexcept -> c_type*
{
  return static_cast<impl_type const*>(this)->c_ptr();
}

constexpr value::value(c_type* const c_ptr) noexcept : c_ptr_ {c_ptr} {}

constexpr value::value(value&& other) noexcept : c_ptr_ {other.take_c_ptr()} {}

value& value::operator=(value&& other) noexcept
{
  if (this == &other)
    return *this;

  do_free();

  c_ptr_ = other.take_c_ptr();

  return *this;
}

value::~value() noexcept
{
  do_free();
}

void value::free() noexcept
{
  do_free();
  c_ptr_ = nullptr;
}

constexpr auto value::take_c_ptr() noexcept -> c_type*
{
  return exchange_nullptr(c_ptr_);
}

constexpr value_ref value::ref() noexcept
{
  return value_ref(*this);
}

constexpr value::operator value_ref() noexcept
{
  return ref();
}

void value::do_free() noexcept
{
  return ::sqlite3_value_free(c_ptr_);
}

constexpr value_ref::value_ref(c_type* const c_ptr) noexcept : c_ptr_ {c_ptr} {}

constexpr value_ref::value_ref(value& other) noexcept :
  value_ref {other.c_ptr()}
{
}


}; // namespace sqleger


#endif
