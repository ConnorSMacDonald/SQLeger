#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/string.hpp>

#include <cstring>


using namespace sqleger;

using namespace std::string_literals;
using namespace std::string_view_literals;


TEST_CASE("A zstring view can be constructed from a C-style string", "[string]")
{
  constexpr auto* const cs = "SQLeger zstring";
  constexpr auto zv = zstring_view(cs);

  REQUIRE(zv.c_str() == cs);
}

TEST_CASE("A zstring view can be constructed from a standard string",
          "[string]")
{
  const auto stds = std::string("std::string");
  const auto zv = zstring_view(stds);

  REQUIRE(zv.c_str() == stds.c_str());
}

TEST_CASE("A zstring view can be converted to a C-style string", "[string]")
{
  constexpr auto zv = zstring_view("this string ends with zero");
  constexpr auto* const cs = static_cast<const char*>(zv);

  REQUIRE(cs == zv.c_str());
}

TEST_CASE("A zstring view can be converted to a standard string", "[string]")
{
  constexpr auto zv = zstring_view("null terminated strings are a hack");
  const auto stds = static_cast<std::string>(zv);

  REQUIRE(stds == zv.c_str());
}

TEST_CASE("A zstring view can be converted to a standard string view",
          "[string]")
{
  constexpr auto zv = zstring_view("I can't wait to be a std::string_view");
  constexpr auto stdsv = static_cast<std::string_view>(zv);

  REQUIRE(stdsv.data() == zv.c_str());
}

TEST_CASE("A zstring view reference or pointer can converted to a pointer to a "
          "C-style string",
          "[string]")
{
  auto zv = zstring_view("Pointer inconvertibility is cool");

  const auto** const cs1 = to_c_str_ptr(zv);

  REQUIRE(*cs1 == zv.c_str());

  const auto** const cs2 = to_c_str_ptr(&zv);

  REQUIRE(*cs2 == zv.c_str());
}

TEST_CASE("A string span can be constructed from a C-style", "[string]")
{
  constexpr auto* const cs = "foo bar baz something something";
  const auto ss = string_span(cs);

  REQUIRE(ss.size() == string_span::zstring_size);
  REQUIRE(ss.data() == cs);
}

TEST_CASE("A string span can be constructed from a char pointer and a size",
          "[string]")
{
  constexpr auto* const cs = "ihg avsa sa sa ssasAS da dfasdfasd a";
  const auto ss = string_span(cs, 7);

  REQUIRE(ss.size() == 7);
  REQUIRE(ss.data() == cs);
}

TEST_CASE("A string span can be constructed from a standard string", "[string]")
{
  const auto stds = "jljj asjkd fjklasdfj sajl;ASDF ASD"s;
  const auto ss = string_span(stds);

  REQUIRE(ss.size() == static_cast<string_span::size_type>(stds.size()));
  REQUIRE(ss.data() == stds.c_str());
}

TEST_CASE("A string span can be constructed from a standard string view",
          "[string]")
{
  const auto stdsv = "jlasvjklsvjksv jalksdv jvdsajdsva;jdls"sv;
  const auto ss = string_span(stdsv);

  REQUIRE(ss.size() == static_cast<string_span::size_type>(stdsv.size()));
  REQUIRE(ss.data() == stdsv.data());
}

TEST_CASE("A string span can be converted to standard string", "[string]")
{
  SECTION("null-terminated")
  {
    constexpr auto* const cs = "asdlvasdf jasd dasfjasdf0000";
    const auto ss = string_span(cs);
    const auto stds = static_cast<std::string>(ss);

    REQUIRE(stds.size() == std::strlen(cs));
    REQUIRE(stds == ss.data());
  }

  SECTION("not null-terminated")
  {
    const auto size = 4;
    const auto ss = string_span("asdlvasdf jasd dasfjasdf0000", size);
    const auto stds = static_cast<std::string>(ss);

    REQUIRE(stds.size() == size);
    REQUIRE(stds == std::string_view(ss.data(), size));
  }
}

TEST_CASE("A string span can be converted to a standard string view",
          "[string]")
{
  SECTION("null-terminated")
  {
    constexpr auto* const cs = "asdlvasdf jasd dasfjasdf0000";
    const auto ss = string_span(cs);
    const auto stdsv = static_cast<std::string_view>(ss);

    REQUIRE(stdsv.size() == std::strlen(cs));
    REQUIRE(stdsv.data() == ss.data());
  }

  SECTION("not null-terminated")
  {
    const auto size = 4;
    const auto ss = string_span("asdlvasdf jasd dasfjasdf0000", size);
    const auto stdsv = static_cast<std::string_view>(ss);

    REQUIRE(stdsv.size() == size);
    REQUIRE(stdsv.data() == ss.data());
  }
}
