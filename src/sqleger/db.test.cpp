#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/db.hpp>


using namespace sqleger;

using namespace std::string_literals;
using namespace std::string_view_literals;


TEST_CASE("A db holds an sqlite3*", "[db]")
{
  SECTION("default constructor")
  {
    db d;
    REQUIRE(d.c_ptr() == nullptr);
    REQUIRE(!d);
  }

  SECTION("pointer constructor")
  {
    ::sqlite3* c_ptr1 {nullptr};
    const auto r1 = int_to_enum<result_t>(::sqlite3_open(":memory:", &c_ptr1));
    REQUIRE(r1 == result_t::ok);
    REQUIRE(c_ptr1 != nullptr);

    auto d = db(c_ptr1);
    REQUIRE(d.c_ptr() == c_ptr1);
    REQUIRE(d);

    auto* const c_ptr2 = d.take_c_ptr();

    REQUIRE(c_ptr2 == c_ptr1);
    REQUIRE(!d);

    const auto r2 = int_to_enum<result_t>(::sqlite3_close(c_ptr1));
    REQUIRE(r2 == result_t::ok);
  }
}

TEST_CASE("A db can be opened and closed", "[db]")
{
  SECTION("open v1, close v1")
  {
    db d;
    const auto r1 = db::open(":memory:", d);

    REQUIRE(r1 == result_t::ok);
    REQUIRE(d);

    const auto r2 = d.close();

    REQUIRE(r2 == result_t::ok);
    REQUIRE(!d);
  }

  SECTION("open v1, close v2")
  {
    db d;
    const auto r1 = db::open(":memory:", d);

    REQUIRE(r1 == result_t::ok);
    REQUIRE(d);

    const auto r2 = d.close_v2();

    REQUIRE(r2 == result_t::ok);
    REQUIRE(!d);
  }

  SECTION("open v2, close v1")
  {
    db d;
    const auto r1
      = db::open_v2(":memory:", d, flags({open_t::readonly, open_t::nomutex}));

    REQUIRE(r1 == result_t::ok);
    REQUIRE(d);

    const auto r2 = d.close();

    REQUIRE(r2 == result_t::ok);
    REQUIRE(!d);
  }

  SECTION("open v2 failure, close v1")
  {
    db d;
    const auto r1
      = db::open_v2("a-db-THATdoeSn0t__exist.bad", d, open_t::readonly);

    REQUIRE(r1 != result_t::ok);
    REQUIRE(d);

    const auto r2 = d.close();

    REQUIRE(r2 == result_t::ok);
    REQUIRE(!d);
  }

  SECTION("open v1, close v1 failure")
  {
    db d;
    const auto r1 = db::open(":memory:", d);

    REQUIRE(r1 == result_t::ok);
    REQUIRE(d);

    ::sqlite3_stmt* s = nullptr;
    const auto r2 = int_to_enum<result_t>(::sqlite3_prepare_v2(
      d.c_ptr(), "CREATE TABLE t(x INTEGER)", -1, &s, nullptr));

    REQUIRE(r2 == result_t::ok);

    const auto r3 = d.close();

    REQUIRE(r3 == result_t::busy);
    REQUIRE(d);

    const auto r4 = int_to_enum<result_t>(::sqlite3_finalize(s));

    REQUIRE(r4 == result_t::ok);

    const auto r5 = d.close();

    REQUIRE(r5 == result_t::ok);
    REQUIRE(!d);
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

    REQUIRE(d);
  }

  SECTION("open v2")
  {
    auto d = db(":memory:", flags({open_t::readonly, open_t::nomutex}));

    REQUIRE(d);
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

TEST_CASE("A db can be moved", "[db]")
{
  SECTION("construction")
  {
    auto d1 = db(":memory:");

    const auto* const p = d1.c_ptr();

    auto d2 = db(std::move(d1));

    REQUIRE(d2.c_ptr() == p);
    REQUIRE(!d1);
  }

  SECTION("assignment into null")
  {
    auto d1 = db(":memory:");

    const auto* const p = d1.c_ptr();

    db d2;
    d2 = std::move(d1);

    REQUIRE(d2.c_ptr() == p);
    REQUIRE(!d1);
  }

  SECTION("assignment into open db")
  {
    auto d1 = db(":memory:");

    const auto* const p = d1.c_ptr();

    auto d2 = db(":memory:");
    d2 = std::move(d1);

    REQUIRE(d2.c_ptr() == p);
    REQUIRE(!d1);
  }
}

TEST_CASE("A db ref can be constructed", "[db]")
{
  SECTION("from pointer")
  {
    auto d = db(":memory:");
    auto dr = db_ref(d.c_ptr());

    REQUIRE(dr.c_ptr() == d.c_ptr());
  }

  SECTION("from db")
  {
    auto d = db(":memory:");
    auto dr = db_ref(d);

    REQUIRE(dr.c_ptr() == d.c_ptr());
  }
}

TEST_CASE("The last insert rowid can be retrieved from a db", "[db]")
{
  auto d = db(":memory:");

  const auto ri1 = d.last_insert_rowid();
  REQUIRE(ri1 == 0);

  auto s1 = stmt(d, "CREATE TABLE t(x INTEGER)"_ss);
  REQUIRE(s1.step() == result_t::done);

  auto s2 = stmt(d, "INSERT INTO t VALUES(1)"_ss);
  REQUIRE(s2.step() == result_t::done);

  const auto ri2 = d.last_insert_rowid();
  REQUIRE(ri2 == 1);
}
