// tests/slim_tests.cpp

#include <slim/SlimValue.hpp>

#include <catch2/catch_test_macros.hpp>
#include <string>

TEST_CASE("SlimValue default construction has no error") {
	slim::SlimValue v;

	REQUIRE(!v.has_error());
}

TEST_CASE("SlimValue stores and retrieves simple value") {
	slim::SlimValue v(42);

	REQUIRE(!v.has_error());
	REQUIRE(v.get<int>() == 42);
}

TEST_CASE("SlimValue string handling") {
	slim::SlimValue v(std::string("hello"));

	REQUIRE(v.get<std::string>() == "hello");
}

TEST_CASE("SlimValue error state construction") {
	slim::SlimValue v = slim::ErrorInfo(1, "failure");

	REQUIRE(v.has_error());
}

TEST_CASE("Multi-map lazy initialization") {
	slim::SlimValue v;

	auto& map = v.get_multi_map("key");
	map.set("inner", 123);

	REQUIRE(map.has("inner"));
}

TEST_CASE("Multi-map persistence across calls") {
	slim::SlimValue v;

	auto& map1 = v.get_multi_map("key");
	map1.set("a", 1);

	auto& map2 = v.get_multi_map("key");

	REQUIRE(map2.has("a"));
}