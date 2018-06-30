//
// Created by FLXR on 6/30/2018.
//

#include "texture2d.hpp"
#include "gfx/glcommon.hpp"
#include "utils/loadimage.hpp"

xe::gfx::api::Texture2D::Texture2D(uint width, uint height,
                                   xe::gfx::api::TextureParameters params,
                                   xe::gfx::api::TextureLoadOptions options) :
		fileName("NULL"), width(width), height(height), parameters(params), loadOptions(options) {

	handle = load();
}

xe::gfx::api::Texture2D::Texture2D(uint width, uint height, uint color,
                                   xe::gfx::api::TextureParameters params,
                                   xe::gfx::api::TextureLoadOptions options) :
		fileName("NULL"), width(width), height(height), parameters(params), loadOptions(options) {

	handle = load();
	setData(color);
}

xe::gfx::api::Texture2D::Texture2D(const std::string_view &name,
                                   const std::string_view &path,
                                   xe::gfx::api::TextureParameters params,
                                   xe::gfx::api::TextureLoadOptions options) :
		name(name), fileName(path), parameters(params), loadOptions(options) {

	handle = load();
}

xe::gfx::api::Texture2D::~Texture2D() {
	glCall(glDeleteTextures(1, &handle));
}

void xe::gfx::api::Texture2D::bind(uint slot) const {
	glCall(glActiveTexture(GL_TEXTURE0 + slot));
	glCall(glBindTexture(GL_TEXTURE_2D, handle));
}

void xe::gfx::api::Texture2D::unbind(uint slot) const {
	glCall(glActiveTexture(GL_TEXTURE0 + slot));
	glCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void xe::gfx::api::Texture2D::setData(const void *pixels) {
	glCall(glBindTexture(GL_TEXTURE_2D, handle));
	glCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height,
	                       textureFormatToGL(parameters.format), GL_UNSIGNED_BYTE, pixels));
}

void xe::gfx::api::Texture2D::setData(uint color) {
	XE_ASSERT(false, "not implemented");
}

uint xe::gfx::api::Texture2D::load() {
	// todo: split  into loading from file and generating from data
	byte *pixels = nullptr;

	if (fileName != "NULL") {
		uint bits;
		// FreeImage loads bottom->top
		pixels = utils::loadImage(fileName.data(), &width, &height, &bits, !loadOptions.flipY);

		if (bits != 24 && bits != 32) {
			XE_FATAL("[Texture] Unsupported image bit-depth! (', bits, ')");
		}
		parameters.format = bits == 24 ? TextureFormat::RGB : TextureFormat::RGBA;
	}

	uint handle;
	glCall(glGenTextures(1, &handle));
	glCall(glBindTexture(GL_TEXTURE_2D, handle));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
	                       parameters.filter == TextureFilter::LINEAR ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST));

	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
	                       parameters.filter == TextureFilter::LINEAR ? GL_LINEAR : GL_NEAREST));

	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrapToGL(wrapMode)));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrapToGL(wrapMode)));

	glCall(glTexImage2D(GL_TEXTURE_2D, 0, textureFormatToGL(parameters.format), width, height, 0,
	                    textureFormatToGL(parameters.format), GL_UNSIGNED_BYTE, pixels ? pixels : nullptr));

	glCall(glGenerateMipmap(GL_TEXTURE_2D));
	glCall(glBindTexture(GL_TEXTURE_2D, 0));

	delete[] pixels;

	return handle;
}

uint xe::gfx::api::Texture2D::textureFormatToGL(xe::gfx::api::TextureFormat format) {
	switch (format) {
		case TextureFormat::RGBA: return GL_RGBA;
		case TextureFormat::RGB: return GL_RGB;
		case TextureFormat::LUMINANCE: return GL_LUMINANCE;
		case TextureFormat::LUMINANCE_ALPHA: return GL_LUMINANCE_ALPHA;
		default: return 0;
	}
}

uint xe::gfx::api::Texture2D::textureWrapToGL(xe::gfx::api::TextureWrap wrap) {
	switch (wrap) {
		case TextureWrap::CLAMP: return GL_CLAMP;
		case TextureWrap::CLAMP_TO_BORDER: return GL_CLAMP_TO_BORDER;
		case TextureWrap::CLAMP_TO_EDGE: return GL_CLAMP_TO_EDGE;
		case TextureWrap::REPEAT: return GL_REPEAT;
		case TextureWrap::MIRRORED_REPEAT: return GL_MIRRORED_REPEAT;
		default: return 0;
	}
}