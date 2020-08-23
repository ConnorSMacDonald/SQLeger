#ifndef SQLEGER_COLUMNER_HPP
#define SQLEGER_COLUMNER_HPP

#include <sqleger/column_traits.hpp>
#include <sqleger/stmt.hpp>

#include <tuple>


namespace sqleger {


class columner {

public:
  constexpr columner(stmt_ref statement) noexcept;

  template <typename ResultValue>
  ResultValue get() noexcept;

  template <typename... ResultValues,
            typename = std::enable_if_t<sizeof...(ResultValues) != 1>>
  std::tuple<ResultValues...> get() noexcept;

  stmt_ref get_stmt_ref() const noexcept { return stmt_ref_; }

  int index() const noexcept { return index_; }

private:
  template <std::size_t I, typename... ResultValues>
  void columns(std::tuple<ResultValues...>& result_values) noexcept;

  stmt_ref stmt_ref_;

  int index_ = -1;
};

template <typename ResultValue>
columner& operator>>(columner& c, ResultValue& result_value) noexcept;


template <typename... ResultValues>
std::tuple<ResultValues...> columns(stmt_ref statement) noexcept;


// =============================================================================


constexpr columner::columner(const stmt_ref statement) noexcept :
  stmt_ref_ {statement}
{
}

template <typename ResultValue>
ResultValue columner::get() noexcept
{
  auto result_value = std::tuple<ResultValue>();
  columns<0>(result_value);
  return std::get<0>(result_value);
}

template <typename... ResultValues, typename>
std::tuple<ResultValues...> columner::get() noexcept
{
  auto result_values = std::tuple<ResultValues...>();
  columns<0>(result_values);
  return result_values;
}

template <std::size_t I, typename... ResultValues>
void columner::columns(std::tuple<ResultValues...>& result_values) noexcept
{
  if constexpr (I >= sizeof...(ResultValues))
    return;
  else
  {
    using tuple_element_t
      = std::tuple_element_t<I, std::tuple<ResultValues...>>;
    using user_value_t
      = std::remove_volatile_t<std::remove_reference_t<tuple_element_t>>;
    using column_traits_t = column_traits<user_value_t>;

    auto v = stmt_ref_.column_value(++index_);

    std::get<I>(result_values) = column_traits_t::from_value(v);

    return columns<I + 1>(result_values);
  }
}

template <typename ResultValue>
columner& operator>>(columner& c, ResultValue& result_value) noexcept
{
  result_value = c.get<ResultValue>();
  return c;
}

template <typename... ResultValues>
std::tuple<ResultValues...> columns(const stmt_ref statement) noexcept
{
  return columner(statement).get<ResultValues...>();
}


}; // namespace sqleger


#endif
