#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sqleger/sqleger.hpp>


using namespace sqleger;
using namespace sqleger::string_span_literals;
using namespace sqleger::zstring_view_literals;


TEST_CASE("for_each_row", "[for_each_row]")
{
  SECTION("empty result set")
  {
    auto d = db(":memory:"_zv);

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "SELECT x FROM t"_ss);

    auto const r = for_each_row(
      s2, [](auto) { FAIL("this function should not have been invoked"); });

    REQUIRE(r == result::done);
  }

  SECTION("one row result set")
  {
    auto d = db(":memory:"_zv);

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(1)"_ss);
    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);

    std::vector<int> vec;

    auto const r = for_each_row(s3, [&](auto const ro) {
      REQUIRE(ro.get_stmt_ref().c_ptr() == s3.c_ptr());
      vec.push_back(column<int>(ro));
    });

    REQUIRE(vec.size() == 1);
    REQUIRE(vec.at(0) == 1);
    REQUIRE(r == result::done);
  }

  SECTION("two row result set")
  {
    auto d = db(":memory:"_zv);

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    REQUIRE(bind(s2, 1) == bind_result {result::ok, 1});
    REQUIRE(s2.step() == result::done);
    REQUIRE(s2.reset() == result::ok);
    REQUIRE(bind(s2, 2) == bind_result {result::ok, 1});
    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);

    std::vector<int> vec;

    auto const r = for_each_row(s3, [&](auto const ro) {
      REQUIRE(ro.get_stmt_ref().c_ptr() == s3.c_ptr());
      vec.push_back(column<int>(ro));
    });

    REQUIRE(vec.size() == 2);
    REQUIRE(vec.at(0) == 1);
    REQUIRE(vec.at(1) == 2);
    REQUIRE(r == result::done);
  }

  SECTION("error")
  {
    auto d = db(":memory:"_zv);

    auto s1 = stmt(d, "CREATE TABLE t(x INTEGER)"_ss);
    REQUIRE(s1.step() == result::done);

    auto s2 = stmt(d, "INSERT INTO t VALUES(?1)"_ss);

    REQUIRE(bind(s2, 1) == bind_result {result::ok, 1});
    REQUIRE(s2.step() == result::done);

    auto s3 = stmt(d, "SELECT x FROM t"_ss);

    auto callback_already_invoked = false;

    auto const r = for_each_row(s3, [&](auto const ro) {
      REQUIRE(ro.get_stmt_ref().c_ptr() == s3.c_ptr());

      REQUIRE_FALSE(callback_already_invoked);
      callback_already_invoked = true;

      // Finalize the statement within the callback such that step returns an
      // error on the next for_each_row iteration

      auto const r1 = s3.step();
      REQUIRE(r1 == result::done);

      auto const r2 = s3.finalize();
      REQUIRE(r2 == result::ok);
    });
    REQUIRE(is_error(r));
  }
}
