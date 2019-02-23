//
// Created by FLXR on 2/19/2019.
//

#include <xe/gfx/particles/particleeffect.hpp>
#include <xe/timeline/ramp.hpp>

namespace xe {

	ParticleEffect::ParticleEffect(float duration, float change, uint count, bool looped, float z) :
			duration(duration),
			change(change),
			looped(looped),
			finished(true),
			count(count),
			z(z),
			texture(nullptr) {

		for (uint i = 0; i < count; ++i) {
			Particle *p = new Particle(this, z);
			p->setVisible(false);
			particles.push_back(p);
		}
	}

	ParticleEffect::~ParticleEffect() {
		for (const auto &p : particles) {
			delete p;
		}
	}

	void ParticleEffect::create() {
		for (auto &&p : particles) {
			Particle *s = dynamic_cast<Particle *>(p);

			Ramp<float> *rotation = new Ramp<float>(rotationStates, Ramp<float>::lerp);
			Ramp<vec2> *translation = new Ramp<vec2>(translationStates, Ramp<vec2>::lerp);
			Ramp<vec2> *size = new Ramp<vec2>(sizeStates, Ramp<vec2>::lerp);
			Ramp<vec4> *color = new Ramp<vec4>(colorStates, Ramp<vec4>::lerp);

			s->setRotationRamp(rotation);
			s->setTranslationRamp(translation);
			s->setSizeRamp(size);
			s->setColorRamp(color);

			s->setTexture(texture);
		}
	}

	void ParticleEffect::update(float delta) {
		if (finished) return;

		for (auto &&p : particles) {
			if (p->isVisible()) {
				Particle *s = dynamic_cast<Particle *>(p);

				s->update(delta);
				if (!s->isVisible()) {
					spawnQueue.push(s);

					if (spawnQueue.size() == count) {
						finished = true;
					}
				}
			}
		}

		if (looped) {
			if (!spawnQueue.empty()) {
				const float d = random::next<float>(duration - change, duration + change);

				Particle *p = spawnQueue.front();
				spawnQueue.pop();
				p->spawn(d);
			}
		}
	}

	void ParticleEffect::play() {
		if (!finished) return;

		finished = false;

		for (auto &&p : particles) {
			Particle *s = dynamic_cast<Particle *>(p);
			const float d = random::next<float>(duration - change, duration + change);

			s->spawn(d);
		}

		while (!spawnQueue.empty()) {
			spawnQueue.pop();
		}
	}

	void ParticleEffect::stop() {
		if (finished) return;

		finished = true;

		for (auto &&p : particles) {
			p->setVisible(false);
		}

		while (!spawnQueue.empty()) {
			spawnQueue.pop();
		}
	}

}