#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/user_text.hpp>


using namespace sqleger;

using namespace std::string_literals;
using namespace std::string_view_literals;


TEST_CASE("A user text can be constructed", "[user_text]")
{
  SECTION("zstring, transient")
  {
    constexpr const char* cs = "asljdfkjsdjfaj sdfa;lsdjlfasdjf";

    const auto ut = user_text(cs);

    REQUIRE(ut.data() == cs);
    REQUIRE(ut.size_bytes() == user_text::zstring_size);
    REQUIRE(ut.destructor() == transient);
  }

  SECTION("zstring, static")
  {
    constexpr const char* cs = "asljdfkjsdjfaj sdfa;lsdjlfasdjf";

    const auto ut = user_text(cs, _static);

    REQUIRE(ut.data() == cs);
    REQUIRE(ut.size_bytes() == user_text::zstring_size);
    REQUIRE(ut.destructor() == _static);
  }

  SECTION("bounded string, transient")
  {
    const auto ssv = "asljdfkjsdjfaj sdfa;lsdjlfasdjf"sv;

    const auto ut = user_text(ssv);

    REQUIRE(ut.data() == ssv.data());
    REQUIRE(ut.size_bytes() == static_cast<int>(ssv.size()));
    REQUIRE(ut.destructor() == transient);
  }
}
