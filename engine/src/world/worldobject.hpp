//
// Created by FLXR on 8/13/2018.
//

#ifndef X808_WORLDOBJECT_HPP
#define X808_WORLDOBJECT_HPP


#include <string>
#include "spatialdata.hpp"

namespace xe {

	class XE_API WorldObject {
	public:
		SpatialData spatial;

	public:
		explicit WorldObject(const SpatialData &data);

		virtual void rotate(const vec3 &angles);
		virtual void rotate(float x, float y, float z);

		void moveForward(float speed);
		void moveBackward(float speed);
		void moveUp(float speed);
		void moveDown(float speed);
		void moveRight(float speed);
		void moveLeft(float speed);

		const std::string &getId() const { return id; }

	private:
		std::string id;
	};

}


#endif //X808_WORLDOBJECT_HPP
