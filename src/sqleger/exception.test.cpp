#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/exception.hpp>


using namespace sqleger;


TEST_CASE("A result exception can be thrown and caught", "[exception]")
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

TEST_CASE("An open exception can be thrown and caught", "[exception]")
{
  try
  {
    throw open_exception(result_t::error, "uh oh spaghettio");
  }
  catch (const open_exception& e)
  {
    REQUIRE(e.code() == result_t::error);
    REQUIRE(e.what() == "uh oh spaghettio");
  }

  try
  {
    throw open_exception(result_t::error, "uh oh spaghettio");
  }
  catch (const result_exception& e)
  {
    REQUIRE(e.code() == result_t::error);
    REQUIRE(e.what() == "uh oh spaghettio");
  }
}
