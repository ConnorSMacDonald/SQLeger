#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/sqleger.hpp>


using namespace sqleger;
using namespace sqleger::string_span_literals;


TEST_CASE("A value holds an sqlite3_value*", "[value]")
{
  SECTION("default constructor")
  {
    value v;
    REQUIRE(v.c_ptr() == nullptr);
    REQUIRE(!v);
  }

  SECTION("pointer constructor")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(2)");
    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t");

    auto const* const c_ptr1 = ::sqlite3_column_value(s3.c_ptr(), 0);
    REQUIRE(c_ptr1 != nullptr);

    auto* const c_ptr2 = ::sqlite3_value_dup(c_ptr1);

    value v {c_ptr2};
    REQUIRE(v.c_ptr() == c_ptr2);
    REQUIRE(v);

    auto* const c_ptr3 = v.take_c_ptr();

    REQUIRE(c_ptr3 == c_ptr2);
    REQUIRE(!v);

    ::sqlite3_value_free(c_ptr2);
  }
}

TEST_CASE("A value can be freed", "[value]")
{
  auto d = db(":memory:");

  auto s1 = stmt(d, "CREATE TABLE t(x INTEGER)"_ss);
  REQUIRE(s1.step() == result::done);

  auto s2 = stmt(d, "INSERT INTO t VALUES(2)");
  REQUIRE(s2.step() == result::done);

  auto s3 = stmt(d, "SELECT x FROM t");

  auto const* const c_ptr = ::sqlite3_column_value(s3.c_ptr(), 0);
  REQUIRE(c_ptr != nullptr);

  auto v = value(::sqlite3_value_dup(c_ptr));

  v.free();
  REQUIRE(!v);
}

TEST_CASE("A value ref can be constructed", "[value]")
{
  SECTION("from pointer")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(2)"_ss);
    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);

    auto* const c_ptr = ::sqlite3_column_value(s3.c_ptr(), 0);

    auto vr = value_ref(c_ptr);

    REQUIRE(vr.c_ptr() == c_ptr);
  }

  SECTION("from value")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(2)"_ss);
    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);

    auto v = value(::sqlite3_value_dup(::sqlite3_column_value(s3.c_ptr(), 0)));
    auto vr = value_ref(v);

    REQUIRE(vr.c_ptr() == v.c_ptr());
  }
}

TEST_CASE("A value can be dupped", "[value]")
{
  auto d = db(":memory:");

  auto s1 = stmt(d, "CREATE TABLE t(x INTEGER)"_ss);
  REQUIRE(s1.step() == result::done);

  auto s2 = stmt(d, "INSERT INTO t VALUES(2)"_ss);
  REQUIRE(s2.step() == result::done);

  auto s3 = stmt(d, "SELECT x FROM t"_ss);

  auto vr = value_ref(::sqlite3_column_value(s3.c_ptr(), 0));

  auto v = vr.dup();

  REQUIRE(v);
  REQUIRE(v.c_ptr() != vr.c_ptr());
}

TEST_CASE("Data can be retrieved from a value", "[value]")
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

  REQUIRE(s1.step() == result::done);

  auto s2 = stmt(d, "INSERT INTO t VALUES(?1, ?2, ?3, ?4, ?5, ?6)"_ss);

  std::vector<uint64_t> const vector = {1, 2, 3, 4, 5};
  constexpr auto ss = " vqlflz.tlue VPNRE103-====++++"_ss;

  REQUIRE(s2.bind_blob(1, vector) == result::ok);
  REQUIRE(s2.bind_double(2, 0.25) == result::ok);
  REQUIRE(s2.bind_int(3, 2) == result::ok);
  REQUIRE(s2.bind_int64(4, 3) == result::ok);
  REQUIRE(s2.bind_null(5) == result::ok);
  REQUIRE(s2.bind_text(6, ss) == result::ok);
  REQUIRE(s2.step() == result::done);

  auto s3 = stmt(d, "SELECT a, b, c, d, e, f FROM t"_ss);

  auto const r1 = s3.step();
  REQUIRE(r1 == result::row);

  auto v0 = value_ref(::sqlite3_column_value(s3.c_ptr(), 0));
  auto v1 = value_ref(::sqlite3_column_value(s3.c_ptr(), 1));
  auto v2 = value_ref(::sqlite3_column_value(s3.c_ptr(), 2));
  auto v3 = value_ref(::sqlite3_column_value(s3.c_ptr(), 3));
  auto v4 = value_ref(::sqlite3_column_value(s3.c_ptr(), 4));
  auto v5 = value_ref(::sqlite3_column_value(s3.c_ptr(), 5));

  auto const dt0 = v0.type();
  REQUIRE(dt0 == datatype::blob);

  auto const dt1 = v1.type();
  REQUIRE(dt1 == datatype::_float);

  auto const dt2 = v2.type();
  REQUIRE(dt2 == datatype::integer);

  auto const dt3 = v3.type();
  REQUIRE(dt3 == datatype::integer);

  auto const dt4 = v4.type();
  REQUIRE(dt4 == datatype::null);

  auto const dt5 = v5.type();
  REQUIRE(dt5 == datatype::text);

  auto const c0_sz = v0.bytes();
  REQUIRE(c0_sz == 40);

  auto const c0 = v0.blob();
  auto const c0_vector = c0.as_vector<uint64_t>(vector.size());
  REQUIRE(c0_vector == vector);

  auto const c1 = v1._double();
  REQUIRE(c1 == 0.25);

  auto const c2 = v2._int();
  REQUIRE(c2 == 2);

  auto const c3 = v3.int64();
  REQUIRE(c3 == int64(3));

  auto const c5_sz = v5.bytes();
  REQUIRE(c5_sz == ss.length());

  auto const c5 = v5.text();
  auto const c5_ascii = utf8_to_ascii(c5);
  REQUIRE(string_span(c5_ascii) == ss);

  auto const r2 = s3.step();
  REQUIRE(r2 == result::done);
}
