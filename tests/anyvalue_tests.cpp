#include <slim/SlimValue.hpp>
#include <catch2/catch_test_macros.hpp>
#include <string>
#include <string_view>


TEST_CASE("AnyValue monostate") {
	slim::AnyValue v;

	REQUIRE_FALSE(v.has_value());
	REQUIRE_FALSE(static_cast<bool>(v));
}

TEST_CASE("AnyValue bool") {
	slim::AnyValue v(true);

	REQUIRE(v.is<bool>());
	REQUIRE(v.get<bool>() == true);
	REQUIRE(static_cast<bool>(v));
}

TEST_CASE("AnyValue int") {
	slim::AnyValue v(42);

	REQUIRE(v.is<int>());
	REQUIRE(v.get<int>() == 42);
}

TEST_CASE("AnyValue long") {
	long x = 123;
	slim::AnyValue v(x);

	REQUIRE(v.is<long>());
	REQUIRE(v.get<long>() == 123);
}

TEST_CASE("AnyValue long long") {
	long long x = 123456789LL;
	slim::AnyValue v(x);

	REQUIRE(v.is<long long>());
	REQUIRE(v.get<long long>() == x);
}

TEST_CASE("AnyValue unsigned") {
	unsigned x = 99;
	slim::AnyValue v(x);

	REQUIRE(v.is<unsigned>());
	REQUIRE(v.get<unsigned>() == x);
}

TEST_CASE("AnyValue unsigned long") {
	unsigned long x = 1000UL;
	slim::AnyValue v(x);

	REQUIRE(v.is<unsigned long>());
	REQUIRE(v.get<unsigned long>() == x);
}

TEST_CASE("AnyValue unsigned long long") {
	unsigned long long x = 10000ULL;
	slim::AnyValue v(x);

	REQUIRE(v.is<unsigned long long>());
	REQUIRE(v.get<unsigned long long>() == x);
}

TEST_CASE("AnyValue float") {
	float x = 3.14f;
	slim::AnyValue v(x);

	REQUIRE(v.is<float>());
	REQUIRE(v.get<float>() == x);
}

TEST_CASE("AnyValue double") {
	double x = 6.28;
	slim::AnyValue v(x);

	REQUIRE(v.is<double>());
	REQUIRE(v.get<double>() == x);
}

TEST_CASE("AnyValue long double") {
	long double x = 9.99L;
	slim::AnyValue v(x);

	REQUIRE(v.is<long double>());
	REQUIRE(v.get<long double>() == x);
}

TEST_CASE("AnyValue char") {
	char x = 'a';
	slim::AnyValue v(x);

	REQUIRE(v.is<char>());
	REQUIRE(v.get<char>() == x);
}

TEST_CASE("AnyValue string") {
	std::string s = "hello";
	slim::AnyValue v(s);

	REQUIRE(v.is<std::string>());
	REQUIRE(v.get<std::string>() == s);
}

TEST_CASE("AnyValue string_view conversion") {
	std::string_view sv = "world";
	slim::AnyValue v(sv);

	REQUIRE(v.is<std::string>());
	REQUIRE(v.get<std::string>() == "world");
}

TEST_CASE("AnyValue coordinates") {
	slim::slim_coordinates c{1, 2};
	slim::AnyValue v(c);

	REQUIRE(v.is<slim::slim_coordinates>());
	REQUIRE(v.get<slim::slim_coordinates>().first == 1);
	REQUIRE(v.get<slim::slim_coordinates>().second == 2);
}