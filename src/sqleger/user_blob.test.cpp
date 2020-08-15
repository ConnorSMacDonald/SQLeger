#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/user_blob.hpp>


using namespace sqleger;


TEST_CASE("A user blob can be constructed", "[user_blob]")
{
  SECTION("array, transient")
  {
    constexpr std::array<uint64_t, 10> a = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    const auto ub = user_blob(a);

    REQUIRE(reinterpret_cast<const uint64_t*>(ub.data()) == a.data());
    REQUIRE(ub.size_bytes() == 80);
    REQUIRE(ub.destructor() == transient);
  }

  SECTION("array, static")
  {
    constexpr std::array<uint64_t, 10> a = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    const auto ub = user_blob(a, _static);

    REQUIRE(reinterpret_cast<const uint64_t*>(ub.data()) == a.data());
    REQUIRE(ub.size_bytes() == 80);
    REQUIRE(ub.destructor() == _static);
  }

  SECTION("vector, transient")
  {
    const std::vector<uint64_t> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    const auto ub = user_blob(v);

    REQUIRE(reinterpret_cast<const uint64_t*>(ub.data()) == v.data());
    REQUIRE(ub.size_bytes() == 80);
    REQUIRE(ub.destructor() == transient);
  }

  SECTION("vector, static")
  {
    const std::vector<uint64_t> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    const auto ub = user_blob(v, _static);

    REQUIRE(reinterpret_cast<const uint64_t*>(ub.data()) == v.data());
    REQUIRE(ub.size_bytes() == 80);
    REQUIRE(ub.destructor() == _static);
  }

  SECTION("continguous iterator, transient")
  {
    const std::vector<uint64_t> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    const auto ub = user_blob(v.begin(), v.end());

    REQUIRE(reinterpret_cast<const uint64_t*>(ub.data()) == v.data());
    REQUIRE(ub.size_bytes() == 80);
    REQUIRE(ub.destructor() == transient);
  }

  SECTION("continguous iterator, static")
  {
    const std::vector<uint64_t> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    const auto ub = user_blob(v.begin(), v.end(), _static);

    REQUIRE(reinterpret_cast<const uint64_t*>(ub.data()) == v.data());
    REQUIRE(ub.size_bytes() == 80);
    REQUIRE(ub.destructor() == _static);
  }

  SECTION("data and pointer, transient")
  {
    const std::vector<uint64_t> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    const auto ub = user_blob(v.data(), static_cast<int>(v.size()));

    REQUIRE(reinterpret_cast<const uint64_t*>(ub.data()) == v.data());
    REQUIRE(ub.size_bytes() == 80);
    REQUIRE(ub.destructor() == transient);
  }

  SECTION("data and pointer, static")
  {
    const std::vector<uint64_t> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    const auto ub = user_blob(v.data(), static_cast<int>(v.size()), _static);

    REQUIRE(reinterpret_cast<const uint64_t*>(ub.data()) == v.data());
    REQUIRE(ub.size_bytes() == 80);
    REQUIRE(ub.destructor() == _static);
  }
}
