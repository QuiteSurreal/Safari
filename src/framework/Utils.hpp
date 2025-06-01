#pragma once

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <string>
#include <cstdarg>
#include <cmath>
#include <numbers>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>
#include <glm/gtc/type_ptr.hpp>

constexpr const char *DIR_SPRITES  = "assets/sprites/";
constexpr const char *DIR_FONTS = "assets/fonts/";
constexpr const char *DIR_SHADERS = "assets/shaders/";

constexpr float PI_F = static_cast<float>(std::numbers::pi);
constexpr float PI_2_F = PI_F / 2;
constexpr float PI_4_F = PI_F / 4;

using Uint = unsigned int;

using Seconds = float;

using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;

using Ivec2 = glm::ivec2;
using Ivec3 = glm::ivec3;
using Ivec4 = glm::ivec4;

using Uvec2 = glm::uvec2;
using Uvec3 = glm::uvec3;
using Uvec4 = glm::uvec4;

using Mat2 = glm::mat2;
using Mat3 = glm::mat3;
using Mat4 = glm::mat4;

void logDebug(const char *format, ...);
void logInfo(const char *format, ...);
[[noreturn]] void logFatal(const char *format, ...);

float randf(float min, float max);

std::string readTextFile(const std::string &path);

template<typename ... Ts>
struct Overload : Ts ... {
    using Ts::operator() ...;
};
template<class... Ts> Overload(Ts...) -> Overload<Ts...>;
