#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/type_erasure.hpp>


using namespace sqleger;


TEST_CASE("A pointer or reference can be type-erased and type-inferred",
          "[type_erasure]")
{
  int i1 {13};

  void* const vp1 = erase_type(&i1);
  auto* const ip1 = infer_type<int*>(vp1);

  REQUIRE(ip1 == &i1);

  void* const vp2 = erase_type(i1);
  auto& ir1 = infer_type<int&>(vp2);

  REQUIRE(&ir1 == &i1);
}

TEST_CASE("A typeless can be constructed from a pointer or reference",
          "[type_erasure]")
{
  int i1 {234};

  const typeless t1 {&i1};
  REQUIRE(t1.ptr() == reinterpret_cast<void*>(&i1));

  const typeless t2 {i1};
  REQUIRE(t2.ptr() == reinterpret_cast<void*>(&i1));
}

TEST_CASE("A typeless can be inferred as a pointer or reference",
          "[type_erasure]")
{
  int i1 {519};
  const typeless t1 {&i1};

  auto& ir1 = t1.as<int&>();
  REQUIRE(&ir1 == &i1);

  auto* const ip1 = t1.as<int*>();
  REQUIRE(ip1 == &i1);
}