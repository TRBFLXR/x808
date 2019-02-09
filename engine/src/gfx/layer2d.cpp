//
// Created by FLXR on 2/9/2019.
//

#include <xe/gfx/layer2d.hpp>

namespace xe {

	Layer2D::~Layer2D() {
		delete camera;
		delete renderer;
	}

	void Layer2D::createCamera(uint width, uint height, float near, float far) {
		camera = new Camera(mat4::ortho(0.0f, width, 0.0f, height, -1.0f, 1000.0f));
	}

	void Layer2D::createRenderer(uint width, uint height, bool enableLighting,
	                             Shader *customShader, Shader *customTextShader) {
		XE_ASSERT(camera, "Error: 'createCamera' must be called first!")

		renderer = new BatchRenderer2D(width, height, camera, enableLighting, customShader, customTextShader);
	}

	void Layer2D::submit(IRenderable2D *renderable) {
		renderer->submit(renderable);
	}

	void Layer2D::submit(Text *text) {
		renderer->submit(text);
	}

	void Layer2D::submit(Light2D *light) {
		renderer->submit(light);
	}

	void Layer2D::render() {
		renderScene();

		renderer->render();

		renderer->clear();
	}

}