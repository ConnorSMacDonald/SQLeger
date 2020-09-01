#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/sqleger.hpp>

#include <array>
#include <cstring>


using namespace sqleger;
using namespace sqleger::zstring_view_literals;
using namespace sqleger::string_span_literals;

using namespace std::string_literals;
using namespace std::string_view_literals;


TEST_CASE("A zstring view can be constructed from a C-style string", "[string]")
{
  constexpr const auto* cs = "SQLeger zstring";
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

TEST_CASE("A zstring view can be created using a literal", "[string]")
{
  constexpr const auto* cs = "literally a zstring";
  constexpr auto zv = operator""_zv(cs, std::char_traits<char>::length(cs));

  REQUIRE(zv.c_str() == cs);
}

TEST_CASE("A zstring view can be converted to a C-style string", "[string]")
{
  constexpr auto zv = "this string ends with zero"_zv;
  constexpr const auto* cs = static_cast<const char*>(zv);

  REQUIRE(cs == zv.c_str());
}

TEST_CASE("A zstring view can be converted to a standard string", "[string]")
{
  constexpr auto zv = "null terminated strings are a hack"_zv;
  const auto stds = static_cast<std::string>(zv);

  REQUIRE(stds == zv.c_str());
}

TEST_CASE("A zstring view can be converted to a standard string view",
          "[string]")
{
  constexpr auto zv = "I can't wait to be a std::string_view"_zv;
  constexpr auto stdsv = static_cast<std::string_view>(zv);

  REQUIRE(stdsv.data() == zv.c_str());
}

TEST_CASE("Two zstring views can be compared", "[string]")
{
  SECTION("equality")
  {
    REQUIRE("abc"_zv == "abc"_zv);
    REQUIRE_FALSE("abc"_zv != "abc"_zv);

    REQUIRE_FALSE("abc"_zv == "abd"_zv);
    REQUIRE("abc"_zv != "abd"_zv);

    REQUIRE_FALSE("abd"_zv == "abc"_zv);
    REQUIRE("abd"_zv != "abc"_zv);

    REQUIRE_FALSE("abc "_zv == "abc"_zv);
    REQUIRE("abc "_zv != "abc"_zv);

    REQUIRE_FALSE("abc"_zv == "abc "_zv);
    REQUIRE("abc"_zv != "abc "_zv);
  }

  SECTION("order")
  {
    REQUIRE_FALSE("abc"_zv < "abc"_zv);
    REQUIRE("abc"_zv <= "abc"_zv);
    REQUIRE_FALSE("abc"_zv > "abc"_zv);
    REQUIRE("abc"_zv >= "abc"_zv);

    REQUIRE("abc"_zv < "abd"_zv);
    REQUIRE("abc"_zv <= "abd"_zv);
    REQUIRE_FALSE("abc"_zv > "abd"_zv);
    REQUIRE_FALSE("abc"_zv >= "abd"_zv);

    REQUIRE("abc"_zv < "abc "_zv);
    REQUIRE("abc"_zv <= "abc "_zv);
    REQUIRE_FALSE("abc"_zv > "abc "_zv);
    REQUIRE_FALSE("abc"_zv >= "abc "_zv);

    REQUIRE_FALSE("abd"_zv < "abc"_zv);
    REQUIRE_FALSE("abd"_zv <= "abc"_zv);
    REQUIRE("abd"_zv > "abc"_zv);
    REQUIRE("abd"_zv >= "abc"_zv);

    REQUIRE_FALSE("abc "_zv < "abc"_zv);
    REQUIRE_FALSE("abc "_zv <= "abc"_zv);
    REQUIRE("abc "_zv > "abc"_zv);
    REQUIRE("abc "_zv >= "abc"_zv);
  }
}

TEST_CASE("A UTF8 encoded zstring view can be converted to a standard string",
          "[string]")
{
  constexpr std::array<unsigned char, 4> a {static_cast<unsigned char>('a'),
                                            static_cast<unsigned char>('b'),
                                            static_cast<unsigned char>('c'),
                                            static_cast<unsigned char>('\0')};
  const auto uzv = uzstring_view(a.data());
  const auto stds = utf8_to_ascii(uzv);

  REQUIRE(stds == "abc");
}

TEST_CASE("A string span can be constructed from a C-style", "[string]")
{
  constexpr const auto* cs = "foo bar baz something something";
  const auto ss = string_span(cs);

  REQUIRE(ss.size() == string_span::zstring_size);
  REQUIRE(ss.data() == cs);
}

TEST_CASE("A string span can be constructed from a char pointer and a size",
          "[string]")
{
  constexpr const auto* cs = "ihg avsa sa sa ssasAS da dfasdfasd a";
  const auto ss = string_span(cs, 7);

  REQUIRE(ss.size() == 7);
  REQUIRE(ss.data() == cs);
}

TEST_CASE("A string span can be constructed from a zstring view", "[string]")
{
  constexpr const auto* cs = "l;jj;l HND vamlfsv m;la/dgfa423e985r3==--0)()&";
  constexpr auto zv = zstring_view(cs);
  constexpr auto ss = string_span(zv);

  REQUIRE(ss.size() == string_span::zstring_size);
  REQUIRE(ss.data() == cs);
}

TEST_CASE("A string span can be constructed from a zstring view and a size",
          "[string]")
{
  constexpr const auto* cs = "l;jj;l HND vamlfsv m;la/dgfa423e985r3==--0)()&";
  constexpr auto zv = zstring_view(cs);
  constexpr auto ss = string_span(zv, 4);

  REQUIRE(ss.size() == 4);
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

TEST_CASE("A string span can be created using a literal", "[string]")
{
  constexpr const auto* cs = "literally a string span";
  constexpr auto ss = operator""_ss(cs, std::char_traits<char>::length(cs));

  REQUIRE(ss.size() == std::char_traits<char>::length(cs));
  REQUIRE(ss.data() == cs);
}

TEST_CASE("A string span can be converted to standard string", "[string]")
{
  SECTION("null-terminated")
  {
    constexpr auto ss = "asdlvasdf jasd dasfjasdf0000"_ss;
    const auto stds = static_cast<std::string>(ss);

    REQUIRE(stds.size() == static_cast<int>(ss.size()));
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
    constexpr auto ss = "asdlvasdf jasd dasfjasdf0000"_ss;
    const auto stdsv = static_cast<std::string_view>(ss);

    REQUIRE(stdsv.size() == static_cast<int>(ss.size()));
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

TEST_CASE("Two string spans can be compared", "[string]")
{
  SECTION("equality")
  {
    REQUIRE("abc"_ss == "abc"_ss);
    REQUIRE_FALSE("abc"_ss != "abc"_ss);

    REQUIRE_FALSE("abc"_ss == "abd"_ss);
    REQUIRE("abc"_ss != "abd"_ss);

    REQUIRE_FALSE("abd"_ss == "abc"_ss);
    REQUIRE("abd"_ss != "abc"_ss);

    REQUIRE_FALSE("abc "_ss == "abc"_ss);
    REQUIRE("abc "_ss != "abc"_ss);

    REQUIRE_FALSE("abc"_ss == "abc "_ss);
    REQUIRE("abc"_ss != "abc "_ss);
  }

  SECTION("order")
  {
    REQUIRE_FALSE("abc"_ss < "abc"_ss);
    REQUIRE("abc"_ss <= "abc"_ss);
    REQUIRE_FALSE("abc"_ss > "abc"_ss);
    REQUIRE("abc"_ss >= "abc"_ss);

    REQUIRE("abc"_ss < "abd"_ss);
    REQUIRE("abc"_ss <= "abd"_ss);
    REQUIRE_FALSE("abc"_ss > "abd"_ss);
    REQUIRE_FALSE("abc"_ss >= "abd"_ss);

    REQUIRE("abc"_ss < "abc "_ss);
    REQUIRE("abc"_ss <= "abc "_ss);
    REQUIRE_FALSE("abc"_ss > "abc "_ss);
    REQUIRE_FALSE("abc"_ss >= "abc "_ss);

    REQUIRE_FALSE("abd"_ss < "abc"_ss);
    REQUIRE_FALSE("abd"_ss <= "abc"_ss);
    REQUIRE("abd"_ss > "abc"_ss);
    REQUIRE("abd"_ss >= "abc"_ss);

    REQUIRE_FALSE("abc "_ss < "abc"_ss);
    REQUIRE_FALSE("abc "_ss <= "abc"_ss);
    REQUIRE("abc "_ss > "abc"_ss);
    REQUIRE("abc "_ss >= "abc"_ss);
  }
}

TEST_CASE("A UTF8 encoded string span can be converted to a standard string",
          "[string]")
{
  SECTION("null-terminated")
  {
    constexpr std::array<unsigned char, 4> a {static_cast<unsigned char>('a'),
                                              static_cast<unsigned char>('b'),
                                              static_cast<unsigned char>('c'),
                                              static_cast<unsigned char>('\0')};
    const auto ss = ustring_span(a.data());
    const auto stds = utf8_to_ascii(ss);

    REQUIRE(stds == "abc");
  }

  SECTION("not null-terminated")
  {
    constexpr std::array<unsigned char, 3> a {static_cast<unsigned char>('a'),
                                              static_cast<unsigned char>('b'),
                                              static_cast<unsigned char>('c')};
    const auto ss = ustring_span(a.data(), 3);
    const auto stds = utf8_to_ascii(ss);

    REQUIRE(stds == "abc");
  }
}
