#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/utility.hpp>


using namespace sqleger;


TEST_CASE("A pointer can be exchanged for nullptr", "[utility]")
{
  int i = 0;
  int* p1 = &i;
  auto* const p2 = exchange_nullptr(p1);

  REQUIRE(p1 == nullptr);
  REQUIRE(p2 == &i);
}
