#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/blob_data.hpp>

#include <utility>


using namespace sqleger;


TEST_CASE("Blob data holds a byte pointer", "[blob_data]")
{
  const auto v = std::vector<uint64_t>({1, 2, 3});

  const auto* const void_ptr = reinterpret_cast<const void*>(v.data());

  const auto bd = blob_data(void_ptr);

  REQUIRE(void_ptr == reinterpret_cast<const void*>(bd.ptr()));
}

TEST_CASE("Blob data can be converted to a pointer", "[blob_data]")
{
  const auto v = std::vector<uint64_t>({1, 2, 3});

  const auto bd = blob_data(reinterpret_cast<const void*>(v.data()));

  const auto* const ptr = bd.as_pointer<uint64_t>();

  REQUIRE(ptr == v.data());
}

TEST_CASE("Blob data can be converted to a vector", "[blob_data]")
{
  const auto v1 = std::vector<std::pair<int, int>>({{1, 2}, {2, 3}});

  const auto bd = blob_data(reinterpret_cast<const void*>(v1.data()));

  const auto v2 = bd.as_vector<std::pair<int, int>>(2);

  REQUIRE(v2 == v1);
}

TEST_CASE("Blob data can be converted to an array", "[blob_data]")
{
  const std::array<int, 2> a1 {1, 6};

  const auto bd = blob_data(reinterpret_cast<const void*>(a1.data()));

  const auto a2 = bd.as_array<int, 2>();

  REQUIRE(a2 == a1);
}
