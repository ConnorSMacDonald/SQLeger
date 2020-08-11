#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/db.hpp>


using namespace sqleger;

using namespace std::string_literals;
using namespace std::string_view_literals;


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
    REQUIRE(d.c_ptr() == nullptr);
  }

  SECTION("open v1, close v2")
  {
    db d;
    const auto r1 = db::open(":memory:", d);

    REQUIRE(r1 == result_t::ok);
    REQUIRE(d.c_ptr() != nullptr);

    const auto r2 = d.close_v2();

    REQUIRE(r2 == result_t::ok);
    REQUIRE(d.c_ptr() == nullptr);
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
    REQUIRE(d.c_ptr() == nullptr);
  }

  SECTION("open v2 failure, close v1")
  {
    db d;
    const auto r1
      = db::open_v2("a-db-THATdoeSn0t__exist.bad", d, open_t::readonly);

    REQUIRE(r1 != result_t::ok);
    REQUIRE(d.c_ptr() != nullptr);

    const auto r2 = d.close();

    REQUIRE(r2 == result_t::ok);
    REQUIRE(d.c_ptr() == nullptr);
  }

  SECTION("open v1, close v1 failure")
  {
    db d;
    const auto r1 = db::open(":memory:", d);

    REQUIRE(r1 == result_t::ok);
    REQUIRE(d.c_ptr() != nullptr);

    ::sqlite3_stmt* s = nullptr;
    const auto r2 = int_to_enum<result_t>(::sqlite3_prepare_v2(
      d.c_ptr(), "CREATE TABLE t(x INTEGER)", -1, &s, nullptr));

    REQUIRE(r2 == result_t::ok);

    const auto r3 = d.close();

    REQUIRE(r3 == result_t::busy);
    REQUIRE(d.c_ptr() != nullptr);

    const auto r4 = int_to_enum<result_t>(::sqlite3_finalize(s));

    REQUIRE(r4 == result_t::ok);

    const auto r5 = d.close();

    REQUIRE(r5 == result_t::ok);
    REQUIRE(d.c_ptr() == nullptr);
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

  SECTION("assignment into null")
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

  SECTION("assignment into open db")
  {
    db d1;
    const auto r1 = db::open(":memory:", d1);

    REQUIRE(r1 == result_t::ok);

    const auto* const p = d1.c_ptr();

    db d2;
    const auto r2 = db::open(":memory:", d2);

    REQUIRE(r2 == result_t::ok);

    d2 = std::move(d1);

    REQUIRE(d2.c_ptr() == p);
    REQUIRE(d1.c_ptr() == nullptr);
  }
}

TEST_CASE("An error message can be retrieved from a db interface", "[db]")
{
  db d;
  const auto r1 = db::open("/", d);

  REQUIRE(r1 != result_t::ok);

  const auto zv = d.errmsg();

  REQUIRE(zv.c_str() == ::sqlite3_errmsg(d.c_ptr()));
}

TEST_CASE("An open exception can be thrown and caught", "[db]")
{
  db d;
  const auto r = db::open("/", d);

  REQUIRE(r != result_t::ok);

  const auto msg = static_cast<std::string>(d.errmsg());

  try
  {
    throw open_exception(r, std::move(d));
  }
  catch (const open_exception& e)
  {
    REQUIRE(e.code() == r);
    REQUIRE(e.what() == msg);
  }
}

TEST_CASE("A db can be opened through a constructor-exception interface",
          "[db]")
{
  SECTION("open v1")
  {
    auto d = db(":memory:");

    REQUIRE(d.c_ptr() != nullptr);
  }

  SECTION("open v2")
  {
    auto d = db(":memory:", flags({open_t::readonly, open_t::nomutex}));

    REQUIRE(d.c_ptr() != nullptr);
  }

  SECTION("open v1 with throw")
  {
    try
    {
      auto d = db("/");
    }
    catch (const open_exception& e)
    {
      REQUIRE(e.code() == result_t::cantopen);
      REQUIRE(e.what() == "unable to open database file"sv);
    }
  }

  SECTION("open v2 with throw")
  {
    try
    {
      auto d = db("/", open_t::readwrite);
    }
    catch (const open_exception& e)
    {
      REQUIRE(e.code() == result_t::cantopen);
      REQUIRE(e.what() == "unable to open database file"sv);
    }
  }
}
