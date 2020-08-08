#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/string.hpp>


using namespace sqleger;


TEST_CASE("A zstring view can be constructed from a C-style string", "[string]")
{
  constexpr auto* const cs = "SQLeger zstring";
  constexpr auto zv = zstring_view(cs);

  REQUIRE(zv.c_str() == cs);
}

TEST_CASE("A zstring view can be constructed from a standard string",
          "[string]")
{
  const auto ss = std::string("std::string");
  const auto zv = zstring_view(ss);

  REQUIRE(zv.c_str() == ss.c_str());
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
  const auto ss = static_cast<std::string>(zv);

  REQUIRE(ss == zv.c_str());
}

TEST_CASE("A zstring view can be converted to a standard string view",
          "[string]")
{
  constexpr auto zv = zstring_view("I can't wait to be a std::string_view");
  constexpr auto ssv = static_cast<std::string_view>(zv);

  REQUIRE(ssv.data() == zv.c_str());
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
