#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/user_text.hpp>


using namespace sqleger;

using namespace std::string_literals;
using namespace std::string_view_literals;


TEST_CASE("A user text can be constructed", "[user_text]")
{
  SECTION("C-style string, transient")
  {
    constexpr const char* cs = "asljdfkjsdjfaj sdfa;lsdjlfasdjf";

    const auto ut = user_text(cs);

    REQUIRE(ut.data() == cs);
    REQUIRE(ut.size_bytes() == -1);
    REQUIRE(ut.destructor() == transient);
  }

  SECTION("C-style string, static")
  {
    constexpr const char* cs = "DT0IE-  DFIJPI-A dfq-i o1-o324";

    const auto ut = user_text(cs, _static);

    REQUIRE(ut.data() == cs);
    REQUIRE(ut.size_bytes() == -1);
    REQUIRE(ut.destructor() == _static);
  }

  SECTION("pointer and size, transient")
  {
    constexpr auto ssv = "asljdfkjsdjfaj sdfa;lsdjlfasdjf"sv;

    const auto ut = user_text(ssv.data(), static_cast<int>(ssv.size()));

    REQUIRE(ut.data() == ssv.data());
    REQUIRE(ut.size_bytes() == static_cast<int>(ssv.size()));
    REQUIRE(ut.destructor() == transient);
  }

  SECTION("pointer and size, static")
  {
    constexpr auto ssv = "DT0IE-  DFIJPI-A dfq-i o1-o324"sv;

    const auto ut
      = user_text(ssv.data(), static_cast<int>(ssv.size()), _static);

    REQUIRE(ut.data() == ssv.data());
    REQUIRE(ut.size_bytes() == static_cast<int>(ssv.size()));
    REQUIRE(ut.destructor() == _static);
  }

  SECTION("std::string, transient")
  {
    const auto ss = "asljdfkjsdjfaj sdfa;lsdjlfasdjf"s;

    const auto ut = user_text(ss);

    REQUIRE(ut.data() == ss.c_str());
    REQUIRE(ut.size_bytes() == static_cast<int>(ss.size()));
    REQUIRE(ut.destructor() == transient);
  }

  SECTION("std::string, static")
  {
    const auto ss = "DT0IE-  DFIJPI-A dfq-i o1-o324"s;

    const auto ut = user_text(ss, _static);

    REQUIRE(ut.data() == ss.c_str());
    REQUIRE(ut.size_bytes() == static_cast<int>(ss.size()));
    REQUIRE(ut.destructor() == _static);
  }

  SECTION("std::string_view, transient")
  {
    const auto ssv = "asljdfkjsdjfaj sdfa;lsdjlfasdjf"sv;

    const auto ut = user_text(ssv);

    REQUIRE(ut.data() == ssv.data());
    REQUIRE(ut.size_bytes() == static_cast<int>(ssv.size()));
    REQUIRE(ut.destructor() == transient);
  }

  SECTION("std::string_view, static")
  {
    const auto ssv = "DT0IE-  DFIJPI-A dfq-i o1-o324"sv;

    const auto ut = user_text(ssv, _static);

    REQUIRE(ut.data() == ssv.data());
    REQUIRE(ut.size_bytes() == static_cast<int>(ssv.size()));
    REQUIRE(ut.destructor() == _static);
  }
}
