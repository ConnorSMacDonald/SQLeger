#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/sqleger.hpp>


using namespace sqleger;


TEST_CASE("A result exception can be thrown and caught", "[result_exception]")
{
  try
  {
    throw result_exception(result::error);
  }
  catch (result_exception const& e)
  {
    REQUIRE(e.code() == result::error);
    REQUIRE(e.what() == errstr(result::error).c_str());
  }
}
