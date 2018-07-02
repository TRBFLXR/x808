//
// Created by FLXR on 6/28/2018.
//

#ifndef X808_MATH_HPP
#define X808_MATH_HPP


#define XE_PI 3.14159265359f

#define to_rad(deg) ((deg) * XE_PI / 180.0f)
#define to_deg(rad) ((rad) * 180.0f / XE_PI)

#define __max(a, b) (((a) > (b)) ? (a) : (b))
#define __min(a, b) (((a) < (b)) ? (a) : (b))

#include <cmath>

#include "xeint.hpp"

#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"
#include "mat4.hpp"
#include "quat.hpp"
#include "aabb.hpp"
#include "aobb.hpp"
#include "rect.hpp"
#include "tvec2.hpp"


#endif //X808_MATH_HPP
