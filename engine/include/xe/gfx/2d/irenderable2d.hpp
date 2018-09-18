//
// Created by FLXR on 9/13/2018.
//

#ifndef X808_IRENDERABLE2D_HPP
#define X808_IRENDERABLE2D_HPP


#include <xe/common.hpp>
#include <xe/math/vec3.hpp>
#include <xe/math/vec2.hpp>
#include <xe/gfx/color.hpp>
#include <xe/gfx/texture.hpp>

namespace xe {

	struct Vertex2D {
		vec3 pos;
		vec2 uv;
	};

	class XE_API IRenderable2D {
	public:
		virtual ~IRenderable2D() = default;

		inline uint getColor() const { return color; }
		inline void setColor(uint color) { IRenderable2D::color = color; }

		inline bool isVisible() const { return visible; }
		inline void setVisible(bool visible) { IRenderable2D::visible = visible; }

		virtual const Texture *getTexture() const = 0;

		virtual uint getVerticesSize() const = 0;
		virtual const Vertex2D *getVertices() const = 0;

		virtual uint getIndicesSize() const = 0;
		virtual const uint *getIndices() const = 0;

		virtual float getZ() const = 0;
		virtual const mat4 &getTransformation() const = 0;

	protected:
		explicit IRenderable2D() :
				color(color::WHITE),
				visible(true) { }

	protected:
		uint color;
		bool visible;
	};

}

#endif //X808_IRENDERABLE2D_HPP
