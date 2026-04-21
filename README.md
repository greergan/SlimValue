# SlimValue.hpp

A modern C++ type system providing flexible, type-safe value containers with integrated error handling and key-value storage.

---

## Features

- **Type-Safe Value Container** — Hold multiple data types in a single, unified interface
- **Integrated Error Handling** — Combine values with error codes and messages
- **Key-Value Storage** — Single and multi-value map structures built-in
- **Zero-Cost Abstractions** — Leverages modern C++ for minimal runtime overhead
- **Automatic Type Conversion** — Smart casting for compatible types
- **Lazy Initialization** — Maps created on-demand

---

## Components

### AnyValue

A variant-based container supporting multiple types.

**Supported Types:**
- Primitives: `bool`, `int`, `long`, `long long`, `unsigned`, `unsigned long`, `unsigned long long`
- Floating-point: `float`, `double`, `long double`
- Character: `char`
- String: `std::string`
- Coordinates: `std::pair<int, int>`
- Empty: `std::monostate`

**Usage:**
```cpp
slim::AnyValue val1(42);
slim::AnyValue val2("hello");
slim::AnyValue val3(3.14);

// Type checking
if (val1.is<int>()) {
    std::cout << val1.get<int>() << std::endl;
}

// String conversion
std::string str = val1.to_string();  // "42"

// Truthiness evaluation
if (val1) {
    // Non-zero, non-empty
}
