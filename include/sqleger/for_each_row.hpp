#ifndef SQLEGER_FOR_EACH_ROW_HPP
#define SQLEGER_FOR_EACH_ROW_HPP

#include <sqleger/row.hpp>


namespace sqleger {


template <typename Function>
result for_each_row(stmt_ref s, Function function);


// =============================================================================


template <typename Function>
result for_each_row(stmt_ref s, Function function)
{
  auto r = s.step();

  for (; r == result::row; r = s.step())
    function(row(s));

  return r;
}


} // namespace sqleger


#endif
