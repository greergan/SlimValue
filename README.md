# SlimValue

A single-header C++20 value container library providing type-safe storage, error propagation, and named map collections — with no external dependencies.

## Types

| Type | Purpose |
|------|---------|
| `AnyValue` | Type-safe variant value supporting all primitive types, `std::string`, and coordinates |
| `ErrorInfo` | Optional error code and/or message |
| `SlimMap` | Named `string → AnyValue` map (unique keys) |
| `SlimMultiMap` | Named `string → AnyValue` multimap (duplicate keys allowed) |
| `SlimValue` | Combines `AnyValue` + `ErrorInfo` + optional named maps |

## Supported Value Types

`AnyValue` (and by extension `SlimValue`) can hold any of the following:

`bool`, `int`, `long`, `long long`, `unsigned`, `unsigned long`, `unsigned long long`, `float`, `double`, `long double`, `std::string`, `slim_storage_container`, `slim_coordinates`

`char`, `std::string_view`, and string literals are automatically converted to `std::string` on construction — `char` is not stored as a distinct type. `std::monostate` represents the empty/unset state.

`slim_coordinates` is an alias for `std::pair<int, int>`. `slim_storage_container` is an alias for `std::vector<uint8_t>`.

## Usage

### AnyValue

```cpp
#include <slim/SlimValue.hpp>

slim::AnyValue v = 42;
slim::AnyValue s = "hello";
slim::AnyValue f = 3.14;
slim::AnyValue coords = slim::slim_coordinates{10, 20};

// Type check
v.is<int>();          // true
v.has_value();        // true

// Exact-type getters (throw std::bad_variant_access on mismatch)
int  n = v.get_int();
auto str = s.get_string();

// Cross-type numeric getters
int64_t  i = v.get_as_int64();
double   d = v.get_as_double();

// Non-throwing getters
std::optional<int64_t> maybe = v.try_int64();

// Fallback
int result = v.value_or(0);

// Stringify
std::string text = v.to_string();

// Bool conversion: monostate → false, 0 → false, "" → false, else → true
if (v) { /* has a truthy value */ }
```

### ErrorInfo

```cpp
slim::ErrorInfo ok;                          // no error
slim::ErrorInfo e1{404};                     // code only
slim::ErrorInfo e2{"not found"};             // message only
slim::ErrorInfo e3{404, "not found"};        // both

e3.has_error();         // true
e3.has_error_code();    // true
e3.code();              // 404
e3.message();           // "not found"
e3.message_or("n/a");  // "not found"
```

### SlimValue

`SlimValue` is the primary interface. It wraps an `AnyValue` and an `ErrorInfo` together, and optionally carries named `SlimMap` / `SlimMultiMap` collections.

```cpp
slim::SlimValue val = 42;
slim::SlimValue err = slim::ErrorInfo{500, "internal error"};

val.has_value();   // true
val.has_error();   // false

err.has_error();               // true
err.get_error().code();        // 500
err.get_error().message();     // "internal error"

// All AnyValue getters are forwarded:
val.get_int();
val.get_as_double();
val.try_string();
val.value_or(0);
val.to_string();
```

### SlimMap

Unique-key `string → AnyValue` store attached to a `SlimValue`:

```cpp
slim::SlimValue result;
slim::SlimMap& meta = result.get_map("meta");

meta.set("status", 200);
meta.set("body", "ok");

meta.has("status");          // true
meta.get("status").get_int(); // 200
meta.size();                 // 2
meta.remove("body");
meta.clear();

result.has_map("meta");      // true
result.has_maps();           // true
```

### SlimMultiMap

`SlimMultiMap` allows multiple values under the same key, making it ideal for bubbling metadata up through a chain of function calls — each layer can append its own values without overwriting those set by callers deeper in the stack.

```cpp
// Deep in the call stack, each layer appends its own metadata
slim::SlimValue result;
slim::SlimMultiMap& trace = result.get_multi_map("trace");

// layer A
trace.set("step", "validate");
// layer B
trace.set("step", "transform");
// layer C
trace.set("step", "persist");

// Caller collects all entries for "step"
auto [begin, end] = trace.get("step");
for (auto it = begin; it != end; ++it) {
    std::cout << it->second.get_string() << "\n";
    // validate, transform, persist
}

result.has_multi_map("trace");  // true
```

## API Reference

### `AnyValue`

#### Construction
```cpp
AnyValue();                  // empty (monostate)
AnyValue(T&& value);         // any supported type; string_view → std::string
```

#### Inspection
```cpp
bool has_value() const;
template <typename T> bool is() const;
explicit operator bool() const;
bool operator==(const AnyValue&) const;
bool operator==(const T&) const;
```

#### Exact-type getters
```cpp
bool          get_bool()               const;
int           get_int()                const;
long          get_long()               const;
long long     get_long_long()          const;
unsigned      get_unsigned()           const;
unsigned long get_unsigned_long()      const;
unsigned long long get_unsigned_long_long() const;
float         get_float()              const;
double        get_double()             const;
long double   get_long_double()        const;
const std::string&          get_string()           const;  // char input is stored as string
slim_storage_container&     get_storage_container();
const slim_storage_container& get_storage_container() const;
slim_coordinates            get_coordinates()       const;
template <typename T> T&   get();
```

#### Cross-type numeric getters *(throw on mismatch)*
```cpp
int64_t     get_as_int64()  const;
uint64_t    get_as_uint64() const;
double      get_as_double() const;
long double get_as_float()  const;
```

#### Non-throwing getters
```cpp
std::optional<bool>                    try_bool()             const;
std::optional<int64_t>                 try_int64()            const;
std::optional<double>                  try_double()           const;
std::optional<std::string>             try_string()           const;
std::optional<slim_storage_container>  try_storage_container() const;
std::optional<slim_coordinates>        try_coordinates()      const;
```

#### Fallback & stringify
```cpp
template <typename T> T value_or(T fallback)             const;
std::string value_or(std::string fallback)                const;
std::string value_or(const char* fallback)                const;
std::string to_string()                                   const;
```

---

### `ErrorInfo`

```cpp
ErrorInfo();
ErrorInfo(int code);
ErrorInfo(std::string_view message);
ErrorInfo(int code, std::string_view message);

bool        has_error()         const;
bool        has_error_code()    const;
bool        has_error_message() const;
int         code()              const;   // -1 if unset
std::string message()           const;   // "" if unset
std::string message_or(std::string fallback = "") const;
```

---

### `SlimMap`

```cpp
AnyValue& create(std::string_view key);
AnyValue& get(std::string_view key);
void      set(std::string_view key, const AnyValue& value);
void      set(std::string_view key, AnyValue&& value);
bool      has(std::string_view key) const;
void      remove(std::string_view key);
std::size_t size() const;
void      clear();
std::unordered_map<std::string, AnyValue>& get();
```

---

### `SlimMultiMap`

```cpp
AnyValue& create(std::string_view key);
auto      get(std::string_view key);     // returns equal_range iterator pair
void      set(std::string_view key, AnyValue&& value);
template <typename T> void set(std::string_view key, T&& value);
bool      has(std::string_view key) const;
void      remove(std::string_view key);  // removes all entries for key
std::size_t size() const;
void      clear();
std::unordered_multimap<std::string, AnyValue>& get();
```

---

### `SlimValue`

All `AnyValue` getters are forwarded. Additional members:

```cpp
// Error
void             set_error(int code);
void             set_error(std::string_view message);
void             set_error(int code, std::string_view message);
void             set_error(const ErrorInfo& error);
void             set_error(ErrorInfo&& error);
bool             has_error()           const;
const ErrorInfo& get_error()           const;

// Value
void set_value(AnyValue value);

// Maps
SlimMap&      get_map(std::string_view key);
SlimMultiMap& get_multi_map(std::string_view key);
bool          has_map(std::string_view key)       const;
bool          has_maps()                          const;
bool          has_multi_map(std::string_view key) const;
bool          has_multi_maps()                    const;
```

## Building

`SlimValue` is a single configured header. Point CMake at it and include it where needed:

```cmake
configure_file(include/slim/SlimValue.hpp.in include/slim/SlimValue.hpp)
target_include_directories(your_target PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/include)
```

Requires a C++20-capable compiler (`std::constructible_from`, `std::same_as` concepts).
