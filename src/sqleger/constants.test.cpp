#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/constants.hpp>


using namespace sqleger;


TEST_CASE("An enum can be converted to and from an integer", "[constants]")
{
  REQUIRE(result::perm == int_to_enum<result>(enum_to_int(result::perm)));
}

TEST_CASE("A set of enums can be converted to a bit mask", "[constants]")
{
  constexpr auto f1 = flags({open::create, open::uri});

  REQUIRE(f1 == static_cast<open>(0x00000044));

  constexpr auto f2 = flags<open>();

  REQUIRE(f2 == static_cast<open>(0));
}

TEST_CASE("A error string for a result code can be retrieved", "[constants]")
{
  const auto* const cs = sqlite3_errstr(SQLITE_NOMEM);
  const auto zv = errstr(result::nomem);

  REQUIRE(zv.c_str() == cs);
}

TEST_CASE("The primary result code of an extended result code can be retrieved",
          "[constants]")
{
  REQUIRE(primary_result(result::ok) == result::ok);
  REQUIRE(primary_result(result::ok_load_permanently) == result::ok);
  REQUIRE(primary_result(result::error_retry) == result::error);
}

TEST_CASE("A result code can be queried to see if it is an error code",
          "[constants]")
{
  REQUIRE_FALSE(is_error(result::ok));
  REQUIRE(is_non_error(result::ok));

  REQUIRE_FALSE(is_error(result::row));
  REQUIRE(is_non_error(result::row));

  REQUIRE_FALSE(is_error(result::done));
  REQUIRE(is_non_error(result::done));

  REQUIRE_FALSE(is_error(result::ok_load_permanently));
  REQUIRE(is_non_error(result::ok_load_permanently));

  REQUIRE(is_error(result::error));
  REQUIRE_FALSE(is_non_error(result::error));

  REQUIRE(is_error(result::ioerr_read));
  REQUIRE_FALSE(is_non_error(result::ioerr_read));
}
