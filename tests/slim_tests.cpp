// tests/slim_tests.cpp

#include <slim/SlimValue.hpp>
#include <catch2/catch_test_macros.hpp>
#include <string>
#include <string_view>

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

TEST_CASE("Multi-map string_view set") {
	slim::SlimValue v;

	auto& map = v.get_multi_map("string_view");

	map.set(std::string_view("key"), std::string_view("value"));

	REQUIRE(map.has("key"));

	auto range = map.get("key");
	REQUIRE(range.first != range.second);
}

TEST_CASE("Multi-map debug output for non-unique key values") {
	slim::SlimValue v;

	auto& map = v.get_multi_map("key");

	map.set("inner", 1);
	map.set("inner", 2);
	map.set("inner", 3);

	auto range = map.get("inner");

	std::size_t count = 0;
	for(auto it = range.first; it != range.second; ++it) {
		++count;
	}

	REQUIRE(count == 3);
}
