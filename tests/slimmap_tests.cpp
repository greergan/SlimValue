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

TEST_CASE("SlimMap const get() returns underlying map", "[SlimMap][const][get]") {
	slim::SlimMap m;
	m.set(std::string_view("key"), 42);

	const slim::SlimMap& cm = m;
	const auto& raw = cm.get();

	REQUIRE(raw.size() == 1);
	REQUIRE(raw.at("key") == 42);
}

TEST_CASE("SlimMap const and non-const get() are consistent", "[SlimMap][const][get]") {
	slim::SlimMap m;
	m.set(std::string_view("x"), std::string_view("hello"));

	const slim::SlimMap& cm = m;

	REQUIRE(m.get().at("x") == std::string("hello"));
	REQUIRE(cm.get().at("x") == std::string("hello"));
}

TEST_CASE("SlimMap const get() reflects mutations made via non-const", "[SlimMap][const][get]") {
	slim::SlimMap m;
	const slim::SlimMap& cm = m;

	m.set(std::string_view("a"), 1);
	m.set(std::string_view("b"), 2);

	const auto& raw = cm.get();
	REQUIRE(raw.size() == 2);
	REQUIRE(raw.at("a") == 1);
	REQUIRE(raw.at("b") == 2);
}

TEST_CASE("SlimMap const get() on empty map returns empty map", "[SlimMap][const][get]") {
	slim::SlimMap m;
	const slim::SlimMap& cm = m;

	REQUIRE(cm.get().empty());
}

TEST_CASE("SlimMap const get() allows iteration", "[SlimMap][const][get]") {
	slim::SlimMap m;
	m.set(std::string_view("p"), 10);
	m.set(std::string_view("q"), 20);

	const slim::SlimMap& cm = m;
	std::size_t count = 0;
	for (const auto& [k, v] : cm.get()) {
		++count;
		REQUIRE((k == "p" || k == "q"));
	}
	REQUIRE(count == 2);
}

TEST_CASE("SlimValue const get_map() returns const SlimMap", "[SlimValue][get_map][const]") {
	slim::SlimValue val;
	val.get_map("mymap").set(std::string_view("k"), 7);

	const slim::SlimValue& cval = val;
	const slim::SlimMap& cmap = cval.get_map("mymap");

	REQUIRE(cmap.get(std::string_view("k")) == 7);
}

TEST_CASE("SlimValue const get_map() allows iterating via get()", "[SlimValue][get_map][const]") {
	slim::SlimValue val;
	val.get_map("data").set(std::string_view("a"), 1);
	val.get_map("data").set(std::string_view("b"), 2);

	const slim::SlimValue& cval = val;
	const auto& raw = cval.get_map("data").get();

	REQUIRE(raw.size() == 2);
	REQUIRE(raw.at("a") == 1);
	REQUIRE(raw.at("b") == 2);
}

TEST_CASE("SlimValue const get_map() throws on missing key", "[SlimValue][get_map][const]") {
	slim::SlimValue val;
	val.get_map("present");

	const slim::SlimValue& cval = val;
	REQUIRE_THROWS_AS(cval.get_map("missing"), std::out_of_range);
}

TEST_CASE("SlimValue const get_map() throws when no maps initialized", "[SlimValue][get_map][const]") {
	const slim::SlimValue cval;
	REQUIRE_THROWS(cval.get_map("anything"));
}

TEST_CASE("SlimValue mutable get_map() creates on demand and persists", "[SlimValue][get_map]") {
	slim::SlimValue val;
	REQUIRE_FALSE(val.has_map("mymap"));

	auto& m = val.get_map("mymap");
	m.set(std::string_view("k"), 99);

	REQUIRE(val.has_map("mymap"));
	REQUIRE(val.get_map("mymap").get(std::string_view("k")) == 99);
}