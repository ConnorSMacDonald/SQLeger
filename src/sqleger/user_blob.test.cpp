#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/sqleger.hpp>

#include <cstdint>


using namespace sqleger;


TEST_CASE("A user blob can be constructed", "[user_blob]")
{
  SECTION("array, transient")
  {
    constexpr std::array<uint64_t, 10> a {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto const ub = user_blob(a);

    REQUIRE(static_cast<uint64_t const*>(ub.data()) == a.data());
    REQUIRE(ub.size_bytes() == 80);
    REQUIRE(ub.destructor() == transient);
  }

  SECTION("array, static")
  {
    constexpr std::array<uint64_t, 10> a {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto const ub = user_blob(a, _static);

    REQUIRE(static_cast<uint64_t const*>(ub.data()) == a.data());
    REQUIRE(ub.size_bytes() == 80);
    REQUIRE(ub.destructor() == _static);
  }

  SECTION("vector, transient")
  {
    std::vector<uint64_t> const v {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto const ub = user_blob(v);

    REQUIRE(static_cast<uint64_t const*>(ub.data()) == v.data());
    REQUIRE(ub.size_bytes() == 80);
    REQUIRE(ub.destructor() == transient);
  }

  SECTION("vector, static")
  {
    std::vector<uint64_t> const v {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto const ub = user_blob(v, _static);

    REQUIRE(static_cast<uint64_t const*>(ub.data()) == v.data());
    REQUIRE(ub.size_bytes() == 80);
    REQUIRE(ub.destructor() == _static);
  }

  SECTION("continguous iterator, transient")
  {
    std::vector<uint64_t> const v {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto const ub = user_blob(v.begin(), v.end());

    REQUIRE(static_cast<uint64_t const*>(ub.data()) == v.data());
    REQUIRE(ub.size_bytes() == 80);
    REQUIRE(ub.destructor() == transient);
  }

  SECTION("continguous iterator, static")
  {
    std::vector<uint64_t> const v {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto const ub = user_blob(v.begin(), v.end(), _static);

    REQUIRE(static_cast<uint64_t const*>(ub.data()) == v.data());
    REQUIRE(ub.size_bytes() == 80);
    REQUIRE(ub.destructor() == _static);
  }

  SECTION("data and pointer, transient")
  {
    std::vector<uint64_t> const v {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto const ub = user_blob(v.data(), static_cast<int>(v.size()));

    REQUIRE(static_cast<uint64_t const*>(ub.data()) == v.data());
    REQUIRE(ub.size_bytes() == 80);
    REQUIRE(ub.destructor() == transient);
  }

  SECTION("data and pointer, static")
  {
    std::vector<uint64_t> const v {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto const ub = user_blob(v.data(), static_cast<int>(v.size()), _static);

    REQUIRE(static_cast<uint64_t const*>(ub.data()) == v.data());
    REQUIRE(ub.size_bytes() == 80);
    REQUIRE(ub.destructor() == _static);
  }
}
