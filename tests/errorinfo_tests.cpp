#include <slim/SlimValue.hpp>
#include <catch2/catch_test_macros.hpp>
#include <string>
#include <string_view>

TEST_CASE("ErrorInfo default state") {
	slim::ErrorInfo e;

	REQUIRE_FALSE(e.has_error());
	REQUIRE_FALSE(e.has_error_code());
	REQUIRE_FALSE(e.has_error_message());
	REQUIRE(e.code() == -1);
	REQUIRE(e.message().empty());
}

TEST_CASE("ErrorInfo code only") {
	slim::ErrorInfo e(404);

	REQUIRE(e.has_error());
	REQUIRE(e.has_error_code());
	REQUIRE_FALSE(e.has_error_message());
	REQUIRE(e.code() == 404);
	REQUIRE(e.message().empty());
}

TEST_CASE("ErrorInfo message only (string_view)") {
	slim::ErrorInfo e(std::string_view("failure"));

	REQUIRE(e.has_error());
	REQUIRE_FALSE(e.has_error_code());
	REQUIRE(e.has_error_message());
	REQUIRE(e.code() == -1);
	REQUIRE(e.message() == "failure");
}

TEST_CASE("ErrorInfo code and message") {
	slim::ErrorInfo e(500, std::string_view("server error"));

	REQUIRE(e.has_error());
	REQUIRE(e.has_error_code());
	REQUIRE(e.has_error_message());
	REQUIRE(e.code() == 500);
	REQUIRE(e.message() == "server error");
}

TEST_CASE("ErrorInfo message_or fallback") {
	slim::ErrorInfo e;

	REQUIRE(e.message_or("fallback") == "fallback");

	slim::ErrorInfo e2(std::string_view("real"));
	REQUIRE(e2.message_or("fallback") == "real");
}

TEST_CASE("ErrorInfo implicit message template constructor") {
	slim::ErrorInfo e(std::string("dynamic error"));

	REQUIRE(e.has_error_message());
	REQUIRE(e.message() == "dynamic error");
}