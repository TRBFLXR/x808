//
// Created by FLXR on 8/4/2018.
//

#include <xe/resources/texturemanager.hpp>
#include <xe/resources/shadermanager.hpp>
#include <xe/gfx/fx/gbuffer.hpp>
#include "test3d.hpp"

using namespace xe;

Test3D::Test3D(DebugUI *ui) :
		ui(ui) {

	const float width = app.getConfig().width;
	const float height = app.getConfig().height;

	TextureParameters params;

	TextureManager::add(new Texture("rock", "assets/textures/rock.png", params));
	TextureManager::add(new Texture("bgr", "assets/textures/bgr.jfif", params));

	//init objects
	TransformComponent *tc = new TransformComponent();

	//move player
	tc->transform.setPosition({0.0f, 8.0f, -95.0f});

	camera = new Camera(mat4::perspective(75.0f, width / height, 0.1f, 1000.0f));
	camera->transform = tc->transform;

	playerEntity = ecs.makeEntity(new DummyPlayerComponent(camera, 6.0f), tc);

	playerControlSystem = new DummyPlayerControlSystem();
	mainSystems.addSystem(*playerControlSystem);

	Transform *t = &ecs.getComponent<TransformComponent>(playerEntity)->transform;
	ui->trackEntity(L"player", t);

	scene = new Scene("assets/scenes/sponza", "sponza");

	renderer = new DeferredRenderer(width, height);

	ui->sp0->setTexture(renderer->getGBuffer()->getDiffuseTexture());
	ui->sp1->setTexture(renderer->getGBuffer()->getNormalDepthTexture());
	ui->sp2->setTexture(renderer->getGBuffer()->getSpecularTexture());
	ui->sp3->setTexture(renderer->getGBuffer()->getDepthStencilTexture());

	renderer->getGBuffer()->enableLightObjects(true);
}

Test3D::~Test3D() {
	delete scene;

	delete playerControlSystem;

	delete camera;
}

void Test3D::render() {
	ecs.updateSystems(renderingPipeline, 0.0f);

	renderer->render(scene, camera);
}

void Test3D::update(float delta) {
	ecs.updateSystems(mainSystems, delta);

	scene->update(camera->transform, 0.0f);


	if (light) {
		scene->getLights()[0]->transform.setRotation(camera->transform.getRotation());
	}

	//point
	if (Keyboard::isKeyPressed(Keyboard::X)) {
		scene->getLights()[0]->transform.move({10 * delta, 0, 0});
	}
	if (Keyboard::isKeyPressed(Keyboard::C)) {
		scene->getLights()[0]->transform.move({-10 * delta, 0, 0});
	}
}

void Test3D::input(Event &event) {
	ecs.inputSystems(mainSystems, event);

	if (event.type == Event::KeyPressed) {
		if (event.key.code == Keyboard::F1) {
			static bool enabled = false;
			enabled = !enabled;
			renderer->getGBuffer()->enableWireframe(enabled);
		}
		if (event.key.code == Keyboard::F2) {
			static bool enabled = false;
			enabled = !enabled;
			renderer->getFinalFX()->useFXAA(enabled);
		}
		if (event.key.code == Keyboard::F3) {
			static bool enabled = true;
			enabled = !enabled;
			renderer->getGBuffer()->enableLightObjects(enabled);
		}
		if (event.key.code == Keyboard::F4) {
			static bool enabled = false;
			enabled = !enabled;
			renderer->getGBuffer()->enableLightBounds(enabled);
		}
		if (event.key.code == Keyboard::T) {
			Transform *cam = &ecs.getComponent<TransformComponent>(playerEntity)->transform;
			static bool t = false;
			t = !t;
			if (t) {
				cam->setPosition({0, 0, 0});
			} else {
				cam->setPosition({0.0f, 8.0f, -95.0f});
			}
		}
		if (event.key.code == Keyboard::F) {
			light = !light;
		}
	}
}
