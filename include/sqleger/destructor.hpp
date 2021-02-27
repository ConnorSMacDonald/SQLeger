#ifndef SQLEGER_DESTRUCTOR_HPP
#define SQLEGER_DESTRUCTOR_HPP

#include <sqlite3.h>


namespace sqleger {


using destructor_type = sqlite3_destructor_type;

static auto const _static = SQLITE_STATIC;
static auto const transient = SQLITE_TRANSIENT;


} // namespace sqleger


#endif
