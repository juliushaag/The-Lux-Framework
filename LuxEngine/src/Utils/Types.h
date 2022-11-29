#pragma once


#include <map>
#include <set>
#include <list>
#include <array>
#include <memory>
#include <vector>
#include <bitset>
#include <unordered_map>
#include <optional>
#include <string_view>

#include <glm.hpp>

namespace Lux
{

#define BIT(x) (1 << x)

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T, int count>
using Array = std::array<T, count>;

template<typename T, typename N>
using Container = std::unordered_map<T, N>;

template<typename T, typename N>
using SortedContainer = std::map<T, N>;

template<typename T>
using Optional = std::optional<T>;

template<typename T>
using List = std::vector<T>;

template<int T>
using Bitset = std::bitset<T>;

template<typename T>
using Set = std::set<T>;

template<typename T>
using LinkedList = std::list<T>;

using String = std::string;

using StringView = std::string_view;

template<typename T, typename N>
using Pair = std::pair<T, N>;

using u64 = uint64_t;
using u32 = uint32_t;
using u16 = uint32_t;
using u8 = uint8_t;

using byte = uint8_t;

using i64 = int64_t;
using i32 = int32_t;
using i16 = int32_t;
using i8 = int8_t;

using f32 = float_t;

using mat4 = glm::mat4;

using v2 = glm::vec2;

using v3 = glm::vec3;

using v4 = glm::vec4;


using iv2 = glm::ivec2;
using iv3 = glm::ivec3;
using iv4 = glm::ivec4;



enum class DataType : u32
{
	INVALID,
	FLOAT,
	INT,
	BYTE,
	CHAR,
	BOOL,
	V2,
	V3,
	V4,
	MAT2,
	MAT3,
	MAT4,
	SAMPLER1D,
	SAMPLER2D,
	SAMPLER3D,
};


template<typename T, typename ... Args>
constexpr Scope<T> create_scope(Args&& ... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}


template<typename T, typename ... Args>
constexpr Ref<T> create_ref(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}


template<typename T, typename N>
inline bool map_contains(Container<T, N>& map, T&& key)
{
	return map.find(key) != map.end();
}

template<typename T, typename N>
inline bool map_contains(Container<T, N>& map, T& key)
{
	return map.find(key) != map.end();
}


}