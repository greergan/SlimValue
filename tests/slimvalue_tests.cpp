#include <string_view>
#include <slim/SlimValue.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("SlimValue assingment from SlimValue in declaration") {
	slim::SlimValue a = 42;
	REQUIRE(a == 42);
	slim::SlimValue b = a;
	REQUIRE(b == a);
}