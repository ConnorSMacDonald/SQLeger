#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/binder.hpp>
#include <sqleger/db.hpp>


using namespace sqleger;
using namespace sqleger::string_span_literals;


TEST_CASE("A binder can be used to bind values", "[binder]")
{
  SECTION("single value")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER NOT NULL)"_ss);
    REQUIRE(s1.step() == result_t::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto b = binder(s2);

    const auto int1 = 39;

    const auto [r, idx] = b(int1);

    REQUIRE(r == result_t::ok);
    REQUIRE(idx == 1);

    REQUIRE(s2.step() == result_t::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result_t::row);

    const auto int2 = s3.column_int(0);
    REQUIRE(int2 == int1);

    REQUIRE(s3.step() == result_t::done);
  }

  SECTION("multiple values")
  {
    auto d = db(":memory:");

    auto s1 = stmt(
      d, "CREATE TABLE t(x INTEGER NOT NULL, y REAL NOT NULL, z INTEGER)"_ss);
    REQUIRE(s1.step() == result_t::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1, ?2, ?3)"_ss);

    auto b = binder(s2);

    const auto int1 = 39;
    const auto double1 = 0.125;
    const auto n = null();

    const auto [r, idx] = b(int1, double1, n);

    REQUIRE(r == result_t::ok);
    REQUIRE(idx == 3);

    REQUIRE(s2.step() == result_t::done);

    auto s3 = stmt(d, "SELECT x, y, z FROM t"_ss);
    REQUIRE(s3.step() == result_t::row);

    const auto int2 = s3.column_int(0);
    REQUIRE(int2 == int1);

    const auto double2 = s3.column_double(1);
    REQUIRE(double2 == double1);

    const auto dt = s3.column_type(2);
    REQUIRE(dt == datatype_t::null);

    REQUIRE(s3.step() == result_t::done);
  }

  SECTION("void")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER NOT NULL)"_ss);
    REQUIRE(s1.step() == result_t::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto b = binder(s2);

    const auto [r, idx] = b();

    REQUIRE(r == result_t::ok);
    REQUIRE(idx == 0);

    REQUIRE(is_error(s2.step()));
  }

  SECTION("single value with error")
  {
    auto d = db(":memory:");

    auto s = stmt(d, "CREATE TABLE t(x INTEGER)"_ss);

    auto b = binder(s);

    const auto [r, idx] = b(null());

    REQUIRE(r == result_t::range);
    REQUIRE(idx == 1);
  }

  SECTION("multiple values with error")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER NOT NULL, y REAL NOT NULL)"_ss);
    REQUIRE(s1.step() == result_t::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1, ?2)"_ss);

    auto b = binder(s2);

    const auto int1 = 39;
    const auto double1 = 0.125;
    const auto n = null();

    const auto [r, idx] = b(int1, double1, n);

    REQUIRE(r == result_t::range);
    REQUIRE(idx == 3);
  }
}

TEST_CASE("A binder can be used to bind values through a stream interface",
          "[binder]")
{
  SECTION("single value")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER NOT NULL)"_ss);
    REQUIRE(s1.step() == result_t::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto b = binder(s2);

    const auto int1 = 39;

    b << int1;

    REQUIRE(b.code() == result_t::ok);
    REQUIRE(b.index() == 1);

    REQUIRE(s2.step() == result_t::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result_t::row);

    const auto int2 = s3.column_int(0);
    REQUIRE(int2 == int1);

    REQUIRE(s3.step() == result_t::done);
  }

  SECTION("multiple values")
  {
    auto d = db(":memory:");

    auto s1 = stmt(
      d, "CREATE TABLE t(x INTEGER NOT NULL, y REAL NOT NULL, z INTEGER)"_ss);
    REQUIRE(s1.step() == result_t::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1, ?2, ?3)"_ss);

    auto b = binder(s2);

    const auto int1 = 39;
    const auto double1 = 0.125;
    const auto n = null();

    b << int1 << double1 << n;

    REQUIRE(b.code() == result_t::ok);
    REQUIRE(b.index() == 3);

    REQUIRE(s2.step() == result_t::done);

    auto s3 = stmt(d, "SELECT x, y, z FROM t"_ss);
    REQUIRE(s3.step() == result_t::row);

    const auto int2 = s3.column_int(0);
    REQUIRE(int2 == int1);

    const auto double2 = s3.column_double(1);
    REQUIRE(double2 == double1);

    const auto dt = s3.column_type(2);
    REQUIRE(dt == datatype_t::null);

    REQUIRE(s3.step() == result_t::done);
  }
}

TEST_CASE("Values can be bound to a stmt through a generic free function",
          "[binder]")
{
  auto d = db(":memory:");

  auto s1 = stmt(
    d, "CREATE TABLE t(x INTEGER NOT NULL, y REAL NOT NULL, z INTEGER)"_ss);
  REQUIRE(s1.step() == result_t::done);

  auto s2 = stmt(d, "INSERT INTO t VALUES(?1, ?2, ?3)"_ss);

  const auto int1 = 39;
  const auto double1 = 0.125;
  const auto n = null();

  const auto [r, idx] = bind(s2, int1, double1, n);

  REQUIRE(r == result_t::ok);
  REQUIRE(idx == 3);

  REQUIRE(s2.step() == result_t::done);

  auto s3 = stmt(d, "SELECT x, y, z FROM t"_ss);
  REQUIRE(s3.step() == result_t::row);

  const auto int2 = s3.column_int(0);
  REQUIRE(int2 == int1);

  const auto double2 = s3.column_double(1);
  REQUIRE(double2 == double1);

  const auto dt = s3.column_type(2);
  REQUIRE(dt == datatype_t::null);

  REQUIRE(s3.step() == result_t::done);
}
