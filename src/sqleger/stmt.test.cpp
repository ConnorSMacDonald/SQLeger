#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/db.hpp>
#include <sqleger/stmt.hpp>

#include <algorithm>
#include <cstdint>
#include <cstring>


using namespace sqleger;
using namespace sqleger::zstring_view_literals;
using namespace sqleger::string_span_literals;


TEST_CASE("A stmt can be prepared and finalized", "[stmt]")
{
  SECTION("prepare v2, zstring")
  {
    auto d = db(":memory:");

    const auto q = "CREATE TABLE t(x INTEGER)"_zv;

    stmt s;
    const auto r1 = d.prepare_v2(q, s);

    REQUIRE(r1 == result_t::ok);
    REQUIRE(s.c_ptr() != nullptr);
    REQUIRE(s.sql() == q);

    const auto r2 = s.finalize();

    REQUIRE(r2 == result_t::ok);
    REQUIRE(s.c_ptr() == nullptr);
  }

  SECTION("prepare v2, bounded string")
  {
    auto d = db(":memory:");

    const auto q1 = "CREATE TABLE t(x INTEGER)garbage"_zv;
    const auto q2 = string_span(q1.c_str(), 25);

    stmt s;
    const auto r1 = d.prepare_v2(q2, s);

    REQUIRE(r1 == result_t::ok);
    REQUIRE(s.c_ptr() != nullptr);
    REQUIRE(string_span(s.sql()) == q2);

    const auto r2 = s.finalize();

    REQUIRE(r2 == result_t::ok);
    REQUIRE(s.c_ptr() == nullptr);
  }

  SECTION("prepare v2, failure")
  {
    auto d = db(":memory:");

    stmt s;
    const auto r = d.prepare_v2("I'm not SQL!", s);

    REQUIRE(is_error(r));
    REQUIRE(s.c_ptr() == nullptr);
  }
}

TEST_CASE("A stmt can be prepared through a constructor-exception interface",
          "[stmt]")
{
  SECTION("prepare v2, zstring")
  {
    auto d = db(":memory:");

    const auto q = "CREATE TABLE t(x INTEGER)"_zv;

    auto s = stmt(d, q.c_str());

    REQUIRE(s.c_ptr() != nullptr);
    REQUIRE(s.sql() == q);
  }

  SECTION("prepare v2, bounded string")
  {
    auto d = db(":memory:");

    const auto q1 = "CREATE TABLE t(x INTEGER)garbage"_zv;
    const auto q2 = string_span(q1.c_str(), 25);

    auto s = stmt(d, q2);

    REQUIRE(s.c_ptr() != nullptr);
    REQUIRE(string_span(s.sql()) == q2);
  }

  SECTION("prepare v2, failure")
  {
    auto d = db(":memory:");

    try
    {
      auto s = stmt(d, "I'm not SQL!");
    }
    catch (const result_exception& e)
    {
      REQUIRE(is_error(e.code()));
    }
  }
}

TEST_CASE("A C handle can be taken from a stmt", "[stmt]")
{
  auto d = db(":memory:");
  auto s = stmt(d, "CREATE TABLE t(x INTEGER)"_ss);

  const auto* const p1 = d.c_ptr();
  const auto* const p2 = d.take_c_ptr();

  REQUIRE(p1 == p2);
  REQUIRE(d.c_ptr() == nullptr);
}

TEST_CASE("A stmt can be moved", "[stmt]")
{
  SECTION("construction")
  {
    auto d = db(":memory:");
    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER)"_ss);

    const auto* const p = s1.c_ptr();

    auto s2 = stmt(std::move(s1));

    REQUIRE(s2.c_ptr() == p);
    REQUIRE(s1.c_ptr() == nullptr);
  }

  SECTION("assignment into null")
  {
    auto d = db(":memory:");
    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER)"_ss);

    const auto* const p = s1.c_ptr();

    stmt s2;
    s2 = std::move(s1);

    REQUIRE(s2.c_ptr() == p);
    REQUIRE(s1.c_ptr() == nullptr);
  }

  SECTION("assignment into prepared stmt")
  {
    auto d = db(":memory:");
    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER)"_ss);

    const auto* const p = s1.c_ptr();

    auto s2 = stmt(d, "CREATE TABLE u(y INTEGER)"_ss);
    s2 = std::move(s1);

    REQUIRE(s2.c_ptr() == p);
    REQUIRE(s1.c_ptr() == nullptr);
  }
}

TEST_CASE("A stmt can be stepped", "[stmt]")
{
  auto d = db(":memory:");
  auto s = stmt(d, "CREATE TABLE t(x INTEGER)"_ss);

  const auto r1 = s.step();
  REQUIRE(r1 == result_t::done);

  const auto r2 = s.step();
  REQUIRE(is_error(r2));
}

TEST_CASE("A stmt can be reset", "[stmt]")
{
  auto d = db(":memory:");
  auto s = stmt(d, "CREATE TABLE IF NOT EXISTS t(x INTEGER)"_ss);

  const auto r1 = s.step();
  REQUIRE(r1 == result_t::done);

  const auto r2 = s.reset();
  REQUIRE(r2 == result_t::ok);

  const auto r3 = s.step();
  REQUIRE(r3 == result_t::done);
}

TEST_CASE("A stmt can be bound to", "[stmt]")
{
  auto d = db(":memory:");

  auto s1 = stmt(d,
                 "CREATE TABLE t("
                 "a BLOB NOT NULL,"
                 "b REAL NOT NULL,"
                 "c INTEGER NOT NULL,"
                 "d INTEGER NOT NULL,"
                 "e INTEGER,"
                 "f TEXT NOT NULL)"_ss);

  REQUIRE(s1.step() == result_t::done);

  auto s2 = stmt(d, "INSERT INTO t VALUES(?1, ?2, ?3, ?4, ?5, ?6)");

  const std::vector<uint64_t> v = {1, 2, 3, 4, 5};

  const auto r1 = s2.bind_blob(1, v);
  REQUIRE(r1 == result_t::ok);

  const auto r2 = s2.bind_double(2, 0.25);
  REQUIRE(r2 == result_t::ok);

  const auto r3 = s2.bind_int(3, 2);
  REQUIRE(r3 == result_t::ok);

  const auto r4 = s2.bind_int64(4, 3);
  REQUIRE(r4 == result_t::ok);

  const auto r5 = s2.bind_null(5);
  REQUIRE(r5 == result_t::ok);

  const auto r6 = s2.bind_text(6, {"aal;sdfkjjk aAAV NFLS4E352"});
  REQUIRE(r6 == result_t::ok);

  const auto r7 = s2.step();
  REQUIRE(r7 == result_t::done);
}

TEST_CASE("Data can be retrieved from a stmt", "[stmt]")
{
  auto d = db(":memory:");

  auto s1 = stmt(d,
                 "CREATE TABLE t("
                 "a BLOB NOT NULL,"
                 "b REAL NOT NULL,"
                 "c INTEGER NOT NULL,"
                 "d INTEGER NOT NULL,"
                 "e INTEGER,"
                 "f TEXT NOT NULL)"_ss);

  REQUIRE(s1.step() == result_t::done);

  auto s2 = stmt(d, "INSERT INTO t VALUES(?1, ?2, ?3, ?4, ?5, ?6)"_ss);

  const std::vector<uint64_t> v = {1, 2, 3, 4, 5};
  constexpr auto ss = " vqlflz.tlue VPNRE103-====++++"_ss;

  REQUIRE(s2.bind_blob(1, v) == result_t::ok);
  REQUIRE(s2.bind_double(2, 0.25) == result_t::ok);
  REQUIRE(s2.bind_int(3, 2) == result_t::ok);
  REQUIRE(s2.bind_int64(4, 3) == result_t::ok);
  REQUIRE(s2.bind_null(5) == result_t::ok);
  REQUIRE(s2.bind_text(6, ss) == result_t::ok);
  REQUIRE(s2.step() == result_t::done);

  auto s3 = stmt(d, "SELECT a, b, c, d, e, f FROM t"_ss);

  const auto r1 = s3.step();
  REQUIRE(r1 == result_t::row);

  const auto dt1 = s3.column_type(0);
  REQUIRE(dt1 == datatype_t::blob);

  const auto dt2 = s3.column_type(1);
  REQUIRE(dt2 == datatype_t::_float);

  const auto dt3 = s3.column_type(2);
  REQUIRE(dt3 == datatype_t::integer);

  const auto dt4 = s3.column_type(3);
  REQUIRE(dt4 == datatype_t::integer);

  const auto dt5 = s3.column_type(4);
  REQUIRE(dt5 == datatype_t::null);

  const auto dt6 = s3.column_type(5);
  REQUIRE(dt6 == datatype_t::text);

  const auto c0_sz = s3.column_bytes(0);
  REQUIRE(c0_sz == 40);

  const auto* const c0 = s3.column_blob(0);
  const auto c0_v = [&]() {
    auto v = std::vector<uint64_t>(5);
    std::memcpy(reinterpret_cast<void*>(v.data()), c0, c0_sz);
    return v;
  }();
  REQUIRE(c0_v == v);

  const auto c1 = s3.column_double(1);
  REQUIRE(c1 == 0.25);

  const auto c2 = s3.column_int(2);
  REQUIRE(c2 == 2);

  const auto c3 = s3.column_int64(3);
  REQUIRE(c3 == 3);

  const auto c5_sz = s3.column_bytes(5);
  REQUIRE(c5_sz == ss.length());

  const auto c5 = s3.column_text(5);
  const auto c5_ascii = utf8_to_ascii(c5);
  REQUIRE(string_span(c5_ascii) == ss);

  const auto r2 = s3.step();
  REQUIRE(r2 == result_t::done);
}

TEST_CASE("A stmt can have its bindings cleared", "[stmt]")
{
  auto d = db(":memory:");

  auto s1 = stmt(d, "CREATE TABLE t(x INTEGER NOT NULL)"_ss);

  REQUIRE(s1.step() == result_t::done);

  auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

  REQUIRE(s2.bind_int(1, 7) == result_t::ok);

  const auto r1 = s2.clear_bindings();

  REQUIRE(is_non_error(r1));

  const auto r2 = s2.step();

  REQUIRE(is_error(r2));
}
