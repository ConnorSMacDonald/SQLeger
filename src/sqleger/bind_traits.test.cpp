#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/sqleger.hpp>

#include <limits>


using namespace sqleger;
using namespace sqleger::string_span_literals;

using namespace std::string_literals;
using namespace std::string_view_literals;


TEST_CASE(
  "Bind traits can be used to bind values through a universal interface",
  "[bind_traits]")
{
  SECTION("blob")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x BLOB NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto p = parameter(s2, 1);

    const std::vector<uint64_t> v1 = {1, 2, 3, 4, 5};
    const auto ub = user_blob(v1);

    const auto r = bind_traits<user_blob>::bind(p, ub);
    REQUIRE(r == result::ok);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t");
    REQUIRE(s3.step() == result::row);

    const auto v2 = s3.column_blob(0).as_vector<uint64_t>(v1.size());
    REQUIRE(v2 == v1);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("double")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x REAL NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto p = parameter(s2, 1);

    const auto double1 = 0.5;

    const auto r = bind_traits<double>::bind(p, double1);
    REQUIRE(r == result::ok);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto double2 = s3.column_double(0);
    REQUIRE(double2 == double1);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("int")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto p = parameter(s2, 1);

    const auto int1 = 34;

    const auto r = bind_traits<int>::bind(p, int1);
    REQUIRE(r == result::ok);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto int2 = s3.column_int(0);
    REQUIRE(int2 == int1);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("int64")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto p = parameter(s2, 1);

    const auto int64_1 = static_cast<int64>(
      static_cast<int64>(std::numeric_limits<int>::max()) + 1LL);

    const auto r = bind_traits<int64>::bind(p, int64_1);
    REQUIRE(r == result::ok);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto int64_2 = s3.column_int64(0);
    REQUIRE(int64_2 == int64_1);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("null")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto p = parameter(s2, 1);

    const auto r = bind_traits<null_t>::bind(p, null);
    REQUIRE(r == result::ok);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto dt = s3.column_type(0);
    REQUIRE(dt == datatype::null);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("skip")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto p = parameter(s2, 1);

    const auto r = bind_traits<skip_t>::bind(p, skip);
    REQUIRE(r == result::ok);

    REQUIRE(is_error(s2.step()));
  }

  SECTION("text")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x TEXT NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto p = parameter(s2, 1);

    const auto ss = "sdga$GFA4QdQcwdf vdv QD D qs#3ertASGR3430"_ss;

    const auto r = bind_traits<user_text>::bind(p, ss);
    REQUIRE(r == result::ok);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto stds = utf8_to_ascii(s3.column_text(0));
    REQUIRE(string_span(stds) == ss);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("vector")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x BLOB NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto p = parameter(s2, 1);

    const auto v1 = std::vector<uint64_t>({1, 2, 3, 4, 5});

    const auto r = bind_traits<std::vector<uint64_t>>::bind(p, v1);
    REQUIRE(r == result::ok);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t");
    REQUIRE(s3.step() == result::row);

    const auto v2 = s3.column_blob(0).as_vector<uint64_t>(v1.size());
    REQUIRE(v2 == v1);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("array")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x BLOB NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto p = parameter(s2, 1);

    const std::array<uint64_t, 5> a1 {1, 2, 3, 4, 5};

    const auto r = bind_traits<std::array<uint64_t, 5>>::bind(p, a1);
    REQUIRE(r == result::ok);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t");
    REQUIRE(s3.step() == result::row);

    const auto a2 = s3.column_blob(0).as_array<uint64_t, 5>();
    REQUIRE(a2 == a1);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("float")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x REAL NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto p = parameter(s2, 1);

    const auto float1 = 0.5f;

    const auto r = bind_traits<float>::bind(p, float1);
    REQUIRE(r == result::ok);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto float2 = static_cast<float>(s3.column_double(0));
    REQUIRE(float2 == float1);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("short")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto p = parameter(s2, 1);

    const auto short1 = static_cast<short>(34);

    const auto r = bind_traits<short>::bind(p, short1);
    REQUIRE(r == result::ok);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto short2 = static_cast<short>(s3.column_int(0));
    REQUIRE(short2 == short1);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("unsigned short")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto p = parameter(s2, 1);

    const auto ushort1 = static_cast<unsigned short>(34);

    const auto r = bind_traits<unsigned short>::bind(p, ushort1);
    REQUIRE(r == result::ok);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto ushort2 = static_cast<unsigned short>(s3.column_int(0));
    REQUIRE(ushort2 == ushort1);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("unsigned int")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto p = parameter(s2, 1);

    const auto uint1
      = static_cast<unsigned int>(std::numeric_limits<int>::max()) + 1U;

    const auto r = bind_traits<unsigned int>::bind(p, uint1);
    REQUIRE(r == result::ok);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto uint2 = static_cast<unsigned int>(s3.column_int64(0));
    REQUIRE(uint2 == uint1);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("long")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto p = parameter(s2, 1);

    const auto long1 = static_cast<long>(std::numeric_limits<int>::max()) + 1L;

    const auto r = bind_traits<long>::bind(p, long1);
    REQUIRE(r == result::ok);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto long2 = static_cast<long>(s3.column_int64(0));
    REQUIRE(long2 == long1);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("unsigned long")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto p = parameter(s2, 1);

    const auto ulong1
      = static_cast<unsigned long>(std::numeric_limits<int>::max()) + 1UL;

    const auto r = bind_traits<unsigned long>::bind(p, ulong1);
    REQUIRE(r == result::ok);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto ulong2 = static_cast<unsigned long>(s3.column_int64(0));
    REQUIRE(ulong2 == ulong1);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("bool")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto p = parameter(s2, 1);

    const auto bool1 = true;

    const auto r = bind_traits<bool>::bind(p, bool1);
    REQUIRE(r == result::ok);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto bool2 = static_cast<bool>(s3.column_int(0));
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

      auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

      auto p = parameter(s2, 1);

      const auto r = bind_traits<std::optional<bool>>::bind(p, std::nullopt);
      REQUIRE(r == result::ok);

      REQUIRE(s2.step() == result::done);

      auto s3 = stmt(d, "SELECT x FROM t"_ss);
      REQUIRE(s3.step() == result::row);

      const auto dt = s3.column_type(0);
      REQUIRE(dt == datatype::null);

      REQUIRE(s3.step() == result::done);
    }

    SECTION("not null")
    {
      auto d = db(":memory:");

      auto s1 = stmt(d, "CREATE TABLE t(x INTEGER)"_ss);
      REQUIRE(s1.step() == result::done);

      auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

      auto p = parameter(s2, 1);

      const auto optional1 = std::make_optional(true);

      const auto r = bind_traits<std::optional<bool>>::bind(p, optional1);
      REQUIRE(r == result::ok);

      REQUIRE(s2.step() == result::done);

      auto s3 = stmt(d, "SELECT x FROM t"_ss);
      REQUIRE(s3.step() == result::row);

      const auto dt = s3.column_type(0);
      REQUIRE(dt == datatype::integer);

      const auto bool1 = static_cast<int>(s3.column_int(0));
      REQUIRE(bool1 == *optional1);

      REQUIRE(s3.step() == result::done);
    }
  }

  SECTION("char")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x TEXT NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto p = parameter(s2, 1);

    const auto char1 = 'z';

    const auto r = bind_traits<char>::bind(p, char1);
    REQUIRE(r == result::ok);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto stds = utf8_to_ascii(s3.column_text(0));
    REQUIRE(stds.size() == 1);
    REQUIRE(stds[0] == char1);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("const char*")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x TEXT NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto p = parameter(s2, 1);

    const auto* const c_str1 = "fjkqdwk;dqfj";

    const auto r = bind_traits<const char*>::bind(p, c_str1);
    REQUIRE(r == result::ok);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto stds = utf8_to_ascii(s3.column_text(0));
    REQUIRE(stds == c_str1);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("std::string")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x TEXT NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto p = parameter(s2, 1);

    const auto stds1 = "fjkqdwk;dqfj"s;

    const auto r = bind_traits<std::string>::bind(p, stds1);
    REQUIRE(r == result::ok);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto stds2 = utf8_to_ascii(s3.column_text(0));
    REQUIRE(stds2 == stds1);

    REQUIRE(s3.step() == result::done);
  }

  SECTION("std::string_view")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x TEXT NOT NULL)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    auto p = parameter(s2, 1);

    const auto stdsv1 = "fjkqdwk;dqfj"sv;

    const auto r = bind_traits<std::string_view>::bind(p, stdsv1);
    REQUIRE(r == result::ok);

    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);

    const auto stds = utf8_to_ascii(s3.column_text(0));
    const auto stdsv2 = static_cast<std::string_view>(stds);
    REQUIRE(stdsv2 == stdsv1);

    REQUIRE(s3.step() == result::done);
  }
}
