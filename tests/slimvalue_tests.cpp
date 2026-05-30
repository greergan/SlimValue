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

TEST_CASE("SlimValue set_error with code only") {
	slim::SlimValue v = 42;
	v.set_error(404);
	REQUIRE(v.has_error());
	REQUIRE(v.get_error().has_error_code());
	REQUIRE(v.get_error().code() == 404);
	REQUIRE(!v.get_error().has_error_message());
}

TEST_CASE("SlimValue set_error with message only") {
	slim::SlimValue v = 42;
	v.set_error("something went wrong");
	REQUIRE(v.has_error());
	REQUIRE(v.get_error().has_error_message());
	REQUIRE(v.get_error().message() == "something went wrong");
	REQUIRE(!v.get_error().has_error_code());
}

TEST_CASE("SlimValue set_error with code and message") {
	slim::SlimValue v = 42;
	v.set_error(500, "internal error");
	REQUIRE(v.has_error());
	REQUIRE(v.get_error().has_error_code());
	REQUIRE(v.get_error().code() == 500);
	REQUIRE(v.get_error().has_error_message());
	REQUIRE(v.get_error().message() == "internal error");
}

TEST_CASE("SlimValue set_error does not clobber value") {
	slim::SlimValue v = 99;
	v.set_error(1, "oops");
	REQUIRE(v.has_value());
	REQUIRE(v == 99);
	REQUIRE(v.has_error());
}

TEST_CASE("SlimValue set_error overwrite replaces previous error") {
	slim::SlimValue v = 0;
	v.set_error(1, "first");
	v.set_error(2, "second");
	REQUIRE(v.get_error().code() == 2);
	REQUIRE(v.get_error().message() == "second");
}