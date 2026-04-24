#include <catch2/catch_all.hpp>
#include "slim/SlimValue.hpp"

// ============================================================
//  Helpers
// ============================================================

using namespace slim;

// ============================================================
//  AnyValue — construction & has_value
// ============================================================

TEST_CASE("AnyValue default construction holds monostate", "[AnyValue][construction]") {
    AnyValue v;
    REQUIRE_FALSE(v.has_value());
    REQUIRE_FALSE(static_cast<bool>(v));
}

TEST_CASE("AnyValue construction from each supported type", "[AnyValue][construction]") {
    REQUIRE(AnyValue(true).has_value());
    REQUIRE(AnyValue(false).has_value());
    REQUIRE(AnyValue(42).has_value());
    REQUIRE(AnyValue(42L).has_value());
    REQUIRE(AnyValue(42LL).has_value());
    REQUIRE(AnyValue(42U).has_value());
    REQUIRE(AnyValue(42UL).has_value());
    REQUIRE(AnyValue(42ULL).has_value());
    REQUIRE(AnyValue(3.14f).has_value());
    REQUIRE(AnyValue(3.14).has_value());
    REQUIRE(AnyValue(3.14L).has_value());
    REQUIRE(AnyValue('x').has_value());
    REQUIRE(AnyValue(std::string("hello")).has_value());
    REQUIRE(AnyValue("hello").has_value());                   // const char* → string
    REQUIRE(AnyValue(std::string_view("hello")).has_value()); // string_view → string
    REQUIRE(AnyValue(slim_coordinates{1, 2}).has_value());
}

// ============================================================
//  AnyValue — operator bool
// ============================================================

TEST_CASE("AnyValue operator bool", "[AnyValue][bool]") {
    REQUIRE_FALSE(static_cast<bool>(AnyValue()));        // monostate → false
    REQUIRE(static_cast<bool>(AnyValue(true)));
    REQUIRE_FALSE(static_cast<bool>(AnyValue(false)));
    REQUIRE(static_cast<bool>(AnyValue(1)));
    REQUIRE_FALSE(static_cast<bool>(AnyValue(0)));
    REQUIRE(static_cast<bool>(AnyValue(1.0)));
    REQUIRE_FALSE(static_cast<bool>(AnyValue(0.0)));
    REQUIRE(static_cast<bool>(AnyValue(std::string("hi"))));
    REQUIRE_FALSE(static_cast<bool>(AnyValue(std::string(""))));
    REQUIRE(static_cast<bool>(AnyValue(slim_coordinates{0, 0}))); // non-arithmetic → true
}

// ============================================================
//  AnyValue — is<T>
// ============================================================

TEST_CASE("AnyValue is<T> identifies stored type", "[AnyValue][is]") {
    REQUIRE(AnyValue(42).is<int>());
    REQUIRE_FALSE(AnyValue(42).is<long>());
    REQUIRE(AnyValue(42L).is<long>());
    REQUIRE(AnyValue(true).is<bool>());
    REQUIRE(AnyValue(std::string("x")).is<std::string>());
    REQUIRE(AnyValue("x").is<std::string>());
    REQUIRE(AnyValue(slim_coordinates{1,2}).is<slim_coordinates>());
    REQUIRE(AnyValue().is<std::monostate>());
}

// ============================================================
//  AnyValue — exact-type named getters
// ============================================================

TEST_CASE("AnyValue get_bool", "[AnyValue][getters][exact]") {
    REQUIRE(AnyValue(true).get_bool() == true);
    REQUIRE(AnyValue(false).get_bool() == false);
}

TEST_CASE("AnyValue get_int", "[AnyValue][getters][exact]") {
    REQUIRE(AnyValue(99).get_int() == 99);
    REQUIRE(AnyValue(-1).get_int() == -1);
}

TEST_CASE("AnyValue get_long", "[AnyValue][getters][exact]") {
    REQUIRE(AnyValue(123L).get_long() == 123L);
}

TEST_CASE("AnyValue get_long_long", "[AnyValue][getters][exact]") {
    REQUIRE(AnyValue(9000000000LL).get_long_long() == 9000000000LL);
}

TEST_CASE("AnyValue get_unsigned", "[AnyValue][getters][exact]") {
    REQUIRE(AnyValue(7U).get_unsigned() == 7U);
}

TEST_CASE("AnyValue get_unsigned_long", "[AnyValue][getters][exact]") {
    REQUIRE(AnyValue(7UL).get_unsigned_long() == 7UL);
}

TEST_CASE("AnyValue get_unsigned_long_long", "[AnyValue][getters][exact]") {
    REQUIRE(AnyValue(7ULL).get_unsigned_long_long() == 7ULL);
}

TEST_CASE("AnyValue get_float", "[AnyValue][getters][exact]") {
    REQUIRE(AnyValue(1.5f).get_float() == Catch::Approx(1.5f));
}

TEST_CASE("AnyValue get_double", "[AnyValue][getters][exact]") {
    REQUIRE(AnyValue(3.14).get_double() == Catch::Approx(3.14));
}

TEST_CASE("AnyValue get_long_double", "[AnyValue][getters][exact]") {
    REQUIRE(AnyValue(2.71L).get_long_double() == Catch::Approx(2.71L));
}

TEST_CASE("AnyValue get_char", "[AnyValue][getters][exact]") {
    REQUIRE(AnyValue('z').get_char() == 'z');
}

TEST_CASE("AnyValue get_string", "[AnyValue][getters][exact]") {
    REQUIRE(AnyValue(std::string("hello")).get_string() == "hello");
    REQUIRE(AnyValue("world").get_string() == "world");
    REQUIRE(AnyValue(std::string_view("view")).get_string() == "view");
}

TEST_CASE("AnyValue get_coordinates", "[AnyValue][getters][exact]") {
    auto coords = AnyValue(slim_coordinates{3, 7}).get_coordinates();
    REQUIRE(coords.first == 3);
    REQUIRE(coords.second == 7);
}

TEST_CASE("AnyValue exact getter throws on wrong type", "[AnyValue][getters][exact][throws]") {
    REQUIRE_THROWS_AS(AnyValue(42).get_string(),    std::bad_variant_access);
    REQUIRE_THROWS_AS(AnyValue("hi").get_int(),     std::bad_variant_access);
    REQUIRE_THROWS_AS(AnyValue(true).get_int(),     std::bad_variant_access);
    REQUIRE_THROWS_AS(AnyValue().get_bool(),        std::bad_variant_access);
}

// ============================================================
//  AnyValue — cross-type numeric getters
// ============================================================

TEST_CASE("AnyValue get_as_int64 from various integral types", "[AnyValue][getters][cross]") {
    REQUIRE(AnyValue(42).get_as_int64()    == 42);
    REQUIRE(AnyValue(42L).get_as_int64()   == 42);
    REQUIRE(AnyValue(42LL).get_as_int64()  == 42);
    REQUIRE(AnyValue(42U).get_as_int64()   == 42);
    REQUIRE(AnyValue(42UL).get_as_int64()  == 42);
    REQUIRE(AnyValue(42ULL).get_as_int64() == 42);
    REQUIRE(AnyValue(true).get_as_int64()  == 1);
    REQUIRE(AnyValue(false).get_as_int64() == 0);
    REQUIRE(AnyValue('A').get_as_int64()   == 65);
}

TEST_CASE("AnyValue get_as_int64 throws on non-integral", "[AnyValue][getters][cross][throws]") {
    REQUIRE_THROWS_AS(AnyValue(3.14).get_as_int64(),             std::bad_variant_access);
    REQUIRE_THROWS_AS(AnyValue(std::string("x")).get_as_int64(), std::bad_variant_access);
    REQUIRE_THROWS_AS(AnyValue().get_as_int64(),                 std::bad_variant_access);
}

TEST_CASE("AnyValue get_as_uint64 from various integral types", "[AnyValue][getters][cross]") {
    REQUIRE(AnyValue(100).get_as_uint64()    == 100ULL);
    REQUIRE(AnyValue(100ULL).get_as_uint64() == 100ULL);
    REQUIRE(AnyValue(true).get_as_uint64()   == 1ULL);
}

TEST_CASE("AnyValue get_as_double from numeric types", "[AnyValue][getters][cross]") {
    REQUIRE(AnyValue(42).get_as_double()    == Catch::Approx(42.0));
    REQUIRE(AnyValue(3.14f).get_as_double() == Catch::Approx(3.14f));
    REQUIRE(AnyValue(3.14).get_as_double()  == Catch::Approx(3.14));
    REQUIRE(AnyValue(true).get_as_double()  == Catch::Approx(1.0));
}

TEST_CASE("AnyValue get_as_double throws on non-numeric", "[AnyValue][getters][cross][throws]") {
    REQUIRE_THROWS_AS(AnyValue(std::string("x")).get_as_double(), std::bad_variant_access);
    REQUIRE_THROWS_AS(AnyValue().get_as_double(),                 std::bad_variant_access);
}

TEST_CASE("AnyValue get_as_float from floating-point types", "[AnyValue][getters][cross]") {
    REQUIRE(AnyValue(1.5f).get_as_float() == Catch::Approx(1.5L));
    REQUIRE(AnyValue(1.5).get_as_float()  == Catch::Approx(1.5L));
    REQUIRE(AnyValue(1.5L).get_as_float() == Catch::Approx(1.5L));
}

TEST_CASE("AnyValue get_as_float throws on non-float", "[AnyValue][getters][cross][throws]") {
    REQUIRE_THROWS_AS(AnyValue(42).get_as_float(),               std::bad_variant_access);
    REQUIRE_THROWS_AS(AnyValue(std::string("x")).get_as_float(), std::bad_variant_access);
}

// ============================================================
//  AnyValue — optional / non-throwing getters
// ============================================================

TEST_CASE("AnyValue try_bool", "[AnyValue][getters][optional]") {
    REQUIRE(AnyValue(true).try_bool().value()  == true);
    REQUIRE(AnyValue(false).try_bool().value() == false);
    REQUIRE_FALSE(AnyValue(42).try_bool().has_value());
    REQUIRE_FALSE(AnyValue().try_bool().has_value());
}

TEST_CASE("AnyValue try_int64 returns value for all integral types", "[AnyValue][getters][optional]") {
    REQUIRE(AnyValue(42).try_int64().value()   == 42);
    REQUIRE(AnyValue(42L).try_int64().value()  == 42);
    REQUIRE(AnyValue(42LL).try_int64().value() == 42);
    REQUIRE(AnyValue(42U).try_int64().value()  == 42);
    REQUIRE(AnyValue(true).try_int64().value() == 1);
    REQUIRE(AnyValue('A').try_int64().value()  == 65);
}

TEST_CASE("AnyValue try_int64 returns nullopt for non-integral", "[AnyValue][getters][optional]") {
    REQUIRE_FALSE(AnyValue(3.14).try_int64().has_value());
    REQUIRE_FALSE(AnyValue(std::string("x")).try_int64().has_value());
    REQUIRE_FALSE(AnyValue().try_int64().has_value());
}

TEST_CASE("AnyValue try_double returns value for numeric types", "[AnyValue][getters][optional]") {
    REQUIRE(AnyValue(42).try_double().value()   == Catch::Approx(42.0));
    REQUIRE(AnyValue(3.14).try_double().value() == Catch::Approx(3.14));
    REQUIRE(AnyValue(1.5f).try_double().value() == Catch::Approx(1.5f));
    REQUIRE(AnyValue(true).try_double().value() == Catch::Approx(1.0));
}

TEST_CASE("AnyValue try_double returns nullopt for non-numeric", "[AnyValue][getters][optional]") {
    REQUIRE_FALSE(AnyValue(std::string("x")).try_double().has_value());
    REQUIRE_FALSE(AnyValue().try_double().has_value());
    REQUIRE_FALSE(AnyValue(slim_coordinates{1,2}).try_double().has_value());
}

TEST_CASE("AnyValue try_string", "[AnyValue][getters][optional]") {
    REQUIRE(AnyValue(std::string("hello")).try_string().value() == "hello");
    REQUIRE(AnyValue("world").try_string().value()              == "world");
    REQUIRE_FALSE(AnyValue(42).try_string().has_value());
    REQUIRE_FALSE(AnyValue().try_string().has_value());
}

TEST_CASE("AnyValue try_coordinates", "[AnyValue][getters][optional]") {
    auto result = AnyValue(slim_coordinates{4, 8}).try_coordinates();
    REQUIRE(result.has_value());
    REQUIRE(result->first  == 4);
    REQUIRE(result->second == 8);
    REQUIRE_FALSE(AnyValue(42).try_coordinates().has_value());
    REQUIRE_FALSE(AnyValue().try_coordinates().has_value());
}

// ============================================================
//  AnyValue — value_or helpers
// ============================================================

TEST_CASE("AnyValue value_or numeric fallback", "[AnyValue][value_or]") {
    REQUIRE(AnyValue(42).value_or(0)     == 42);
    REQUIRE(AnyValue(42L).value_or(0L)   == 42L);
    REQUIRE(AnyValue().value_or(99)      == 99);  // monostate → fallback
    REQUIRE(AnyValue(3.14).value_or(0.0) == Catch::Approx(3.14));
}

TEST_CASE("AnyValue value_or string fallback", "[AnyValue][value_or]") {
    REQUIRE(AnyValue(std::string("hi")).value_or(std::string("default")) == "hi");
    REQUIRE(AnyValue(42).value_or(std::string("default"))                == "default");
    REQUIRE(AnyValue().value_or(std::string("default"))                  == "default");
}

TEST_CASE("AnyValue value_or const char* fallback", "[AnyValue][value_or]") {
    REQUIRE(AnyValue(std::string("hi")).value_or("default") == "hi");
    REQUIRE(AnyValue(42).value_or("default")                == "default");
    REQUIRE(AnyValue().value_or("default")                  == "default");
}

TEST_CASE("AnyValue value_or bool does not coerce string", "[AnyValue][value_or]") {
    REQUIRE(AnyValue(1).value_or(false) == true);
    REQUIRE(AnyValue(0).value_or(true)  == false);
    REQUIRE(AnyValue(std::string("yes")).value_or(false) == false);
}

// ============================================================
//  AnyValue — to_string
// ============================================================

TEST_CASE("AnyValue to_string", "[AnyValue][to_string]") {
    REQUIRE(AnyValue().to_string()                     == "");
    REQUIRE(AnyValue(true).to_string()                 == "true");
    REQUIRE(AnyValue(false).to_string()                == "false");
    REQUIRE(AnyValue(42).to_string()                   == "42");
    REQUIRE(AnyValue(-7L).to_string()                  == "-7");
    REQUIRE(AnyValue(std::string("hello")).to_string() == "hello");
}

// ============================================================
//  AnyValue — equality operators
// ============================================================

TEST_CASE("AnyValue operator== same type", "[AnyValue][equality]") {
    REQUIRE(AnyValue(42) == AnyValue(42));
    REQUIRE_FALSE(AnyValue(42) == AnyValue(43));
    REQUIRE(AnyValue(std::string("hi")) == AnyValue(std::string("hi")));
}

TEST_CASE("AnyValue operator== cross-type integral", "[AnyValue][equality]") {
    REQUIRE(AnyValue(42) == 42L);
    REQUIRE(AnyValue(42L) == 42);
    REQUIRE(AnyValue(1) == true);
    REQUIRE(AnyValue(0) == false);
}

TEST_CASE("AnyValue operator== string vs string_view and const char*", "[AnyValue][equality]") {
    REQUIRE(AnyValue(std::string("hello")) == "hello");
    REQUIRE(AnyValue(std::string("hello")) == std::string_view("hello"));
}

TEST_CASE("AnyValue operator!= works correctly", "[AnyValue][equality]") {
    REQUIRE(AnyValue(1) != AnyValue(2));
    REQUIRE(AnyValue(std::string("a")) != AnyValue(std::string("b")));
    REQUIRE_FALSE(AnyValue(42) != 42);
}

// ============================================================
//  AnyValue — generic get<T> (mutable and const)
// ============================================================

TEST_CASE("AnyValue generic get<T> mutable allows modification", "[AnyValue][get]") {
    AnyValue v(std::string("hello"));
    v.get<std::string>() += " world";
    REQUIRE(v.get_string() == "hello world");
}

TEST_CASE("AnyValue generic get<T> const is accessible", "[AnyValue][get]") {
    const AnyValue v(42);
    REQUIRE(v.get<int>() == 42);
}

// ============================================================
//  ErrorInfo
// ============================================================

TEST_CASE("ErrorInfo default has no error", "[ErrorInfo]") {
    ErrorInfo e;
    REQUIRE_FALSE(e.has_error());
    REQUIRE_FALSE(e.has_error_code());
    REQUIRE_FALSE(e.has_error_message());
    REQUIRE(e.code()    == -1);
    REQUIRE(e.message() == "");
}

TEST_CASE("ErrorInfo from code only", "[ErrorInfo]") {
    ErrorInfo e(404);
    REQUIRE(e.has_error());
    REQUIRE(e.has_error_code());
    REQUIRE_FALSE(e.has_error_message());
    REQUIRE(e.code() == 404);
}

TEST_CASE("ErrorInfo from message only", "[ErrorInfo]") {
    ErrorInfo e("not found");
    REQUIRE(e.has_error());
    REQUIRE_FALSE(e.has_error_code());
    REQUIRE(e.has_error_message());
    REQUIRE(e.message() == "not found");
}

TEST_CASE("ErrorInfo from code and message", "[ErrorInfo]") {
    ErrorInfo e(500, "internal error");
    REQUIRE(e.has_error());
    REQUIRE(e.code()    == 500);
    REQUIRE(e.message() == "internal error");
}

TEST_CASE("ErrorInfo message_or returns fallback when no message", "[ErrorInfo]") {
    ErrorInfo e(404);
    REQUIRE(e.message_or("unknown") == "unknown");
}

TEST_CASE("ErrorInfo message_or returns message when present", "[ErrorInfo]") {
    ErrorInfo e(404, "not found");
    REQUIRE(e.message_or("unknown") == "not found");
}

// ============================================================
//  SlimValue — construction
// ============================================================

TEST_CASE("SlimValue default construction", "[SlimValue][construction]") {
    SlimValue sv;
    REQUIRE_FALSE(sv.has_value());
    REQUIRE_FALSE(sv.has_error());
    REQUIRE_FALSE(static_cast<bool>(sv));
}

TEST_CASE("SlimValue construction from primitives", "[SlimValue][construction]") {
    REQUIRE(SlimValue(42).has_value());
    REQUIRE(SlimValue(3.14).has_value());
    REQUIRE(SlimValue(true).has_value());
    REQUIRE(SlimValue(std::string("hi")).has_value());
    REQUIRE(SlimValue("hi").has_value());
    REQUIRE(SlimValue(slim_coordinates{1,2}).has_value());
}

TEST_CASE("SlimValue construction from ErrorInfo", "[SlimValue][construction]") {
    SlimValue sv(ErrorInfo(500, "oops"));
    REQUIRE_FALSE(sv.has_value());
    REQUIRE(sv.has_error());
    REQUIRE(sv.get_error().code()    == 500);
    REQUIRE(sv.get_error().message() == "oops");
}

TEST_CASE("SlimValue copy construction", "[SlimValue][construction]") {
    SlimValue a(42);
    SlimValue b = a;
    REQUIRE(b.get_int() == 42);
}

TEST_CASE("SlimValue move construction", "[SlimValue][construction]") {
    SlimValue a(std::string("hello"));
    SlimValue b = std::move(a);
    REQUIRE(b.get_string() == "hello");
}

// ============================================================
//  SlimValue — assignment
// ============================================================

TEST_CASE("SlimValue copy assignment", "[SlimValue][assignment]") {
    SlimValue a(99);
    SlimValue b;
    b = a;
    REQUIRE(b.get_int() == 99);
}

TEST_CASE("SlimValue move assignment", "[SlimValue][assignment]") {
    SlimValue a(std::string("moved"));
    SlimValue b;
    b = std::move(a);
    REQUIRE(b.get_string() == "moved");
}

// ============================================================
//  SlimValue — set_value / set_error
// ============================================================

TEST_CASE("SlimValue set_value replaces stored value", "[SlimValue][setters]") {
    SlimValue sv(42);
    sv.set_value(AnyValue(99));
    REQUIRE(sv.get_int() == 99);
}

TEST_CASE("SlimValue set_error attaches an error", "[SlimValue][setters]") {
    SlimValue sv(42);
    sv.set_error(ErrorInfo(1, "bad"));
    REQUIRE(sv.has_error());
    REQUIRE(sv.get_error().code() == 1);
}

// ============================================================
//  SlimValue — is<T>
// ============================================================

TEST_CASE("SlimValue is<T> delegates to AnyValue", "[SlimValue][is]") {
    REQUIRE(SlimValue(42).is<int>());
    REQUIRE_FALSE(SlimValue(42).is<long>());
    REQUIRE(SlimValue(std::string("x")).is<std::string>());
}

// ============================================================
//  SlimValue — exact-type named getters
// ============================================================

TEST_CASE("SlimValue get_bool", "[SlimValue][getters][exact]") {
    REQUIRE(SlimValue(true).get_bool()  == true);
    REQUIRE(SlimValue(false).get_bool() == false);
}

TEST_CASE("SlimValue get_int", "[SlimValue][getters][exact]") {
    REQUIRE(SlimValue(7).get_int() == 7);
}

TEST_CASE("SlimValue get_long", "[SlimValue][getters][exact]") {
    REQUIRE(SlimValue(7L).get_long() == 7L);
}

TEST_CASE("SlimValue get_long_long", "[SlimValue][getters][exact]") {
    REQUIRE(SlimValue(7LL).get_long_long() == 7LL);
}

TEST_CASE("SlimValue get_unsigned", "[SlimValue][getters][exact]") {
    REQUIRE(SlimValue(7U).get_unsigned() == 7U);
}

TEST_CASE("SlimValue get_unsigned_long", "[SlimValue][getters][exact]") {
    REQUIRE(SlimValue(7UL).get_unsigned_long() == 7UL);
}

TEST_CASE("SlimValue get_unsigned_long_long", "[SlimValue][getters][exact]") {
    REQUIRE(SlimValue(7ULL).get_unsigned_long_long() == 7ULL);
}

TEST_CASE("SlimValue get_float", "[SlimValue][getters][exact]") {
    REQUIRE(SlimValue(2.5f).get_float() == Catch::Approx(2.5f));
}

TEST_CASE("SlimValue get_double", "[SlimValue][getters][exact]") {
    REQUIRE(SlimValue(2.5).get_double() == Catch::Approx(2.5));
}

TEST_CASE("SlimValue get_long_double", "[SlimValue][getters][exact]") {
    REQUIRE(SlimValue(2.5L).get_long_double() == Catch::Approx(2.5L));
}

TEST_CASE("SlimValue get_char", "[SlimValue][getters][exact]") {
    REQUIRE(SlimValue('q').get_char() == 'q');
}

TEST_CASE("SlimValue get_string", "[SlimValue][getters][exact]") {
    REQUIRE(SlimValue(std::string("test")).get_string() == "test");
    REQUIRE(SlimValue("test").get_string()              == "test");
}

TEST_CASE("SlimValue get_coordinates", "[SlimValue][getters][exact]") {
    auto c = SlimValue(slim_coordinates{5, 10}).get_coordinates();
    REQUIRE(c.first  == 5);
    REQUIRE(c.second == 10);
}

// ============================================================
//  SlimValue — const get<T> bug fix verification
// ============================================================

TEST_CASE("SlimValue const get<T> works correctly (bug fix)", "[SlimValue][getters][bugfix]") {
    const SlimValue sv(42);
    REQUIRE(sv.get<int>() == 42);

    const SlimValue sv2(std::string("hello"));
    REQUIRE(sv2.get<std::string>() == "hello");

    const SlimValue sv3(true);
    REQUIRE(sv3.get<bool>() == true);
}

// ============================================================
//  SlimValue — cross-type numeric getters
// ============================================================

TEST_CASE("SlimValue get_as_int64 across integral types", "[SlimValue][getters][cross]") {
    REQUIRE(SlimValue(42).get_as_int64()    == 42);
    REQUIRE(SlimValue(42L).get_as_int64()   == 42);
    REQUIRE(SlimValue(42LL).get_as_int64()  == 42);
    REQUIRE(SlimValue(42U).get_as_int64()   == 42);
    REQUIRE(SlimValue(true).get_as_int64()  == 1);
    REQUIRE(SlimValue(false).get_as_int64() == 0);
}

TEST_CASE("SlimValue get_as_uint64", "[SlimValue][getters][cross]") {
    REQUIRE(SlimValue(42).get_as_uint64()    == 42ULL);
    REQUIRE(SlimValue(42ULL).get_as_uint64() == 42ULL);
}

TEST_CASE("SlimValue get_as_double", "[SlimValue][getters][cross]") {
    REQUIRE(SlimValue(42).get_as_double()    == Catch::Approx(42.0));
    REQUIRE(SlimValue(3.14f).get_as_double() == Catch::Approx(3.14f));
    REQUIRE(SlimValue(3.14).get_as_double()  == Catch::Approx(3.14));
}

TEST_CASE("SlimValue get_as_float (long double)", "[SlimValue][getters][cross]") {
    REQUIRE(SlimValue(1.5f).get_as_float()  == Catch::Approx(1.5L));
    REQUIRE(SlimValue(1.5).get_as_float()   == Catch::Approx(1.5L));
    REQUIRE(SlimValue(1.5L).get_as_float()  == Catch::Approx(1.5L));
}

// ============================================================
//  SlimValue — optional / non-throwing getters
// ============================================================

TEST_CASE("SlimValue try_bool", "[SlimValue][getters][optional]") {
    REQUIRE(SlimValue(true).try_bool().value()  == true);
    REQUIRE_FALSE(SlimValue(42).try_bool().has_value());
    REQUIRE_FALSE(SlimValue().try_bool().has_value());
}

TEST_CASE("SlimValue try_int64", "[SlimValue][getters][optional]") {
    REQUIRE(SlimValue(42).try_int64().value()  == 42);
    REQUIRE(SlimValue(42L).try_int64().value() == 42);
    REQUIRE_FALSE(SlimValue(3.14).try_int64().has_value());
    REQUIRE_FALSE(SlimValue().try_int64().has_value());
}

TEST_CASE("SlimValue try_double", "[SlimValue][getters][optional]") {
    REQUIRE(SlimValue(42).try_double().value()   == Catch::Approx(42.0));
    REQUIRE(SlimValue(3.14).try_double().value() == Catch::Approx(3.14));
    REQUIRE_FALSE(SlimValue(std::string("x")).try_double().has_value());
}

TEST_CASE("SlimValue try_string", "[SlimValue][getters][optional]") {
    REQUIRE(SlimValue(std::string("hi")).try_string().value() == "hi");
    REQUIRE_FALSE(SlimValue(42).try_string().has_value());
    REQUIRE_FALSE(SlimValue().try_string().has_value());
}

TEST_CASE("SlimValue try_coordinates", "[SlimValue][getters][optional]") {
    auto result = SlimValue(slim_coordinates{2, 4}).try_coordinates();
    REQUIRE(result.has_value());
    REQUIRE(result->first  == 2);
    REQUIRE(result->second == 4);
    REQUIRE_FALSE(SlimValue(42).try_coordinates().has_value());
}

// ============================================================
//  SlimValue — value_or helpers
// ============================================================

TEST_CASE("SlimValue value_or numeric", "[SlimValue][value_or]") {
    REQUIRE(SlimValue(42).value_or(0)     == 42);
    REQUIRE(SlimValue().value_or(99)      == 99);
    REQUIRE(SlimValue(3.14).value_or(0.0) == Catch::Approx(3.14));
}

TEST_CASE("SlimValue value_or string", "[SlimValue][value_or]") {
    REQUIRE(SlimValue(std::string("hi")).value_or(std::string("x")) == "hi");
    REQUIRE(SlimValue(42).value_or(std::string("fallback"))         == "fallback");
    REQUIRE(SlimValue().value_or(std::string("fallback"))           == "fallback");
}

TEST_CASE("SlimValue value_or const char*", "[SlimValue][value_or]") {
    REQUIRE(SlimValue(std::string("hi")).value_or("x") == "hi");
    REQUIRE(SlimValue(42).value_or("fallback")         == "fallback");
}

// ============================================================
//  SlimValue — to_string
// ============================================================

TEST_CASE("SlimValue to_string delegates to AnyValue", "[SlimValue][to_string]") {
    REQUIRE(SlimValue(42).to_string()                  == "42");
    REQUIRE(SlimValue(true).to_string()                == "true");
    REQUIRE(SlimValue(std::string("hi")).to_string()   == "hi");
    REQUIRE(SlimValue().to_string()                    == "");
}

// ============================================================
//  SlimValue — operator bool / operator!
// ============================================================

TEST_CASE("SlimValue operator bool", "[SlimValue][bool]") {
    REQUIRE(static_cast<bool>(SlimValue(1)));
    REQUIRE_FALSE(static_cast<bool>(SlimValue(0)));
    REQUIRE_FALSE(static_cast<bool>(SlimValue()));
    REQUIRE(static_cast<bool>(SlimValue(std::string("hi"))));
    REQUIRE_FALSE(static_cast<bool>(SlimValue(std::string(""))));
}

TEST_CASE("SlimValue operator!", "[SlimValue][bool]") {
    REQUIRE(!SlimValue());
    REQUIRE(!SlimValue(0));
    REQUIRE_FALSE(!SlimValue(1));
}

// ============================================================
//  SlimValue — equality operators
// ============================================================

TEST_CASE("SlimValue operator== and != with SlimValue", "[SlimValue][equality]") {
    REQUIRE(SlimValue(42) == SlimValue(42));
    REQUIRE_FALSE(SlimValue(42) == SlimValue(43));
    REQUIRE(SlimValue(42) != SlimValue(43));
}

TEST_CASE("SlimValue operator== with raw types", "[SlimValue][equality]") {
    REQUIRE(SlimValue(42) == 42);
    REQUIRE(SlimValue(42) == 42L);
    REQUIRE(SlimValue(std::string("hi")) == "hi");
    REQUIRE_FALSE(SlimValue(42) == 99);
}

// ============================================================
//  SlimMap
// ============================================================

TEST_CASE("SlimMap basic set and get", "[SlimMap]") {
    SlimMap m;
    m.set("key", AnyValue(42));
    REQUIRE(m.has("key"));
    REQUIRE(m.get("key").get_int() == 42);
}

TEST_CASE("SlimMap create returns writable reference", "[SlimMap]") {
    SlimMap m;
    m.create("x") = AnyValue(std::string("hello"));
    REQUIRE(m.get("x").get_string() == "hello");
}

TEST_CASE("SlimMap const get", "[SlimMap]") {
    SlimMap m;
    m.set("n", AnyValue(7));
    const SlimMap& cm = m;
    REQUIRE(cm.get("n").get_int() == 7);
}

TEST_CASE("SlimMap has returns false for missing key", "[SlimMap]") {
    SlimMap m;
    REQUIRE_FALSE(m.has("missing"));
}

TEST_CASE("SlimMap remove", "[SlimMap]") {
    SlimMap m;
    m.set("k", AnyValue(1));
    m.remove("k");
    REQUIRE_FALSE(m.has("k"));
}

TEST_CASE("SlimMap size and clear", "[SlimMap]") {
    SlimMap m;
    m.set("a", AnyValue(1));
    m.set("b", AnyValue(2));
    REQUIRE(m.size() == 2);
    m.clear();
    REQUIRE(m.size() == 0);
}

TEST_CASE("SlimMap get throws on missing key", "[SlimMap]") {
    SlimMap m;
    REQUIRE_THROWS_AS(m.get("nope"), std::out_of_range);
}

// ============================================================
//  SlimMultiMap
// ============================================================

TEST_CASE("SlimMultiMap set and has", "[SlimMultiMap]") {
    SlimMultiMap mm;
    mm.set("tag", AnyValue(std::string("a")));
    REQUIRE(mm.has("tag"));
}

TEST_CASE("SlimMultiMap allows multiple values per key", "[SlimMultiMap]") {
    SlimMultiMap mm;
    mm.set("x", AnyValue(1));
    mm.set("x", AnyValue(2));
    mm.set("x", AnyValue(3));
    REQUIRE(mm.size() == 3);
    auto [begin, end] = mm.get("x");
    int count = 0;
    for (auto it = begin; it != end; ++it) ++count;
    REQUIRE(count == 3);
}

TEST_CASE("SlimMultiMap create returns reference", "[SlimMultiMap]") {
    SlimMultiMap mm;
    mm.create("y") = AnyValue(99);
    REQUIRE(mm.has("y"));
}

TEST_CASE("SlimMultiMap remove erases all values for key", "[SlimMultiMap]") {
    SlimMultiMap mm;
    mm.set("k", AnyValue(1));
    mm.set("k", AnyValue(2));
    mm.remove("k");
    REQUIRE_FALSE(mm.has("k"));
}

TEST_CASE("SlimMultiMap size and clear", "[SlimMultiMap]") {
    SlimMultiMap mm;
    mm.set("a", AnyValue(1));
    mm.set("a", AnyValue(2));
    REQUIRE(mm.size() == 2);
    mm.clear();
    REQUIRE(mm.size() == 0);
}

// ============================================================
//  SlimValue — map accessors
// ============================================================

TEST_CASE("SlimValue get_map creates on demand", "[SlimValue][maps]") {
    SlimValue sv;
    REQUIRE_FALSE(sv.has_maps());
    auto& m = sv.get_map("props");
    REQUIRE(sv.has_maps());
    REQUIRE(sv.has_map("props"));
    (void)m;
}

TEST_CASE("SlimValue get_map stores and retrieves values", "[SlimValue][maps]") {
    SlimValue sv;
    sv.get_map("data").set("count", AnyValue(5));
    REQUIRE(sv.get_map("data").get("count").get_int() == 5);
}

TEST_CASE("SlimValue has_map returns false for absent map", "[SlimValue][maps]") {
    SlimValue sv;
    REQUIRE_FALSE(sv.has_map("nope"));
}

TEST_CASE("SlimValue get_multi_map creates on demand", "[SlimValue][maps]") {
    SlimValue sv;
    REQUIRE_FALSE(sv.has_multi_maps());
    auto& mm = sv.get_multi_map("tags");
    REQUIRE(sv.has_multi_maps());
    REQUIRE(sv.has_multi_map("tags"));
    (void)mm;
}

TEST_CASE("SlimValue get_multi_map stores multiple values", "[SlimValue][maps]") {
    SlimValue sv;
    sv.get_multi_map("tags").set("color", AnyValue(std::string("red")));
    sv.get_multi_map("tags").set("color", AnyValue(std::string("blue")));
    REQUIRE(sv.get_multi_map("tags").size() == 2);
}