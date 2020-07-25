#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/zstring.hpp>

#include <cstring>
#include <memory>


using namespace sqleger;

using namespace std::string_literals;
using namespace std::string_view_literals;


TEST_CASE("A default or nullptr initialized zstring view implicitly converts "
          "to false and is equal to nullptr",
          "[zstring_view]")
{
  const zstring_view zv1 {};

  REQUIRE_FALSE(static_cast<bool>(zv1));
  REQUIRE(zv1 == nullptr);
  REQUIRE_FALSE(zv1 != nullptr);
  REQUIRE(nullptr == zv1);
  REQUIRE_FALSE(nullptr != zv1);

  const zstring_view zv2 {nullptr};

  REQUIRE_FALSE(static_cast<bool>(zv2));
  REQUIRE(zv2 == nullptr);
  REQUIRE_FALSE(zv2 != nullptr);
  REQUIRE(nullptr == zv2);
  REQUIRE_FALSE(nullptr != zv2);
}

TEST_CASE("A zstring view can be constructed with a C-style string",
          "[zstring_view]")
{
  const zstring_view zv1 {"test string"};

  REQUIRE(std::strcmp(zv1, "test string") == 0);
}

TEST_CASE("A zstring view can be constructed with a standard string",
          "[zstring_view]")
{
  const auto s1 = "test std::string"s;
  const zstring_view zv1 {s1};

  REQUIRE(zv1.c_str() == s1.c_str());
}

TEST_CASE("A zstring view can be converted to a standard string",
          "[zstring_view]")
{
  const zstring_view zv1 {"some text"};

  const auto s1 = zv1.to_std_string(std::allocator<char>());

  REQUIRE(s1 == "some text"s);

  const auto s2 = static_cast<std::string>(zv1);

  REQUIRE(s2 == "some text"s);
}

TEST_CASE("A zstring view can be converted to a standard string view",
          "[zstring_view]")
{
  const zstring_view zv1 {"some more text"};

  const auto sv1 = zv1.to_std_string_view();

  REQUIRE(sv1 == "some more text"sv);

  const auto sv2 = static_cast<std::string_view>(zv1);

  REQUIRE(sv2 == "some more text"sv);
}

TEST_CASE("A UTF16 zstring view can be converted to and from a void pointer",
          "[zstring]")
{
  const zstring16_view zv1 {u"SQLite's evil void* UTF16 rep"};

  const void* const vp1 = to_void_ptr(zv1);

  const auto zv2 = to_zstring16_view(vp1);

  REQUIRE(zv1.c_str() == zv2.c_str());
}
