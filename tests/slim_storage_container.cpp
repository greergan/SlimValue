#include <catch2/catch_test_macros.hpp>
#include <slim/SlimValue.hpp>

using namespace slim;

TEST_CASE("AnyValue stores std::vector<uint8_t> using constructor", "[AnyValue][slim_storage_container]") {
	std::vector<uint8_t> storage = {0x41, 0x42, 0x43};
	AnyValue value(storage);
	REQUIRE(value.has_value());
	REQUIRE(value.is<slim_storage_container>());
	const auto& result = value.get_storage_container();
	REQUIRE(result.size() == 3);
	CHECK(result[0] == 0x41);
	CHECK(result[1] == 0x42);
	CHECK(result[2] == 0x43);
}

TEST_CASE("AnyValue stores std::vector<uint8_t> using assignment", "[AnyValue][slim_storage_container]") {
	std::vector<uint8_t> storage = {0x41, 0x42, 0x43};
	AnyValue value = storage;
	REQUIRE(value.has_value());
	REQUIRE(value.is<slim_storage_container>());
	const auto& result = value.get_storage_container();
	REQUIRE(result.size() == 3);
	CHECK(result[0] == 0x41);
	CHECK(result[1] == 0x42);
	CHECK(result[2] == 0x43);
}

TEST_CASE("AnyValue to_string from slim_storage_container", "[AnyValue][slim_storage_container]") {
	slim_storage_container storage = {'A', 'B', 'C'};
	AnyValue value = storage;
	REQUIRE(value.to_string() == "ABC");
}

TEST_CASE("AnyValue bool conversion from slim_storage_container", "[AnyValue][slim_storage_container]") {
	SECTION("non-empty container") {
		slim_storage_container storage{1};
		AnyValue value(storage);
		REQUIRE(static_cast<bool>(value));
	}

	SECTION("empty container") {
		std::vector<uint8_t> storage;
		AnyValue value = storage;
		REQUIRE_FALSE(static_cast<bool>(value));
	}
}

TEST_CASE("AnyValue equality with slim_storage_container", "[AnyValue][slim_storage_container]") {
	slim_storage_container storage = {1, 2, 3};
	AnyValue value(storage);
	REQUIRE(value == storage);
	CHECK_FALSE(value != storage);
}

TEST_CASE("SlimValue stores slim_storage_container", "[SlimValue][slim_storage_container]") {
	SlimValue value(slim_storage_container {10, 20, 30});
	REQUIRE(value.has_value());
	REQUIRE(value.is<slim_storage_container>());
	const auto& storage = value.get_storage_container();
	REQUIRE(storage.size() == 3);
	CHECK(storage[0] == 10);
	CHECK(storage[1] == 20);
	CHECK(storage[2] == 30);
}

TEST_CASE("SlimValue stores std::vector<uint8_t> using constructor", "[SlimValue][slim_storage_container]") {
	std::vector<uint8_t> storage = {0x41, 0x42, 0x43};
	AnyValue value(storage);
	REQUIRE(value.has_value());
	REQUIRE(value.is<slim_storage_container>());
	const auto& result = value.get_storage_container();
	REQUIRE(result.size() == 3);
	CHECK(result[0] == 0x41);
	CHECK(result[1] == 0x42);
	CHECK(result[2] == 0x43);
}

TEST_CASE("SlimValue stores std::vector<uint8_t> using assignment", "[SlimValue][slim_storage_container]") {
	std::vector<uint8_t> storage = {0x41, 0x42, 0x43};
	AnyValue value = storage;
	REQUIRE(value.has_value());
	REQUIRE(value.is<slim_storage_container>());
	const auto& result = value.get_storage_container();
	REQUIRE(result.size() == 3);
	CHECK(result[0] == 0x41);
	CHECK(result[1] == 0x42);
	CHECK(result[2] == 0x43);
}

TEST_CASE("SlimValue try_storage_container", "[SlimValue][slim_storage_container]") {
	slim_storage_container storage{4, 5,6};
	SlimValue value = storage;
	auto result = value.try_storage_container();
	REQUIRE(result.has_value());
	CHECK((*result)[0] == 4);
	CHECK((*result)[1] == 5);
	CHECK((*result)[2] == 6);
}

TEST_CASE("SlimValue try_storage_container wrong type", "[SlimValue][slim_storage_container]") {
	SlimValue value(123);
	auto result = value.try_storage_container();
	REQUIRE_FALSE(result.has_value());
}

TEST_CASE("SlimValue assignment from slim_storage_container", "[SlimValue][slim_storage_container]") {
	SlimValue value;
	value = slim_storage_container {0xAA, 0xBB};
	REQUIRE(value.is<slim_storage_container>());
	const auto& storage = value.get_storage_container();
	REQUIRE(storage.size() == 2);
	CHECK(storage[0] == 0xAA);
	CHECK(storage[1] == 0xBB);
}

TEST_CASE("SlimValue equality with slim_storage_container", "[SlimValue][slim_storage_container]") {
	slim_storage_container storage = {11, 22, 33};
	SlimValue value(storage);
	REQUIRE(value == storage);
	CHECK_FALSE(value != storage);
}

TEST_CASE("SlimValue to_string from slim_storage_container", "[SlimValue][slim_storage_container]") {
	SlimValue value(slim_storage_container {'T', 'E', 'S', 'T'});
	REQUIRE(value.to_string() == "TEST");
}

TEST_CASE("SlimValue bool conversion from slim_storage_container", "[SlimValue][slim_storage_container]") {
	SECTION("non-empty container") {
		SlimValue value(slim_storage_container {123});
		REQUIRE(static_cast<bool>(value));
	}

	SECTION("empty container") {
		SlimValue value(slim_storage_container {});
		REQUIRE_FALSE(static_cast<bool>(value));
	}
}