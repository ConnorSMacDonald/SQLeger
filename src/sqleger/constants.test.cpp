#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/constants.hpp>


using namespace sqleger;


TEST_CASE("An enum can be converted to and from an integer", "[constants]")
{
  REQUIRE(result_t::perm == int_to_enum<result_t>(enum_to_int(result_t::perm)));
}

TEST_CASE("A set of enums can be converted to a bit mask", "[constants]")
{
  constexpr auto f1 = flags({open_t::create, open_t::uri});

  REQUIRE(f1 == static_cast<open_t>(0x00000044));

  constexpr auto f2 = flags<open_t>();

  REQUIRE(f2 == static_cast<open_t>(0));
}

TEST_CASE("A error string for a result code can be retrieved", "[constants]")
{
  const auto* const cs = sqlite3_errstr(SQLITE_NOMEM);
  const auto zv = errstr(result_t::nomem);

  REQUIRE(zv.c_str() == cs);
}

TEST_CASE("The primary result code of an extended result code can be retrieved",
          "[constants]")
{
  REQUIRE(primary_result(result_t::ok) == result_t::ok);
  REQUIRE(primary_result(result_t::ok_load_permanently) == result_t::ok);
  REQUIRE(primary_result(result_t::error_retry) == result_t::error);
}

TEST_CASE("A result code can be queried to see if it is an error code",
          "[constants]")
{
  REQUIRE_FALSE(is_error(result_t::ok));
  REQUIRE(is_non_error(result_t::ok));

  REQUIRE_FALSE(is_error(result_t::row));
  REQUIRE(is_non_error(result_t::row));

  REQUIRE_FALSE(is_error(result_t::done));
  REQUIRE(is_non_error(result_t::done));

  REQUIRE_FALSE(is_error(result_t::ok_load_permanently));
  REQUIRE(is_non_error(result_t::ok_load_permanently));

  REQUIRE(is_error(result_t::error));
  REQUIRE_FALSE(is_non_error(result_t::error));

  REQUIRE(is_error(result_t::ioerr_read));
  REQUIRE_FALSE(is_non_error(result_t::ioerr_read));
}
