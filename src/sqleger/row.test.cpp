#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/sqleger.hpp>


using namespace sqleger;
using namespace sqleger::string_span_literals;
using namespace sqleger::zstring_view_literals;


TEST_CASE("Constructors", "[row]")
{
  SECTION("default")
  {
    row const ro;

    auto const s_ref = ro.get_stmt_ref();
    REQUIRE_FALSE(s_ref);

    REQUIRE_FALSE(ro);
  }

  SECTION("stmt_ref")
  {
    auto d = db(":memory:"_zv);

    auto s = stmt(d, "CREATE TABLE t(x INTEGER)"_ss);
    REQUIRE(s);

    auto const ro = row(s);

    auto const s_ref = ro.get_stmt_ref();
    REQUIRE(s_ref.c_ptr() == s.c_ptr());

    REQUIRE(ro);
  }
}

TEST_CASE("Columns", "[row]")
{
  auto d = db(":memory:"_zv);

  auto s1 = stmt(d,
                 "CREATE TABLE t("
                 "a BLOB NOT NULL,"
                 "b REAL NOT NULL,"
                 "c INTEGER NOT NULL,"
                 "d INTEGER NOT NULL,"
                 "e INTEGER,"
                 "f TEXT NOT NULL,"
                 "g INTEGER)"_ss);
  REQUIRE(s1.step() == result::done);

  auto s2 = stmt(d, "INSERT INTO t VALUES(?1, ?2, ?3, ?4, ?5, ?6, ?7)"_ss);

  std::vector<uint64_t> const v = {1, 2, 3, 4, 5};
  constexpr auto ss = " vqlflz.tlue VPNRE103-====++++"_ss;

  REQUIRE(s2.bind_blob(1, v) == result::ok);
  REQUIRE(s2.bind_double(2, 0.25) == result::ok);
  REQUIRE(s2.bind_int(3, 2) == result::ok);
  REQUIRE(s2.bind_int64(4, 3) == result::ok);
  REQUIRE(s2.bind_null(5) == result::ok);
  REQUIRE(s2.bind_text(6, ss) == result::ok);
  REQUIRE(s2.bind_null(7) == result::ok);
  REQUIRE(s2.step() == result::done);

  auto s3 = stmt(d, "SELECT a, b, c, d, e, f, g FROM t"_ss);

  auto const r1 = s3.step();
  REQUIRE(r1 == result::row);

  auto ro = row(s3);

  auto const dt0 = ro.column_type(0);
  REQUIRE(dt0 == datatype::blob);

  auto const dt1 = ro.column_type(1);
  REQUIRE(dt1 == datatype::_float);

  auto const dt2 = ro.column_type(2);
  REQUIRE(dt2 == datatype::integer);

  auto const dt3 = ro.column_type(3);
  REQUIRE(dt3 == datatype::integer);

  auto const dt4 = ro.column_type(4);
  REQUIRE(dt4 == datatype::null);

  auto const dt5 = ro.column_type(5);
  REQUIRE(dt5 == datatype::text);

  auto const dt6 = ro.column_type(6);
  REQUIRE(dt6 == datatype::null);

  auto const c0_sz = ro.column_bytes(0);
  REQUIRE(c0_sz == 40);

  auto const c0 = ro.column_blob(0);
  auto const c0_v = c0.as_vector<uint64_t>(v.size());
  REQUIRE(c0_v == v);

  auto const c1 = ro.column_double(1);
  REQUIRE(c1 == 0.25);

  auto const c2 = ro.column_int(2);
  REQUIRE(c2 == 2);

  auto const c3 = ro.column_int64(3);
  REQUIRE(c3 == int64(3));

  auto const c5_sz = ro.column_bytes(5);
  REQUIRE(c5_sz == ss.length());

  auto const c5 = ro.column_text(5);
  auto const c5_ascii = utf8_to_ascii(c5);
  REQUIRE(string_span(c5_ascii) == ss);

  auto c6 = ro.column_value(6);
  REQUIRE(c6.type() == datatype::null);
}
