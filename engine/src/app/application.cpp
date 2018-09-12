//
// Created by FLXR on 6/29/2018.
//

#include <xe/utils/random.hpp>
#include <xe/gfx/renderer.hpp>
#include <xe/gfx/layer.hpp>
#include <embedded/embedded.hpp>
#include <xe/resources/fontmanager.hpp>
#include <xe/resources/texturemanager.hpp>
#include <xe/resources/shadermanager.hpp>
#include <xe/resources/soundmanager.hpp>
#include "xe/app/application.hpp"

namespace xe {

	Application *Application::instance = nullptr;

	Application::Application(const Config &config) :
			config(config),
			frameTime(0.0f) {

		//init random
		random::nextInt32(0, 1);

		Context::setRenderAPI(config.renderApi, config.apiVersion);
		instance = this;

		init();
	}

	void Application::init() {
		if (config.fullScreen) {
			window.create(VideoMode::getDesktopMode(), config.title, WindowStyle::Fullscreen);

		} else {
			VideoMode mode(config.width, config.height);
			window.create(mode, config.title, WindowStyle::Default);
		}

		window.setFramerateLimit(config.fps);
		window.setVerticalSyncEnabled(config.vSync);

		ShaderManager::init();
		SoundManager::init();
		FontManager::init();
		TextureManager::init();
	}

	void Application::start() {
		running = true;
		suspend_ = false;
		run();
	}

	void Application::suspend() {
		suspend_ = true;
	}

	void Application::resume() {
		suspend_ = false;
	}

	void Application::stop() {
		running = false;
	}

	void Application::run() {
		Timer timer;

		const float MS_PER_TICK = 1.0f / config.ups;

		uint tickCount = 0;
		uint frames = 0;

		float time = 0.0f;

		float lastTime = timer.elapsed();
		float tickLag = 0;

		while (running) {
			Renderer::resetDC();

			float currentTime = timer.elapsed();
			float delta = currentTime - lastTime;

			lastTime = currentTime;
			tickLag += delta;

			while (tickLag >= MS_PER_TICK) {
				++tickCount;

				fixedUpdate(delta);

				tickLag -= MS_PER_TICK;
			}

			update(delta);

			window.clear();

			Timer frameTimer;
			render();
			frames++;
			frameTime = frameTimer.elapsedMillis();

			window.update();

			processEvents();

			if (timer.elapsed() - time > 1.0f) {
				time += 1.0f;
				fps = frames;
				ups = tickCount;
				frames = 0;
				tickCount = 0;

				tick();
			}

			lateUpdate(delta);

			if (!window.isOpen()) running = false;
		}

		FontManager::clean();
		TextureManager::clean();
		ShaderManager::clean();
	}

	void Application::tick() {
		for (auto &&layer : layerStack) {
			layer->tick();
		}
	}

	void Application::update(float delta) {
		for (auto &&layer : layerStack) {
			layer->update(delta);
		}
	}

	void Application::lateUpdate(float delta) {
		for (auto &&layer : layerStack) {
			layer->lateUpdate(delta);
		}
	}

	void Application::fixedUpdate(float delta) {
		for (auto &&layer : layerStack) {
			layer->fixedUpdate(delta);
		}
	}

	void Application::render() {
		for (auto &&layer : layerStack) {
			if (layer->isVisible()) {
				layer->render();
			}
		}
	}

	void Application::processEvents() {
		xe::Event event{ };

		while (window.pollEvent(event)) {
			for (int i = static_cast<int32>(layerStack.size() - 1); i >= 0; i--) {
				layerStack[i]->input(event);
				if (event.handled) return;
			}

			if (event.type == Event::Closed) {
				window.close();
			}
		}
	}

	void Application::pushLayer(Layer *layer) {
		layerStack.push_back(layer);
		layer->init();
	}

	Layer *Application::popLayer() {
		Layer *layer = layerStack.back();
		layerStack.pop_back();
		return layer;
	}

	Layer *Application::popLayer(Layer *layer) {
		for (uint i = 0; i < layerStack.size(); i++) {
			if (layerStack[i] == layer) {
				layerStack.erase(layerStack.begin() + i);
				break;
			}
		}
		return layer;
	}

}