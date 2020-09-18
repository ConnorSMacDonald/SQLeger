#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/sqleger.hpp>

#include <type_traits>


using namespace sqleger;
using namespace sqleger::string_span_literals;

using namespace std::string_literals;


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

  SECTION("skip")
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

    const auto sk = column_traits<skip_t>::from_value(s3.column_value(0));
    REQUIRE(std::is_same_v<std::remove_const_t<decltype(sk)>, skip_t>);

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

  SECTION("vector")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x BLOB NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    const auto v1 = std::vector<uint64_t>({4, 3, 2, 1});

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    const auto [r, idx] = ::sqleger::bind(s2, v1);
    REQUIRE(r == result::ok);
    REQUIRE(idx == 1);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto v2
      = column_traits<std::vector<uint64_t>>::from_value(s3.column_value(0));
    REQUIRE(v2 == v1);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("array")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x BLOB NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    const std::array<uint64_t, 3> a1 {5, 2, 3};

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    const auto [r, idx] = ::sqleger::bind(s2, a1);
    REQUIRE(r == result::ok);
    REQUIRE(idx == 1);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto a2
      = column_traits<std::array<uint64_t, 3>>::from_value(s3.column_value(0));
    REQUIRE(a2 == a1);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("float")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x REAL NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    const auto float1 = 2.0f;

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    const auto [r, idx] = bind(s2, float1);
    REQUIRE(r == result::ok);
    REQUIRE(idx == 1);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto float2 = column_traits<float>::from_value(s3.column_value(0));
    REQUIRE(float2 == float1);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("short")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    const auto short1 = static_cast<short>(-31);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    const auto [r, idx] = bind(s2, short1);
    REQUIRE(r == result::ok);
    REQUIRE(idx == 1);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto short2 = column_traits<short>::from_value(s3.column_value(0));
    REQUIRE(short2 == short1);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("unsigned short")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    const auto ushort1 = static_cast<unsigned short>(789);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    const auto [r, idx] = bind(s2, ushort1);
    REQUIRE(r == result::ok);
    REQUIRE(idx == 1);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto ushort2
      = column_traits<unsigned short>::from_value(s3.column_value(0));
    REQUIRE(ushort2 == ushort1);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("unsigned int")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    const auto uint1
      = static_cast<unsigned int>(std::numeric_limits<int>::max()) + 1U;

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    const auto [r, idx] = bind(s2, uint1);
    REQUIRE(r == result::ok);
    REQUIRE(idx == 1);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto uint2
      = column_traits<unsigned int>::from_value(s3.column_value(0));
    REQUIRE(uint2 == uint1);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("long")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    const auto long1 = static_cast<long>(std::numeric_limits<int>::max()) + 1L;

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    const auto [r, idx] = bind(s2, long1);
    REQUIRE(r == result::ok);
    REQUIRE(idx == 1);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto long2 = column_traits<long>::from_value(s3.column_value(0));
    REQUIRE(long2 == long1);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("unsigned long")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    const auto ulong1
      = static_cast<unsigned long>(std::numeric_limits<int>::max()) + 1UL;

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    const auto [r, idx] = bind(s2, ulong1);
    REQUIRE(r == result::ok);
    REQUIRE(idx == 1);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto ulong2
      = column_traits<unsigned long>::from_value(s3.column_value(0));
    REQUIRE(ulong2 == ulong1);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("bool")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    const auto bool1 = true;

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    const auto [r, idx] = bind(s2, bool1);
    REQUIRE(r == result::ok);
    REQUIRE(idx == 1);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto bool2 = column_traits<bool>::from_value(s3.column_value(0));
    REQUIRE(bool2 == bool1);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("optional")
  {
    SECTION("null")
    {
      auto d = db(":memory:");

      auto s1 = stmt(d, "CREATE TABLE t(x INTEGER)"_ss);
      REQUIRE(s1.step() == result::done);

      const auto optional1 = std::optional<bool>();

      auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

      const auto [r, idx] = ::sqleger::bind(s2, optional1);
      REQUIRE(r == result::ok);
      REQUIRE(idx == 1);

      REQUIRE(s2.step() == result::done);

      auto s3 = stmt(d, "SELECT x FROM t"_ss);
      REQUIRE(s3.step() == result::row);

      const auto optional2
        = column_traits<std::optional<bool>>::from_value(s3.column_value(0));
      REQUIRE(optional2 == optional1);

      REQUIRE(s3.step() == result::done);
    }

    SECTION("not null")
    {
      auto d = db(":memory:");

      auto s1 = stmt(d, "CREATE TABLE t(x INTEGER)"_ss);
      REQUIRE(s1.step() == result::done);

      const auto optional1 = std::make_optional(true);

      auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

      const auto [r, idx] = ::sqleger::bind(s2, optional1);
      REQUIRE(r == result::ok);
      REQUIRE(idx == 1);

      REQUIRE(s2.step() == result::done);

      auto s3 = stmt(d, "SELECT x FROM t"_ss);
      REQUIRE(s3.step() == result::row);

      const auto optional2
        = column_traits<std::optional<bool>>::from_value(s3.column_value(0));
      REQUIRE(optional2 == optional1);

      REQUIRE(s3.step() == result::done);
    }
  }

  SECTION("char")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x TEXT NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    const auto char1 = '!';

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    const auto [r, idx] = bind(s2, char1);
    REQUIRE(r == result::ok);
    REQUIRE(idx == 1);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto char2 = column_traits<char>::from_value(s3.column_value(0));
    REQUIRE(char2 == char1);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("std::string")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x TEXT NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    const auto stds1 = "AJSCAPMOS Mdfaslflasfl#@$2323"s;

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    const auto [r, idx] = ::sqleger::bind(s2, stds1);
    REQUIRE(r == result::ok);
    REQUIRE(idx == 1);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto stds2
      = column_traits<std::string>::from_value(s3.column_value(0));
    REQUIRE(stds2 == stds1);

    REQUIRE(s3.step() == result::done);
  }
}
