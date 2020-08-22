#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/binder.hpp>
#include <sqleger/column_traits.hpp>
#include <sqleger/db.hpp>

#include <type_traits>


using namespace sqleger;
using namespace sqleger::string_span_literals;


TEST_CASE(
  "Column traits can be used to retrieve values through a universal interface",
  "[column_traits]")
{
  SECTION("blob")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x BLOB NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    const auto v1 = std::vector<uint64_t>({4, 3, 2, 1});
    const auto ub = user_blob(v1);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    const auto [r, idx] = bind(s2, ub);
    REQUIRE(r == result::ok);
    REQUIRE(idx == 1);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto bd = column_traits<blob_data>::from_value(s3.column_value(0));

    const auto v2 = bd.as_vector<uint64_t>(v1.size());
    REQUIRE(v2 == v1);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("double")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x REAL NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    const auto double1 = 2.0;

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    const auto [r, idx] = bind(s2, double1);
    REQUIRE(r == result::ok);
    REQUIRE(idx == 1);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto double2 = column_traits<double>::from_value(s3.column_value(0));
    REQUIRE(double2 == double1);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("int")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    const auto int1 = -31;

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    const auto [r, idx] = bind(s2, int1);
    REQUIRE(r == result::ok);
    REQUIRE(idx == 1);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto int2 = column_traits<int>::from_value(s3.column_value(0));
    REQUIRE(int2 == int1);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("int64")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    const auto int64_1 = -31;

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    const auto [r, idx] = bind(s2, int64_1);
    REQUIRE(r == result::ok);
    REQUIRE(idx == 1);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto int64_2 = column_traits<int64>::from_value(s3.column_value(0));
    REQUIRE(int64_2 == int64_1);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("null")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    const auto [r, idx] = bind(s2, null);
    REQUIRE(r == result::ok);
    REQUIRE(idx == 1);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto n = column_traits<null_t>::from_value(s3.column_value(0));
    REQUIRE(std::is_same_v<std::remove_const_t<decltype(n)>, null_t>);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("text")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x TEXT NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    const auto ss = "sdga$GFsadf asdfaAQQWWWWtASGR3430"_ss;
    const auto ut = user_text(ss);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    const auto [r, idx] = bind(s2, ut);
    REQUIRE(r == result::ok);
    REQUIRE(idx == 1);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto uzv
      = column_traits<uzstring_view>::from_value(s3.column_value(0));
    const auto stds = utf8_to_ascii(uzv);
    REQUIRE(string_span(stds) == ss);

    REQUIRE(s3.step() == result::done);
  }
}
