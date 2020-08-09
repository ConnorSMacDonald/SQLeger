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
