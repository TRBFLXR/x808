//
// Created by FLXR on 6/29/2018.
//

#ifndef X808_BUFFERLAYOUT_HPP
#define X808_BUFFERLAYOUT_HPP


#include <string>
#include <vector>
#include "utils/log.hpp"
#include "xeint.hpp"
#include "gltypes.hpp"

namespace xe { namespace gfx { namespace api {

	struct BufferElement {
		std::string name;
		uint type;
		uint size;
		uint count;
		uint offset;
		bool normalized;

		BufferElement(const std::string_view &name, uint type,
		              uint size, uint count, uint offset, bool normalized) :
				name(name), type(type), size(size), count(count), offset(offset), normalized(normalized) { }
	};

	class BufferLayout {
	public:
		BufferLayout() : size(0) { }

		//todo: d3d cases

		template<typename T>
		void push(const std::string_view &name, uint count = 1, bool normalized = false) {
			XE_ASSERT(false, "Unknown type!"); //for default types only
		}

		inline const std::vector<BufferElement> &getLayout() const { return layout; }
		inline uint getStride() const { return size; }

	private:
		void push(const std::string_view &name, uint type, uint size, uint count, bool normalized) {
			layout.emplace_back(name, type, size, count, size, normalized);
			size += size * count;
		}

	private:
		uint size;
		std::vector<BufferElement> layout;
	};

	template<>
	inline void BufferLayout::push<float>(const std::string_view &name, uint count, bool normalized) {
		push(name, GL_FLOAT, sizeof(float), count, normalized);
	}

	template<>
	inline void BufferLayout::push<uint>(const std::string_view &name, uint count, bool normalized) {
		push(name, GL_UNSIGNED_INT, sizeof(uint), count, normalized);
	}

	template<>
	inline void BufferLayout::push<byte>(const std::string_view &name, uint count, bool normalized) {
		push(name, GL_UNSIGNED_BYTE, sizeof(byte), count, normalized);
	}

	template<>
	inline void BufferLayout::push<vec2>(const std::string_view &name, uint count, bool normalized) {
		push(name, GL_FLOAT, sizeof(vec2), 2 * count, normalized);
	}

	template<>
	inline void BufferLayout::push<vec3>(const std::string_view &name, uint count, bool normalized) {
		push(name, GL_FLOAT, sizeof(vec3), 3 * count, normalized);
	}

	template<>
	inline void BufferLayout::push<vec4>(const std::string_view &name, uint count, bool normalized) {
		push(name, GL_FLOAT, sizeof(vec4), 4 * count, normalized);
	}

}}}


#endif //X808_BUFFERLAYOUT_HPP
