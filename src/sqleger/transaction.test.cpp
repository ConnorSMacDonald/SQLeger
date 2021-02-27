#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/sqleger.hpp>


using namespace sqleger;
using namespace sqleger::string_span_literals;


TEST_CASE("A transaction can be used as an RAII interface", "[transaction]")
{
  SECTION("commit")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER)"_ss);
    REQUIRE(s1.step() == result::done);

    {
      auto t = transaction(d);

      auto s2 = stmt(d, "INSERT INTO t VALUES(1)"_ss);
      REQUIRE(s2.step() == result::done);

      t.commit();
    }

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);
    REQUIRE(s3.step() == result::done);
  }

  SECTION("rollback")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER)"_ss);
    REQUIRE(s1.step() == result::done);

    {
      auto t = transaction(d);

      auto s2 = stmt(d, "INSERT INTO t VALUES(1)"_ss);
      REQUIRE(s2.step() == result::done);
    }

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::done);
  }

  SECTION("immediate commit")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER)"_ss);
    REQUIRE(s1.step() == result::done);

    auto t = transaction(d);

    auto s2 = stmt(d, "INSERT INTO t VALUES(1)"_ss);
    REQUIRE(s2.step() == result::done);

    auto const r = t.commit_now();
    REQUIRE(is_non_error(r));
    REQUIRE(t.db_handle().c_ptr() == nullptr);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::row);
    REQUIRE(s3.step() == result::done);
  }

  SECTION("immediate rollback")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER)"_ss);
    REQUIRE(s1.step() == result::done);

    auto t = transaction(d);

    auto s2 = stmt(d, "INSERT INTO t VALUES(1)"_ss);
    REQUIRE(s2.step() == result::done);

    auto const r = t.rollback_now();
    REQUIRE(is_non_error(r));
    REQUIRE(t.db_handle().c_ptr() == nullptr);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result::done);
  }
}
