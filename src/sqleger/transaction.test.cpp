#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/db.hpp>
#include <sqleger/transaction.hpp>


using namespace sqleger;
using namespace sqleger::string_span_literals;


TEST_CASE("A transaction can be used as an RAII interface", "[transaction]")
{
  SECTION("commit")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER)"_ss);
    REQUIRE(s1.step() == result_t::done);

    {
      auto t = transaction(d);

      auto s2 = stmt(d, "INSERT INTO t VALUES(1)"_ss);
      REQUIRE(s2.step() == result_t::done);

      t.commit();
    }

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result_t::row);
    REQUIRE(s3.step() == result_t::done);
  }

  SECTION("rollback")
  {
    auto d = db(":memory:");

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER)"_ss);
    REQUIRE(s1.step() == result_t::done);

    {
      auto t = transaction(d);

      auto s2 = stmt(d, "INSERT INTO t VALUES(1)"_ss);
      REQUIRE(s2.step() == result_t::done);
    }

    auto s3 = stmt(d, "SELECT x FROM t"_ss);
    REQUIRE(s3.step() == result_t::done);
  }
}
