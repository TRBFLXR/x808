//
// Created by FLXR on 7/8/2018.
//

#ifndef X808_CAMERASYSTEM_HPP
#define X808_CAMERASYSTEM_HPP


#include <xe/gfx/renderer2d.hpp>
#include <xe/ecs/ecssystem.hpp>
#include "orthocameracomponent.hpp"

class OrthoCameraSystem : public xe::BaseECSSystem {
public:
	explicit OrthoCameraSystem(xe::Renderer2D *renderer) : BaseECSSystem(), renderer(renderer) {
		addComponentType(CameraComponent::ID);
	}

	void updateComponents(float delta, xe::BaseECSComponent **components) override {
		CameraComponent *camera = (CameraComponent *) components[0];

		renderer->setCamera(&camera->camera);
	}

private:
	xe::Renderer2D *renderer;
};


#endif //X808_CAMERASYSTEM_HPP
