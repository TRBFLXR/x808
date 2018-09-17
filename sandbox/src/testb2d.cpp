//
// Created by FLXR on 9/11/2018.
//

#include <xe/resources/texturemanager.hpp>
#include "testb2d.hpp"

using namespace xe;

TestB2D::TestB2D() {
	const float width = app.getConfig().width;
	const float height = app.getConfig().height;

	TextureParameters params;

	TextureManager::add(new Texture("0", "assets/textures/enemyspotted.png", params));
	TextureManager::add(new Texture("1", "assets/textures/jdm.png", params, true));
	TextureManager::add(new Texture("2", "assets/textures/star.png", params, true));
	TextureManager::add(new Texture("3", "assets/textures/test1.png", params));
	TextureManager::add(new Texture("4", "assets/textures/test8.png", params));


	//create camera
	camera = new Camera(mat4::ortho(0, width, 0, height, -1, 1000));

	renderer = new BatchRenderer2D(width, height, camera);


	box = new RectangleShape({50.0f, 50.0f});
	box->setTexture(GETTEXTURE("0"));
	box->transformation({400.0f, 400.0f, 1.0f});

	ground = new RectangleShape({500.0f, 5.0f});
	ground->setTexture(GETTEXTURE("3"));
	ground->transformation({400.0f, 50.0f, 0.0f}, 20.0f);

	sprite = new RectangleShape({50.0f, 50.0f});
	sprite->setTexture(GETTEXTURE("1"));
	sprite->transformation({400.0f, 400.0f, 2.0f});

	world = new PhysicsWorld2D({0.0f, -9.8f});

	boxCollider = new BoxCollider2D(world, ColliderType::Dynamic, box);
	boxCollider->setDensity(20.5f);
	boxCollider->setFriction(0.2f);
	boxCollider->setRestitution(0.5f);

	groundCollider = new BoxCollider2D(world, ColliderType::Static, ground);
}

TestB2D::~TestB2D() {
	delete camera;
	delete renderer;

	delete boxCollider;
	delete groundCollider;

	delete box;
	delete ground;
	delete sprite;

	delete world;
}

void TestB2D::render() {
	renderer->submit(box);
	renderer->submit(ground);
	renderer->submit(sprite);

	renderer->renderSprites();
	renderer->renderText();

	renderer->clear();
}

void TestB2D::update(float delta) {
	if (Mouse::isButtonPressed(Mouse::Left)) {
		const vec2 p = Mouse::getPosition(window);
		boxCollider->set(p, boxCollider->getRotation());
//		boxCollider->setLinearVelocity({0, 0});
//		boxCollider->setAngularVelocity(0);
		boxCollider->setAwake(true);
	}

	world->update(delta, 6, 3);
}

void TestB2D::input(xe::Event &event) {


	switch (event.type) {
		case Event::KeyPressed: {
			if (event.key.code == Keyboard::Up) {
				box->setSize(box->getSize() + 1);
			} else if (event.key.code == Keyboard::Down) {
				box->setSize(box->getSize() - 1);
			}
			break;
		}

		case Event::MouseButtonPressed: {
			if (event.mouseButton.button == Mouse::Right) {
				static bool wake = false;
				boxCollider->setAwake(wake);
				wake = !wake;
			}
			break;
		}

		default: break;
	}
}

void TestB2D::tick() {

}
