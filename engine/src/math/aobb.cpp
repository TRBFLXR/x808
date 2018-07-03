//
// Created by FLXR on 7/2/2018.
//

#include "aobb.hpp"
#include <math/math.hpp>

xe::math::aobb::aobb() : transformed(true), position(0, 0), size(0, 0), rotation(0.0f) { }
xe::math::aobb::aobb(const vec2 &position, const vec2 &size, float rotation) :
		transformed(true), position(position), size(size), rotation(rotation) { }

xe::math::aobb::aobb(float x, float y, float width, float height, float rotation) :
		transformed(true), position(x, y), size(width, height), rotation(rotation) { }

xe::math::aobb::aobb(const aobb &other) :
		transformed(true), position(other.position), size(other.size), rotation(other.rotation) { }


bool xe::math::aobb::contains(const xe::vec2 &point) const {
	bool result = false;
	for (char i = 0, j = 3; i < 4; j = i++) {
		if (((vertices[i].y > point.y) != (vertices[j].y > point.y)) &&
		    (point.x < (vertices[j].x - vertices[i].x) * (point.y - vertices[i].y) /
		               (vertices[j].y - vertices[i].y) + vertices[i].x)) {
			result = !result;
		}
	}
	return result;
}

void xe::math::aobb::move(const xe::vec2 &direction) {
	setPosition(position + direction);
}

void xe::math::aobb::rotate(float deg) {
	setRotation(rotation + deg);
}

void xe::math::aobb::setPosition(const xe::vec2 &position) {
	aobb::position = position;
	transformed = true;
}

void xe::math::aobb::setSize(const xe::vec2 &size) {
	aobb::size = size;
	transformed = true;
}

void xe::math::aobb::setRotation(float deg) {
	rotation = fmodf(deg, 360.0f);
	if (rotation < 0.0f) {
		rotation += 360.0f;
	}
	transformed = true;
}

const std::array<xe::vec2, 4> &xe::math::aobb::getVertices() const {
	if (transformed) {
		transformed = false;

		const float localX = -size.x / 2.0f;
		const float localY = -size.y / 2.0f;
		const float localX2 = localX + size.x;
		const float localY2 = localY + size.y;
		const float worldOriginX = position.x - localX;
		const float worldOriginY = position.y - localY;

		const float cos = cosf(to_rad(rotation));
		const float sin = sinf(to_rad(rotation));
		const float localXCos = localX * cos;
		const float localXSin = localX * sin;
		const float localYCos = localY * cos;
		const float localYSin = localY * sin;
		const float localX2Cos = localX2 * cos;
		const float localX2Sin = localX2 * sin;
		const float localY2Cos = localY2 * cos;
		const float localY2Sin = localY2 * sin;

		vertices[0] = {localXCos - localYSin + worldOriginX, localYCos + localXSin + worldOriginY};
		vertices[1] = {localXCos - localY2Sin + worldOriginX, localY2Cos + localXSin + worldOriginY};
		vertices[2] = {localX2Cos - localY2Sin + worldOriginX, localY2Cos + localX2Sin + worldOriginY};
		vertices[3] = {vertices[0].x + (vertices[2].x - vertices[1].x), vertices[2].y - (vertices[1].y - vertices[0].y)};
	}

	return vertices;
}