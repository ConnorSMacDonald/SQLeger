#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/db.hpp>
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
  db d;
  const auto r = db::open("/", d);

  REQUIRE(r != result_t::ok);

  const auto msg = static_cast<std::string>(d.errmsg());

  try
  {
    throw open_exception(r, std::move(d));
  }
  catch (const open_exception& e)
  {
    REQUIRE(e.code() == r);
    REQUIRE(e.what() == msg);
  }
}
