#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/db.hpp>


using namespace sqleger;


TEST_CASE("A db can be opened and closed", "[db]")
{
  SECTION("open v1, close v1")
  {
    db d;
    const auto r1 = db::open(":memory:", d);

    REQUIRE(r1 == result_t::ok);
    REQUIRE(d.c_ptr() != nullptr);

    const auto r2 = d.close();

    REQUIRE(r2 == result_t::ok);
  }

  SECTION("open v1, close v2")
  {
    db d;
    const auto r1 = db::open(":memory:", d);

    REQUIRE(r1 == result_t::ok);
    REQUIRE(d.c_ptr() != nullptr);

    const auto r2 = d.close_v2();

    REQUIRE(r2 == result_t::ok);
  }

  SECTION("open v2, close v1")
  {
    db d;
    const auto r1
      = db::open_v2(":memory:", d, flags({open_t::readonly, open_t::nomutex}));

    REQUIRE(r1 == result_t::ok);
    REQUIRE(d.c_ptr() != nullptr);

    const auto r2 = d.close();

    REQUIRE(r2 == result_t::ok);
  }
}

TEST_CASE("A C handle can be taken from a db", "[db]")
{
  db d1;
  const auto r = db::open(":memory:", d1);

  REQUIRE(r == result_t::ok);

  const auto* const p1 = d1.c_ptr();
  const auto* const p2 = d1.take_c_ptr();

  REQUIRE(p1 == p2);
  REQUIRE(d1.c_ptr() == nullptr);
}

TEST_CASE("A db can be moved", "[db]")
{
  SECTION("construction")
  {
    db d1;
    const auto r = db::open(":memory:", d1);

    REQUIRE(r == result_t::ok);

    const auto* const p = d1.c_ptr();

    auto d2 = db(std::move(d1));

    REQUIRE(d2.c_ptr() == p);
    REQUIRE(d1.c_ptr() == nullptr);
  }

  SECTION("assignment")
  {
    db d1;
    const auto r = db::open(":memory:", d1);

    REQUIRE(r == result_t::ok);

    const auto* const p = d1.c_ptr();

    db d2;
    d2 = std::move(d1);

    REQUIRE(d2.c_ptr() == p);
    REQUIRE(d1.c_ptr() == nullptr);
  }
}
