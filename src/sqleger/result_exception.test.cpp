#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/result_exception.hpp>


using namespace sqleger;


TEST_CASE("A result exception can be thrown and caught", "[result_exception]")
{
  try
  {
    throw result_exception(result_t::error);
  }
  catch (const result_exception& e)
  {
    REQUIRE(e.code() == result_t::error);
    REQUIRE(e.what() == errstr(result_t::error).c_str());
  }
}
