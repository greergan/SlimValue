#include <slim/SlimValue.hpp>
#include <catch2/catch_test_macros.hpp>
#include <string>
#include <string_view>
#include <unordered_set>

// ─── Default state ────────────────────────────────────────────────────────────

TEST_CASE("SlimMultiMap default state", "[SlimMultiMap]") {
	slim::SlimMultiMap mm;

	REQUIRE(mm.size() == 0);
	REQUIRE_FALSE(mm.has("key"));
}

// ─── set / has ────────────────────────────────────────────────────────────────

TEST_CASE("SlimMultiMap set and has with string_view key", "[SlimMultiMap][set]") {
	slim::SlimMultiMap mm;
	mm.set(std::string_view("tag"), std::string_view("value"));

	REQUIRE(mm.has("tag"));
}

TEST_CASE("SlimMultiMap set int value", "[SlimMultiMap][set]") {
	slim::SlimMultiMap mm;
	mm.set(std::string_view("k"), 42);

	REQUIRE(mm.has("k"));
}

TEST_CASE("SlimMultiMap set double value", "[SlimMultiMap][set]") {
	slim::SlimMultiMap mm;
	mm.set(std::string_view("k"), 3.14);

	REQUIRE(mm.has("k"));
	auto [begin, end] = mm.get(std::string_view("k"));
	REQUIRE(begin != end);
	REQUIRE(begin->second == 3.14);
}

TEST_CASE("SlimMultiMap set bool value", "[SlimMultiMap][set]") {
	slim::SlimMultiMap mm;
	mm.set(std::string_view("flag"), true);

	REQUIRE(mm.has("flag"));
	auto [begin, end] = mm.get(std::string_view("flag"));
	REQUIRE(begin->second == true);
}

TEST_CASE("SlimMultiMap set AnyValue rvalue", "[SlimMultiMap][set]") {
	slim::SlimMultiMap mm;
	mm.set(std::string_view("k"), slim::AnyValue(99));

	REQUIRE(mm.has("k"));
	auto [begin, end] = mm.get(std::string_view("k"));
	REQUIRE(begin->second == 99);
}

TEST_CASE("SlimMultiMap set string value via string_view", "[SlimMultiMap][set]") {
	slim::SlimMultiMap mm;
	mm.set(std::string_view("key"), std::string_view("value"));

	auto [begin, end] = mm.get(std::string_view("key"));
	REQUIRE(begin != end);
	REQUIRE(begin->second == std::string("value"));
}

// ─── Multiple values per key ──────────────────────────────────────────────────

TEST_CASE("SlimMultiMap allows multiple values for the same key", "[SlimMultiMap][multi]") {
	slim::SlimMultiMap mm;
	mm.set(std::string_view("x"), 1);
	mm.set(std::string_view("x"), 2);
	mm.set(std::string_view("x"), 3);

	REQUIRE(mm.size() == 3);

	auto [begin, end] = mm.get(std::string_view("x"));
	int count = 0;
	for (auto it = begin; it != end; ++it)
		++count;
	REQUIRE(count == 3);
}

TEST_CASE("SlimMultiMap all inserted values are retrievable", "[SlimMultiMap][multi]") {
	slim::SlimMultiMap mm;
	mm.set(std::string_view("n"), 10);
	mm.set(std::string_view("n"), 20);
	mm.set(std::string_view("n"), 30);

	auto [begin, end] = mm.get(std::string_view("n"));
	std::unordered_set<int> values;
	for (auto it = begin; it != end; ++it)
		values.insert(it->second.get_int());

	REQUIRE(values.count(10) == 1);
	REQUIRE(values.count(20) == 1);
	REQUIRE(values.count(30) == 1);
}

TEST_CASE("SlimMultiMap multiple keys are independent", "[SlimMultiMap][multi]") {
	slim::SlimMultiMap mm;
	mm.set(std::string_view("a"), 1);
	mm.set(std::string_view("a"), 2);
	mm.set(std::string_view("b"), 9);

	REQUIRE(mm.size() == 3);

	auto [ab, ae] = mm.get(std::string_view("a"));
	int acount = 0;
	for (auto it = ab; it != ae; ++it) ++acount;
	REQUIRE(acount == 2);

	auto [bb, be] = mm.get(std::string_view("b"));
	int bcount = 0;
	for (auto it = bb; it != be; ++it) ++bcount;
	REQUIRE(bcount == 1);
}

TEST_CASE("SlimMultiMap mixed types under same key", "[SlimMultiMap][multi]") {
	slim::SlimMultiMap mm;
	mm.set(std::string_view("mixed"), 42);
	mm.set(std::string_view("mixed"), std::string_view("hello"));
	mm.set(std::string_view("mixed"), true);

	REQUIRE(mm.size() == 3);

	auto [begin, end] = mm.get(std::string_view("mixed"));
	int count = 0;
	for (auto it = begin; it != end; ++it) ++count;
	REQUIRE(count == 3);
}

// ─── create ───────────────────────────────────────────────────────────────────

TEST_CASE("SlimMultiMap create returns writable reference", "[SlimMultiMap][create]") {
	slim::SlimMultiMap mm;
	mm.create("y") = slim::AnyValue(99);

	REQUIRE(mm.has("y"));
	auto [begin, end] = mm.get(std::string_view("y"));
	REQUIRE(begin->second == 99);
}

TEST_CASE("SlimMultiMap create adds a new entry each call", "[SlimMultiMap][create]") {
	slim::SlimMultiMap mm;
	mm.create("k") = slim::AnyValue(1);
	mm.create("k") = slim::AnyValue(2);

	REQUIRE(mm.size() == 2);
}

// ─── get (range) ──────────────────────────────────────────────────────────────

TEST_CASE("SlimMultiMap get returns empty range for missing key", "[SlimMultiMap][get]") {
	slim::SlimMultiMap mm;
	auto [begin, end] = mm.get(std::string_view("nope"));
	REQUIRE(begin == end);
}

TEST_CASE("SlimMultiMap get range covers all values for key", "[SlimMultiMap][get]") {
	slim::SlimMultiMap mm;
	mm.set(std::string_view("v"), 5);
	mm.set(std::string_view("v"), 6);

	auto [begin, end] = mm.get(std::string_view("v"));
	int count = 0;
	for (auto it = begin; it != end; ++it) ++count;
	REQUIRE(count == 2);
}

TEST_CASE("SlimMultiMap const get returns const iterator range", "[SlimMultiMap][get][const]") {
	slim::SlimMultiMap mm;
	mm.set(std::string_view("c"), 1);
	mm.set(std::string_view("c"), 2);

	const slim::SlimMultiMap& cmm = mm;
	auto [begin, end] = cmm.get(std::string_view("c"));
	int count = 0;
	for (auto it = begin; it != end; ++it) ++count;
	REQUIRE(count == 2);
}

TEST_CASE("SlimMultiMap const get returns empty range for missing key", "[SlimMultiMap][get][const]") {
	const slim::SlimMultiMap cmm;
	auto [begin, end] = cmm.get(std::string_view("nope"));
	REQUIRE(begin == end);
}

// ─── get() const — raw multimap access ───────────────────────────────────────

TEST_CASE("SlimMultiMap const get() returns underlying multimap", "[SlimMultiMap][get][const]") {
	slim::SlimMultiMap mm;
	mm.set(std::string_view("key"), 1);
	mm.set(std::string_view("key"), 2);

	const slim::SlimMultiMap& cmm = mm;
	const auto& raw = cmm.get();

	REQUIRE(raw.size() == 2);
	REQUIRE(raw.count("key") == 2);
}

TEST_CASE("SlimMultiMap const and non-const get() are consistent", "[SlimMultiMap][get][const]") {
	slim::SlimMultiMap mm;
	mm.set(std::string_view("a"), 10);

	const slim::SlimMultiMap& cmm = mm;
	REQUIRE(mm.get().count("a") == 1);
	REQUIRE(cmm.get().count("a") == 1);
}

TEST_CASE("SlimMultiMap const get() reflects live mutations", "[SlimMultiMap][get][const]") {
	slim::SlimMultiMap mm;
	const slim::SlimMultiMap& cmm = mm;

	mm.set(std::string_view("x"), 1);
	REQUIRE(cmm.get().size() == 1);

	mm.set(std::string_view("x"), 2);
	REQUIRE(cmm.get().size() == 2);
}

TEST_CASE("SlimMultiMap const get() on empty map returns empty", "[SlimMultiMap][get][const]") {
	const slim::SlimMultiMap cmm;
	REQUIRE(cmm.get().empty());
}

TEST_CASE("SlimMultiMap const get() allows iteration over all entries", "[SlimMultiMap][get][const]") {
	slim::SlimMultiMap mm;
	mm.set(std::string_view("p"), 10);
	mm.set(std::string_view("p"), 20);
	mm.set(std::string_view("q"), 30);

	const slim::SlimMultiMap& cmm = mm;
	std::size_t count = 0;
	for (const auto& [k, v] : cmm.get()) {
		REQUIRE((k == "p" || k == "q"));
		++count;
	}
	REQUIRE(count == 3);
}

// ─── remove ───────────────────────────────────────────────────────────────────

TEST_CASE("SlimMultiMap remove erases all values for a key", "[SlimMultiMap][remove]") {
	slim::SlimMultiMap mm;
	mm.set(std::string_view("k"), 1);
	mm.set(std::string_view("k"), 2);
	mm.remove(std::string_view("k"));

	REQUIRE_FALSE(mm.has("k"));
	REQUIRE(mm.size() == 0);
}

TEST_CASE("SlimMultiMap remove only affects the targeted key", "[SlimMultiMap][remove]") {
	slim::SlimMultiMap mm;
	mm.set(std::string_view("a"), 1);
	mm.set(std::string_view("b"), 2);
	mm.remove(std::string_view("a"));

	REQUIRE_FALSE(mm.has("a"));
	REQUIRE(mm.has("b"));
	REQUIRE(mm.size() == 1);
}

TEST_CASE("SlimMultiMap remove on missing key is a no-op", "[SlimMultiMap][remove]") {
	slim::SlimMultiMap mm;
	mm.set(std::string_view("x"), 1);
	mm.remove(std::string_view("nope"));

	REQUIRE(mm.size() == 1);
}

// ─── size / clear ─────────────────────────────────────────────────────────────

TEST_CASE("SlimMultiMap size counts all entries not unique keys", "[SlimMultiMap][size]") {
	slim::SlimMultiMap mm;
	mm.set(std::string_view("a"), 1);
	mm.set(std::string_view("a"), 2);
	mm.set(std::string_view("b"), 3);

	REQUIRE(mm.size() == 3);
}

TEST_CASE("SlimMultiMap clear removes all entries", "[SlimMultiMap][clear]") {
	slim::SlimMultiMap mm;
	mm.set(std::string_view("a"), 1);
	mm.set(std::string_view("b"), 2);
	mm.clear();

	REQUIRE(mm.size() == 0);
	REQUIRE_FALSE(mm.has("a"));
	REQUIRE_FALSE(mm.has("b"));
}

TEST_CASE("SlimMultiMap clear on empty map is a no-op", "[SlimMultiMap][clear]") {
	slim::SlimMultiMap mm;
	mm.clear();
	REQUIRE(mm.size() == 0);
}

// ─── SlimValue — get_multi_map integration ───────────────────────────────────

TEST_CASE("SlimValue get_multi_map creates on demand", "[SlimValue][SlimMultiMap]") {
	slim::SlimValue val;
	REQUIRE_FALSE(val.has_multi_maps());

	auto& mm = val.get_multi_map("tags");
	REQUIRE(val.has_multi_maps());
	REQUIRE(val.has_multi_map("tags"));
	(void)mm;
}

TEST_CASE("SlimValue get_multi_map persists across calls", "[SlimValue][SlimMultiMap]") {
	slim::SlimValue val;
	val.get_multi_map("tags").set(std::string_view("color"), std::string_view("red"));
	val.get_multi_map("tags").set(std::string_view("color"), std::string_view("blue"));

	REQUIRE(val.get_multi_map("tags").size() == 2);
}

TEST_CASE("SlimValue has_multi_map returns false for absent map", "[SlimValue][SlimMultiMap]") {
	slim::SlimValue val;
	REQUIRE_FALSE(val.has_multi_map("nope"));
}

TEST_CASE("SlimValue multiple named multi_maps are independent", "[SlimValue][SlimMultiMap]") {
	slim::SlimValue val;
	val.get_multi_map("a").set(std::string_view("k"), 1);
	val.get_multi_map("a").set(std::string_view("k"), 2);
	val.get_multi_map("b").set(std::string_view("k"), 9);

	REQUIRE(val.get_multi_map("a").size() == 2);
	REQUIRE(val.get_multi_map("b").size() == 1);
}