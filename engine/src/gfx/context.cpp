//
// Created by FLXR on 6/28/2018.
//

#include <xe/gfx/context.hpp>
#include <gfx/platform/opengl/glcontext.hpp>

namespace xe {

	Context *Context::context = nullptr;
	RenderAPI Context::api = RenderAPI::Invalid;
	uint Context::apiVersion = 0;
	uint Context::maxTextureUnits = 1;

	void Context::create(void *deviceContext) {
		switch (getRenderAPI()) {
			case RenderAPI::OpenGL: context = new internal::GLContext(deviceContext);
				break;
			default: break;
		}
	}

	void Context::setMaxTextureUnits(uint size) {
		if (size == 0) {
			size = context->getMaxTexUnits();
		}

		maxTextureUnits = size;
	}

}