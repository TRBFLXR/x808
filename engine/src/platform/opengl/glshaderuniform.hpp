//
// Created by FLXR on 7/6/2018.
//

#ifndef X808_GLSHADERUNIFORM_HPP
#define X808_GLSHADERUNIFORM_HPP


#include "gfx/api/shaderuniform.hpp"

namespace xe { namespace api {

	class XE_API GLShaderUniform : public ShaderUniform {
	private:
		friend class GLShader;
		friend class GLShaderUniformBuffer;

	public:
		enum class Type {
			NONE, INT32, FLOAT32, VEC2, VEC3, VEC4, MAT4, STRUCT
		};

	public:
		explicit GLShaderUniform(Type type, const std::string_view &name, uint count = 1);
		explicit GLShaderUniform(ShaderStruct *uniformStruct, const std::string_view &name, uint count = 1);

		inline const std::string &getName() const override { return name; }
		inline uint getSize() const override { return size; }
		inline uint getCount() const override { return count; }
		inline uint getOffset() const override { return offset; }
		inline uint getAbsoluteOffset() const { return struct_ ? struct_->getOffset() + offset : offset; }

		uint getLocation() const { return location; }
		inline Type getType() const { return type; }
		const ShaderStruct &getShaderUniformStruct() const;

		static uint sizeOfUniformType(Type type);
		static Type stringToType(const std::string_view &type);
		static std::string typeToString(Type type);

	protected:
		void setOffset(uint offset) override;

	private:
		std::string name;
		uint size;
		uint count;
		uint offset;

		Type type;
		ShaderStruct *struct_;
		mutable uint location;
	};

	///------------------------------///
	class XE_API GLShaderUniformBuffer : public ShaderUniformBuffer {
	public:
		explicit GLShaderUniformBuffer(const std::string_view &name);

		void pushUniform(GLShaderUniform *uniform);

		inline const std::string &getName() const override { return name; }
		inline uint getRegister() const override { return register_; }
		inline uint getSize() const override { return size; }
		inline const ShaderUniformVec &getUniforms() const override { return uniforms; }

		ShaderUniform *findUniform(const std::string_view &name) override;

	private:
		std::string name;
		ShaderUniformVec uniforms;
		uint register_;
		uint size;
	};
	typedef std::vector<ShaderUniformBuffer *> ShaderUniformBufferVec;

}}


#endif //X808_GLSHADERUNIFORM_HPP
