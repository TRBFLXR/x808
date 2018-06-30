//
// Created by FLXR on 6/29/2018.
//

#include "layer2d.hpp"
#include "application/application.hpp"

xe::gfx::Layer2D::Layer2D(const xe::mat4 &projectionMatrix) {
	const uint width = Application::getApplication().getWindowWidth();
	const uint height = Application::getApplication().getWindowHeight();

	renderer = new Renderer2D(width, height);
}

xe::gfx::Layer2D::~Layer2D() {

}

void xe::gfx::Layer2D::init() {
	init(*renderer);
}

void xe::gfx::Layer2D::init(xe::gfx::Renderer2D &renderer) {

}

void xe::gfx::Layer2D::render() {
	renderer->begin();

	renderer->drawLine(0, 0, 100, 100, 0xffffffff, 0.1f);

	renderer->fillRect({-2,-2},{5,5}, 0xffff00ff);

	renderer->end();
	renderer->flush();

	render(*renderer);
}

void xe::gfx::Layer2D::render(xe::gfx::Renderer2D &renderer) {

}

bool xe::gfx::Layer2D::resize(uint width, uint height) {
	return false;
}