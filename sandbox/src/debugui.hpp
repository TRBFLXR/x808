//
// Created by FLXR on 7/24/2018.
//

#ifndef X808_TESTUI_HPP
#define X808_TESTUI_HPP


#include <gfx/layers/layer2d.hpp>
#include <ecs/components/transform2dcomponent.hpp>
#include <ecs/systems/spriterenderersystem.hpp>
#include <ecs/systems/textrenderersystem.hpp>
#include "orthocamerasystem.hpp"

class DebugUI : public xe::gfx::Layer2D {
private:
	friend class Test3D;

public:
	explicit DebugUI();
	~DebugUI() override;

	void render() override;

	void update(float delta) override;
	void lateUpdate(float delta) override;

	void input(xe::Event &event) override;

private:
	xe::ECS ecs;

	OrthoCameraSystem *cameraSystem;
	xe::EntityHandle cameraEntity;

	xe::SpriteRendererSystem *spriteRenderer;
	xe::TextRendererSystem *textRenderer;

	xe::ECSSystemList mainSystems;
	xe::ECSSystemList renderingPipeline;

	xe::EntityHandle fpsText;
	xe::EntityHandle upsText;
	xe::EntityHandle frameTimeText;
	xe::EntityHandle dcText;
};


#endif //X808_TESTUI_HPP