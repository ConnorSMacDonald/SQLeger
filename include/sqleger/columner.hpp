#ifndef SQLEGER_COLUMNER_HPP
#define SQLEGER_COLUMNER_HPP

#include <sqleger/column_traits.hpp>
#include <sqleger/row.hpp>

#include <tuple>


namespace sqleger {


class columner {

public:
  constexpr columner(row r) noexcept;

  constexpr columner(stmt_ref s) noexcept;

  template <typename ResultValue>
  ResultValue get() noexcept;

  template <typename... ResultValues,
            typename = std::enable_if_t<sizeof...(ResultValues) != 1>>
  std::tuple<ResultValues...> get() noexcept;

  constexpr row get_row() const noexcept { return row_; }

  constexpr int index() const noexcept { return index_; }

private:
  template <std::size_t I, typename... ResultValues>
  void column(std::tuple<ResultValues...>& result_values) noexcept;

  row row_;

  int index_ = -1;
};

template <typename ResultValue>
columner& operator>>(columner& c, ResultValue& result_value) noexcept;


template <typename ResultValue>
ResultValue column(row r) noexcept;

template <typename ResultValue>
ResultValue column(stmt_ref s) noexcept;

template <typename... ResultValues,
          typename = std::enable_if_t<sizeof...(ResultValues) != 1>>
std::tuple<ResultValues...> column(row r) noexcept;

template <typename... ResultValues,
          typename = std::enable_if_t<sizeof...(ResultValues) != 1>>
std::tuple<ResultValues...> column(stmt_ref s) noexcept;


// =============================================================================


constexpr columner::columner(row const r) noexcept : row_ {r} {}

constexpr columner::columner(stmt_ref const s) noexcept : columner {row(s)} {}

template <typename ResultValue>
ResultValue columner::get() noexcept
{
  auto result_value = std::tuple<ResultValue>();
  column<0>(result_value);
  return std::get<0>(result_value);
}

template <typename... ResultValues, typename>
std::tuple<ResultValues...> columner::get() noexcept
{
  auto result_values = std::tuple<ResultValues...>();
  column<0>(result_values);
  return result_values;
}

template <std::size_t I, typename... ResultValues>
void columner::column(std::tuple<ResultValues...>& result_values) noexcept
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

    auto v = row_.column_value(++index_);

    std::get<I>(result_values) = column_traits_t::from_value(v);

    return column<I + 1>(result_values);
  }
}

template <typename ResultValue>
columner& operator>>(columner& c, ResultValue& result_value) noexcept
{
  result_value = c.get<ResultValue>();
  return c;
}

template <typename ResultValue>
ResultValue column(row const r) noexcept
{
  return columner(r).get<ResultValue>();
}

template <typename ResultValue>
ResultValue column(stmt_ref const s) noexcept
{
  return column<ResultValue>(row(s));
}

template <typename... ResultValues, typename>
std::tuple<ResultValues...> column(row const r) noexcept
{
  return columner(r).get<ResultValues...>();
}

template <typename... ResultValues, typename>
std::tuple<ResultValues...> column(stmt_ref const s) noexcept
{
  return column<ResultValues...>(row(s));
}


} // namespace sqleger


#endif
