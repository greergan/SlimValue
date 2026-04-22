#include <string_view>
#include <slim/SlimValue.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("SlimValue int assignment from SlimValue in declaration") {
	slim::SlimValue a = 42;
	REQUIRE(a == 42);
	slim::SlimValue b = a;
	REQUIRE(b == a);
	REQUIRE(b == 42);
}

TEST_CASE("SlimValue string assignment from SlimValue in declaration") {
	slim::SlimValue a = "my new world";
	REQUIRE(a == "my new world");
	slim::SlimValue b = a;
	REQUIRE(b == a);
	REQUIRE(b == "my new world");
	a = "is awesome";
	REQUIRE(a == "is awesome");
	REQUIRE(b == "my new world");
}