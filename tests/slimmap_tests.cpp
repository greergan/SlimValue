#include <slim/SlimValue.hpp>
#include <catch2/catch_test_macros.hpp>
#include <string>
#include <string_view>

TEST_CASE("SlimMap default state") {
	slim::SlimMap m;

	REQUIRE(m.size() == 0);
	REQUIRE_FALSE(m.has(std::string_view("key")));
}

TEST_CASE("SlimMap set and get value") {
	slim::SlimMap m;

	m.set(std::string_view("key"), 42);

	REQUIRE(m.has(std::string_view("key")));
	REQUIRE(m.get(std::string_view("key")).get<int>() == 42);
}

TEST_CASE("SlimMap overwrite value") {
	slim::SlimMap m;

	m.set(std::string_view("key"), 1);
	m.set(std::string_view("key"), 2);

	REQUIRE(m.get(std::string_view("key")).get<int>() == 2);
}

TEST_CASE("SlimMap string_view insertion") {
	slim::SlimMap m;

	m.set(std::string_view("key"), std::string_view("value"));

	REQUIRE(m.has(std::string_view("key")));
	REQUIRE(m.get(std::string_view("key")).get<std::string>() == "value");
}

TEST_CASE("SlimMap remove key") {
	slim::SlimMap m;

	m.set(std::string_view("key"), 123);
	m.remove(std::string_view("key"));

	REQUIRE_FALSE(m.has(std::string_view("key")));
}

TEST_CASE("SlimMap clear") {
	slim::SlimMap m;

	m.set(std::string_view("a"), 1);
	m.set(std::string_view("b"), 2);
	REQUIRE(m.size() == 2);

	m.clear();

	REQUIRE(m.size() == 0);
	REQUIRE_FALSE(m.has(std::string_view("a")));
	REQUIRE_FALSE(m.has(std::string_view("b")));
}

TEST_CASE("SlimMap multiple types") {
	slim::SlimMap m;

	m.set(std::string_view("int"), 1);
	m.set(std::string_view("double"), 3.14);
	m.set(std::string_view("string"), std::string_view("hello"));

	REQUIRE(m.get(std::string_view("int")).get<int>() == 1);
	REQUIRE(m.get(std::string_view("double")).get<double>() == 3.14);
	REQUIRE(m.get(std::string_view("string")).get<std::string>() == "hello");
}