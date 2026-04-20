#ifndef __SLIM__HPP
#define __SLIM__HPP
#include <any>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <variant>

namespace slim {
	using slim_coordinates = std::pair<int, int>;
	struct AnyValue {
		private:
			using Variant = std::variant<
				std::monostate,
				bool,
				int,
				long,
				long long,
				unsigned,
				unsigned long,
				unsigned long long,
				float,
				double,
				long double,
				char,
				std::string,
				slim_coordinates
			>;
			Variant __value;

		public:
			AnyValue() = default;

            template <typename _T>
            requires std::constructible_from<Variant, std::decay_t<_T>>AnyValue(_T&& _v) {
                using U = std::decay_t<_T>;

	            if constexpr (std::is_same_v<U, std::string_view>) {
		            __value = std::string(_v);
	            }
	            else {
		            __value = std::forward<_T>(_v);
	            }
            }

			explicit operator bool() const {
				return std::visit([](auto&& _v)-> bool {
					using _T = std::decay_t<decltype(_v)>;

					if constexpr (std::is_same_v<_T, std::monostate>) {
						return false;
					}
					else if constexpr (std::is_same_v<_T, bool>) {
						return _v;
					}
					else if constexpr (std::is_arithmetic_v<_T>) {
						return _v != 0;
					}
					else if constexpr (std::is_same_v<_T, std::string>) {
						return !_v.empty();
					} 
					else {
						return true;
					}
				}, __value);
			}

			bool operator==(const AnyValue& _other) const {
				return __value == _other.__value;
			}

			bool operator!=(const AnyValue& _other) const {
				return __value != _other.__value;
			}

			template <typename _T>
			bool operator==(const _T& _rhs) const {
				using U = std::decay_t<_T>;

				return std::visit([&](auto&& _v) -> bool {
					using _V = std::decay_t<decltype(_v)>;

					if constexpr (std::is_same_v<_V, std::string>) {
						if constexpr (std::is_same_v<U, std::string>) {
							return _v == _rhs;
						}
						else {
							return _v == std::string_view(_rhs);
						}
					}
					else if constexpr (std::is_integral_v<_V> && std::is_integral_v<U>) {
						return static_cast<int64_t>(_v) == static_cast<int64_t>(_rhs);
					}
					else if constexpr (std::is_floating_point_v<_V> && std::is_floating_point_v<U>) {
						return static_cast<double>(_v) == static_cast<double>(_rhs);
					}
					else if constexpr ((std::is_arithmetic_v<_V> && std::is_arithmetic_v<U>)) {
						return static_cast<double>(_v) == static_cast<double>(_rhs);
					}
					else if constexpr (std::is_same_v<_V, U>) {
						return _v == _rhs;
					}
					else {
						return false;
					}
				}, __value);
			}

			template <typename _T>
			bool operator!=(const _T& _rhs) const {
				using U = std::decay_t<_T>;

				return std::visit([&](auto&& _v) -> bool {
					using _V = std::decay_t<decltype(_v)>;

					if constexpr (std::is_same_v<_V, std::string>) {
						if constexpr (std::is_same_v<U, std::string>) {
							return _v != _rhs;
						}
						else {
							return _v != std::string_view(_rhs);
						}
					}
					else if constexpr (std::is_integral_v<_V> && std::is_integral_v<U>) {
						return static_cast<int64_t>(_v) != static_cast<int64_t>(_rhs);
					}
					else if constexpr (std::is_floating_point_v<_V> && std::is_floating_point_v<U>) {
						return static_cast<double>(_v) != static_cast<double>(_rhs);
					}
					else if constexpr ((std::is_arithmetic_v<_V> && std::is_arithmetic_v<U>)) {
						return static_cast<double>(_v) != static_cast<double>(_rhs);
					}
					else if constexpr (std::is_same_v<_V, U>) {
						return _v != _rhs;
					}
					else {
						return false;
					}
				}, __value);
			}

			template <typename _T>
			bool is() const {
				return std::holds_alternative<_T>(__value);
			}

			bool has_value() const {
				return !std::holds_alternative<std::monostate>(__value);
			}

			template <typename _T>
			_T& get() {
				return std::get<_T>(__value);
			}

			template <typename _T>
			const _T& get() const {
				return std::get<_T>(__value);
			}

			bool get_bool() const {
				return get<bool>();
			}

			std::string to_string() const {
				return std::visit([](auto&& _v) -> std::string {
					using _T = std::decay_t<decltype(_v)>;

					if constexpr (std::is_same_v<_T, std::monostate>) {
						return "";
					}
					else if constexpr (std::is_same_v<_T, bool>) {
						return _v ? "true" : "false";
					}
					else if constexpr (std::is_integral_v<_T>) {
						return std::to_string(_v);
					}
					else if constexpr (std::is_floating_point_v<_T>) {
						return std::to_string(_v);
					}
					else if constexpr (std::is_same_v<_T, std::string>) {
						return _v;
					}
					else {
						return "";
					}
				}, __value);
			}
	};

	struct ErrorInfo {
		private:
			std::optional<int> __code;
			std::optional<std::string> __message;

		public:
			ErrorInfo() = default;
			ErrorInfo(int _code) : __code(_code) {}
			ErrorInfo(std::string_view _message) : __message(_message) {}
			ErrorInfo(int _code, std::string_view _message) : __code(_code), __message(_message) {}

			template <typename _T>
			ErrorInfo(_T&& _message): __message(std::forward<_T>(_message)) {}

			template <typename _T>
			ErrorInfo(int _code, _T&& _message): __code(_code), __message(std::forward<_T>(_message)) {}

			bool has_error() const {
				return __code.has_value() || __message.has_value();
			}

			bool has_error_code() const {
				return __code.has_value();
			}

			bool has_error_message() const {
				return __message.has_value();
			}

			int code() const {
				return __code.value_or(-1);
			}

			std::string message() const {
				return __message.value_or("");
			}

			std::string message_or(std::string _or_message = "") const {
				return __message.value_or(_or_message);
			}
	};

	struct SlimMap {
		private:
			std::unordered_map<std::string, AnyValue> __map;

		public:
			SlimMap() = default;

			AnyValue& create(const std::string& _key) {
				return __map[_key];
			}

			std::unordered_map<std::string, AnyValue>& get() {
				return __map;
			}

			AnyValue& get(const std::string& _key) {
				return __map.at(_key);
			}

			const AnyValue& get(const std::string& _key) const {
				return __map.at(_key);
			}

			bool has(const std::string& _key) const {
				return __map.contains(_key);
			}

			void set(const std::string& _key, const AnyValue& _value) {
				__map[_key] = _value;
			}

			void set(const std::string& _key, AnyValue&& _value) {
				__map[_key] = std::move(_value);
			}

			void remove(const std::string& _key) {
				__map.erase(_key);
			}

			std::size_t size() const {
				return __map.size();
			}

			void clear() {
				__map.clear();
			}
	};

	struct SlimMultiMap {
		private:
			std::unordered_multimap<std::string, AnyValue> __map;

		public:
			SlimMultiMap() = default;

			AnyValue& create(const std::string& _key) {
				return __map.emplace(_key, AnyValue{})->second;
			}

			std::unordered_multimap<std::string, AnyValue>& get() {
				return __map;
			}

			std::pair<
				std::unordered_multimap<std::string, AnyValue>::iterator,
				std::unordered_multimap<std::string, AnyValue>::iterator
			> get(const std::string& _key) {
				return __map.equal_range(_key);
			}

			std::pair<
				std::unordered_multimap<std::string, AnyValue>::const_iterator,
				std::unordered_multimap<std::string, AnyValue>::const_iterator
			> get(const std::string& _key) const {
				return __map.equal_range(_key);
			}

			bool has(const std::string& _key) const {
				return __map.find(_key) != __map.end();
			}

			void set(const std::string& _key, AnyValue&& _value) {
				__map.emplace(_key, std::move(_value));
			}

			template <typename _T>
			void set(const std::string& _key, _T&& _value) {
				__map.emplace(_key, AnyValue(std::forward<_T>(_value)));
			}

			void remove(const std::string& _key) {
				__map.erase(_key);
			}

			std::size_t size() const {
				return __map.size();
			}

			void clear() {
				__map.clear();
			}
	};

	struct SlimValue {
		private:
			AnyValue __value;
			ErrorInfo __error;

			std::optional<std::unordered_map<std::string, SlimMap>> __maps;
			std::optional<std::unordered_map<std::string, SlimMultiMap>> __multi_maps;

		public:
			SlimValue() = default;

			template <typename _T>requires (!std::same_as<std::decay_t<_T>, AnyValue>
				&& !std::same_as<std::decay_t<_T>, ErrorInfo> && std::constructible_from<AnyValue, _T>)
			SlimValue(_T&& _value) : __value(std::forward<_T>(_value)), __error() {}

			template <typename _T, std::enable_if_t<std::is_same_v<std::decay_t<_T>, AnyValue>, int> = 0>
			SlimValue(_T&& _value) : __value(std::forward<_T>(_value)), __error() {}

			template <typename _T, std::enable_if_t<std::is_same_v<std::decay_t<_T>, ErrorInfo>, int> = 0>
			SlimValue(_T&& _error) : __value(), __error(std::forward<_T>(_error)) {}

			SlimValue(AnyValue _value, ErrorInfo _error) : __value(std::move(_value)), __error(std::move(_error)) {}

			explicit operator bool() const {
				return static_cast<bool>(__value);
			}

			bool operator!() const {
				return !static_cast<bool>(__value);
			}

			bool operator==(const SlimValue& _other) const {
				return __value == _other.__value;
			}

			bool operator!=(const SlimValue& _other) const {
				return __value != _other.__value;
			}

			template <typename _T>
			bool operator==(const _T& _rhs) const {
				return __value == _rhs;
			}

			template <typename _T>
			bool operator!=(const _T& _rhs) const {
				return __value != _rhs;
			}

			SlimMap& get_map(const std::string& _key) {
				if(!__maps) {
					__maps.emplace();
				}
				return (*__maps)[_key];
			}

			SlimMultiMap& get_multi_map(const std::string& _key) {
				if(!__multi_maps) {
					__multi_maps.emplace();
				}
				return (*__multi_maps)[_key];
			}

			bool has_maps() const {
				return __maps.has_value();
			}

			bool has_map(const std::string& _key) const {
				return __maps && __maps->contains(_key);
			}

			bool has_multi_maps() const {
				return __multi_maps.has_value();
			}

			bool has_multi_map(const std::string& _key) const {
				return __multi_maps && __multi_maps->contains(_key);
			}

			void set_error(const ErrorInfo& _error) {
				__error = _error;
			}

			void set_error(ErrorInfo&& _error) {
				__error = std::move(_error);
			}

			void set_value(const AnyValue& _value) {
				__value = _value;
			}

			void set_value(AnyValue&& _value) {
				__value = std::move(_value);
			}

			bool has_error() const {
				return __error.has_error();
			}

			const ErrorInfo& get_error() const {
				return __error;
			}

			bool has_value() const {
				return __value.has_value();
			}

			template <typename _T>
			bool is() const {
				return __value.is<_T>();
			}

			template <typename _T>
			_T& get() {
				return __value.get<_T>();
			}

			template <typename _T>
			const _T& get() const {
				return std::any_cast<const _T&>(__value);
			}

			std::string to_string() const {
				return __value.to_string();
			}
	};
}
#endif
