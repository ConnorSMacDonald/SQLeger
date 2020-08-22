#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/bind_traits.hpp>
#include <sqleger/db.hpp>


using namespace sqleger;
using namespace sqleger::string_span_literals;


TEST_CASE(
  "User value traits can be used to bind values through a universal interface",
  "[bind_traits]")
{
  SECTION("blob")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x BLOB NOT NULL)"_ss);
    REQUIRE(s1.step() == result_t::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto p = parameter(s2, 1);

    const std::vector<uint64_t> v1 = {1, 2, 3, 4, 5};
    const auto ub = user_blob(v1);

    const auto r = bind_traits<user_blob>::bind(p, ub);
    REQUIRE(r == result_t::ok);

    REQUIRE(s2.step() == result_t::done);

    auto s3 = stmt(d, "SELECT x FROM t");
    REQUIRE(s3.step() == result_t::row);

    const auto v2 = [&]() {
      auto v = std::vector<uint64_t>(5);
      std::memcpy(reinterpret_cast<void*>(v.data()),
                  s3.column_blob(0),
                  s3.column_bytes(0));
      return v;
    }();
    REQUIRE(v2 == v1);

    REQUIRE(s3.step() == result_t::done);
  }

  SECTION("double")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x REAL NOT NULL)"_ss);
    REQUIRE(s1.step() == result_t::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto p = parameter(s2, 1);

    const auto double1 = 0.5;

    const auto r = bind_traits<double>::bind(p, double1);
    REQUIRE(r == result_t::ok);

    REQUIRE(s2.step() == result_t::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result_t::row);

    const auto double2 = s3.column_double(0);
    REQUIRE(double2 == double1);

    REQUIRE(s3.step() == result_t::done);
  }

  SECTION("int")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER NOT NULL)"_ss);
    REQUIRE(s1.step() == result_t::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto p = parameter(s2, 1);

    const auto int1 = 34;

    const auto r = bind_traits<int>::bind(p, int1);
    REQUIRE(r == result_t::ok);

    REQUIRE(s2.step() == result_t::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result_t::row);

    const auto int2 = s3.column_int(0);
    REQUIRE(int2 == int1);

    REQUIRE(s3.step() == result_t::done);
  }

  SECTION("int64")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER NOT NULL)"_ss);
    REQUIRE(s1.step() == result_t::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto p = parameter(s2, 1);

    const auto int64_1 = int64(34);

    const auto r = bind_traits<int64>::bind(p, int64_1);
    REQUIRE(r == result_t::ok);

    REQUIRE(s2.step() == result_t::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result_t::row);

    const auto int64_2 = s3.column_int(0);
    REQUIRE(int64_2 == int64_1);

    REQUIRE(s3.step() == result_t::done);
  }

  SECTION("null")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER)"_ss);
    REQUIRE(s1.step() == result_t::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto p = parameter(s2, 1);

    const auto n = null();

    const auto r = bind_traits<null>::bind(p, n);
    REQUIRE(r == result_t::ok);

    REQUIRE(s2.step() == result_t::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result_t::row);

    const auto dt = s3.column_type(0);
    REQUIRE(dt == datatype_t::null);

    REQUIRE(s3.step() == result_t::done);
  }

  SECTION("text")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x TEXT NOT NULL)"_ss);
    REQUIRE(s1.step() == result_t::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto p = parameter(s2, 1);

    const auto ss = "sdga$GFA4QdQcwdf vdv QD D qs#3ertASGR3430"_ss;

    const auto r = bind_traits<user_text>::bind(p, ss);
    REQUIRE(r == result_t::ok);

    REQUIRE(s2.step() == result_t::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result_t::row);

    const auto stds = utf8_to_ascii(s3.column_text(0));
    REQUIRE(string_span(stds) == ss);

    REQUIRE(s3.step() == result_t::done);
  }
}
