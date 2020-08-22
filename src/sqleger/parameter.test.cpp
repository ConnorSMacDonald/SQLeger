#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/db.hpp>
#include <sqleger/parameter.hpp>


using namespace sqleger;
using namespace sqleger::zstring_view_literals;
using namespace sqleger::string_span_literals;


TEST_CASE("A parameter holds a stmt ref and an index", "[parameter]")
{
  auto d = db(":memory:");

  auto s1 = stmt(d, "CREATE TABLE t(x INTEGER)"_ss);
  REQUIRE(s1.step() == result::done);

  auto s2 = stmt(d, "INSERT INTO t VALUES(?1)");
  REQUIRE(s2.step() == result::done);

  const auto p = parameter(s2, 1);

  REQUIRE(p.stmt_ref().c_ptr() == s2.c_ptr());
  REQUIRE(p.index() == 1);
}

TEST_CASE("Data can be bound to a parameter", "[parameter]")
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

  auto s2 = stmt(d, "INSERT INTO t VALUES(?1, ?2, ?3, ?4, ?5, ?6)");

  auto p1 = parameter(s2, 1);
  auto p2 = parameter(s2, 2);
  auto p3 = parameter(s2, 3);
  auto p4 = parameter(s2, 4);
  auto p5 = parameter(s2, 5);
  auto p6 = parameter(s2, 6);

  const std::vector<uint64_t> v = {1, 2, 3, 4, 5};

  const auto r1 = p1.bind_blob(v);
  REQUIRE(r1 == result::ok);

  const auto r2 = p2.bind_double(0.25);
  REQUIRE(r2 == result::ok);

  const auto r3 = p3.bind_int(2);
  REQUIRE(r3 == result::ok);

  const auto r4 = p4.bind_int64(3);
  REQUIRE(r4 == result::ok);

  const auto r5 = p5.bind_null();
  REQUIRE(r5 == result::ok);

  const auto r6 = p6.bind_text({"aal;sdfkjjk aAAV NFLS4E352"});
  REQUIRE(r6 == result::ok);

  const auto r7 = s2.step();
  REQUIRE(r7 == result::done);
}
