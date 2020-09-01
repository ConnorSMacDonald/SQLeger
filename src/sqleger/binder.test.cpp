#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/sqleger.hpp>


using namespace sqleger;
using namespace sqleger::string_span_literals;


TEST_CASE("A bind result can be compared for equality", "[binder]")
{
  const bind_result br1 {result::ok, 1};
  const bind_result br2 {result::error, 1};
  const bind_result br3 {result::ok, 3};

  REQUIRE(br1 == br1);
  REQUIRE_FALSE(br1 != br1);

  REQUIRE_FALSE(br1 == br2);
  REQUIRE(br1 != br2);

  REQUIRE_FALSE(br1 == br3);
  REQUIRE(br1 != br3);
}

TEST_CASE("A binder can be used to bind values", "[binder]")
{
  SECTION("single value")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto b = binder(s2);

    const auto int1 = 39;

    const auto [r, idx] = b(int1);
    REQUIRE(r == result::ok);
    REQUIRE(idx == 1);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto int2 = s3.column_int(0);
    REQUIRE(int2 == int1);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("multiple values")
  {
    auto d = db(":memory:");

    auto s1 = stmt(
      d, "CREATE TABLE t(x INTEGER NOT NULL, y REAL NOT NULL, z INTEGER)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1, ?2, ?3)"_ss);

    auto b = binder(s2);

    const auto int1 = 39;
    const auto double1 = 0.125;

    const auto [r, idx] = b(int1, double1, null);
    REQUIRE(r == result::ok);
    REQUIRE(idx == 3);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x, y, z FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto int2 = s3.column_int(0);
    REQUIRE(int2 == int1);

    const auto double2 = s3.column_double(1);
    REQUIRE(double2 == double1);

    const auto dt = s3.column_type(2);
    REQUIRE(dt == datatype::null);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("void")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto b = binder(s2);

    const auto [r, idx] = b();
    REQUIRE(r == result::ok);
    REQUIRE(idx == 0);

    REQUIRE(is_error(s2.step()));
  }

  SECTION("single value with error")
  {
    auto d = db(":memory:");

    auto s = stmt(d, "CREATE TABLE t(x INTEGER)"_ss);

    auto b = binder(s);

    const auto [r, idx] = b(null);

    REQUIRE(r == result::range);
    REQUIRE(idx == 1);
  }

  SECTION("multiple values with error")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER NOT NULL, y REAL NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1, ?2)"_ss);

    auto b = binder(s2);

    const auto int1 = 39;
    const auto double1 = 0.125;

    const auto [r, idx] = b(int1, double1, null);
    REQUIRE(r == result::range);
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
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto b = binder(s2);

    const auto int1 = 39;

    b << int1;

    REQUIRE(b.code() == result::ok);
    REQUIRE(b.index() == 1);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto int2 = s3.column_int(0);
    REQUIRE(int2 == int1);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("multiple values")
  {
    auto d = db(":memory:");

    auto s1 = stmt(
      d, "CREATE TABLE t(x INTEGER NOT NULL, y REAL NOT NULL, z INTEGER)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1, ?2, ?3)"_ss);

    auto b = binder(s2);

    const auto int1 = 39;
    const auto double1 = 0.125;

    b << int1 << double1 << null;

    REQUIRE(b.code() == result::ok);
    REQUIRE(b.index() == 3);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x, y, z FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto int2 = s3.column_int(0);
    REQUIRE(int2 == int1);

    const auto double2 = s3.column_double(1);
    REQUIRE(double2 == double1);

    const auto dt = s3.column_type(2);
    REQUIRE(dt == datatype::null);

    REQUIRE(s3.step() == result::done);
  }
}

TEST_CASE("Values can be bound to a stmt through a generic free function",
          "[binder]")
{
  auto d = db(":memory:");

  auto s1 = stmt(
    d, "CREATE TABLE t(x INTEGER NOT NULL, y REAL NOT NULL, z INTEGER)"_ss);
  REQUIRE(s1.step() == result::done);

  auto s2 = stmt(d, "INSERT INTO t VALUES(?1, ?2, ?3)"_ss);

  const auto int1 = 39;
  const auto double1 = 0.125;

  const auto [r, idx] = bind(s2, int1, double1, null);

  REQUIRE(r == result::ok);
  REQUIRE(idx == 3);

  REQUIRE(s2.step() == result::done);

  auto s3 = stmt(d, "SELECT x, y, z FROM t"_ss);
  REQUIRE(s3.step() == result::row);

  const auto int2 = s3.column_int(0);
  REQUIRE(int2 == int1);

  const auto double2 = s3.column_double(1);
  REQUIRE(double2 == double1);

  const auto dt = s3.column_type(2);
  REQUIRE(dt == datatype::null);

  REQUIRE(s3.step() == result::done);
}
