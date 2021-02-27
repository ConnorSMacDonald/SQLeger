#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/sqleger.hpp>


using namespace sqleger;
using namespace sqleger::string_span_literals;
using namespace sqleger::zstring_view_literals;


TEST_CASE("A user text can be constructed", "[user_text]")
{
  SECTION("zstring, transient")
  {
    constexpr auto zv = "asljdfkjsdjfaj sdfa;lsdjlfasdjf"_zv;

    auto const ut = user_text(zv);

    REQUIRE(ut.data() == zv.c_str());
    REQUIRE(ut.size_bytes() == user_text::zstring_size);
    REQUIRE(ut.destructor() == transient);
  }

  SECTION("zstring, static")
  {
    constexpr auto zv = "asljdfkjsdjfaj sdfa;lsdjlfasdjf"_zv;

    auto const ut = user_text(zv, _static);

    REQUIRE(ut.data() == zv.c_str());
    REQUIRE(ut.size_bytes() == user_text::zstring_size);
    REQUIRE(ut.destructor() == _static);
  }

  SECTION("bounded string, transient")
  {
    auto const ss = "asljdfkjsdjfaj sdfa;lsdjlfasdjf"_ss;

    auto const ut = user_text(ss);

    REQUIRE(ut.data() == ss.data());
    REQUIRE(ut.size_bytes() == ss.size());
    REQUIRE(ut.destructor() == transient);
  }
}
