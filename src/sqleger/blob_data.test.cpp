#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/sqleger.hpp>

#include <utility>


using namespace sqleger;


TEST_CASE("Blob data holds a byte pointer", "[blob_data]")
{
  auto const v = std::vector<uint64_t>({1, 2, 3});

  auto const* const void_ptr = reinterpret_cast<void const*>(v.data());

  auto const bd = blob_data(void_ptr);

  REQUIRE(void_ptr == reinterpret_cast<void const*>(bd.ptr()));
}

TEST_CASE("Blob data can be converted to a pointer", "[blob_data]")
{
  auto const v = std::vector<uint64_t>({1, 2, 3});

  auto const bd = blob_data(reinterpret_cast<void const*>(v.data()));

  auto const* const ptr = bd.as_pointer<uint64_t>();

  REQUIRE(ptr == v.data());
}

TEST_CASE("Blob data can be converted to a vector", "[blob_data]")
{
  auto const v1 = std::vector<std::pair<int, int>>({{1, 2}, {2, 3}});

  auto const bd = blob_data(reinterpret_cast<void const*>(v1.data()));

  auto const v2 = bd.as_vector<std::pair<int, int>>(2);

  REQUIRE(v2 == v1);
}

TEST_CASE("Blob data can be converted to an array", "[blob_data]")
{
  std::array<int, 2> const a1 {1, 6};

  auto const bd = blob_data(reinterpret_cast<void const*>(a1.data()));

  auto const a2 = bd.as_array<int, 2>();

  REQUIRE(a2 == a1);
}
