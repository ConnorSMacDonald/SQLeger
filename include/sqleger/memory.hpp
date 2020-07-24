#ifndef SQLEGER_MEMORY_HPP_INCLUDED
#define SQLEGER_MEMORY_HPP_INCLUDED

#include <sqleger/int.hpp>

#include <sqlite3.h>

#include <memory>


namespace sqleger {


template <typename T>
struct default_delete {
  void operator()(T* ptr) const noexcept;
};

template <typename T>
using unique_ptr = std::unique_ptr<T, default_delete<T>>;


// ============================================================================


template <typename T>
void default_delete::operator()(T* const ptr) const noexcept
{
  ::sqlite3_free(ptr);
}


}; // namespace sqleger


#endif
