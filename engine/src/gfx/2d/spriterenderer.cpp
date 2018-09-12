//
// Created by FLXR on 9/6/2018.
//

#include <algorithm>
#include <xe/gfx/renderer.hpp>
#include <xe/gfx/2d/spriterenderer.hpp>

namespace xe {

	SpriteRenderer::SpriteRenderer(uint width, uint height, Camera *camera) :
			IRenderer2D(width, height, camera) {

		shader = new Shader("dSpriteRenderer");
	}

	void SpriteRenderer::begin() {
		Renderer::enableBlend(true);
		Renderer::setBlendFunction(BlendFunction::SourceAlpha, BlendFunction::OneMinusSourceAlpha);
		Renderer::setViewport(0, 0, width, height);

		vertexArray->bind();
		buffer = static_cast<VertexData *>(vertexArray->getBuffer(0)->getPointer());
	}

	void SpriteRenderer::end() {
		updateCamera();

		Renderer::enableDepthTesting(true);

		std::sort(targets.begin(), targets.end(),
		          [](const RenderTarget2D a, const RenderTarget2D b) {
			          return a.sprite->getTexture() > b.sprite->getTexture();
		          });

		for (auto &&target : targets) {
			submitInternal(target);
		}
		targets.clear();

		//draw transparent
		if (!transparentTargets.empty()) {
			//todo: there must be a better solution
			std::sort(transparentTargets.begin(), transparentTargets.end(),
			          [](const RenderTarget2D a, const RenderTarget2D b) {
				          return a.transform->getPosition().z < b.transform->getPosition().z;
			          });

			for (auto &&target : transparentTargets) {
				submitInternal(target);
			}
			transparentTargets.clear();
		}

		releaseBuffer();
	}

	void SpriteRenderer::flush() {
		shader->bind();
		shader->updateUniforms();

		for (uint i = 0; i < textures.size(); i++) {
			textures[i]->bind(i);
		}

		vertexArray->bind();
		indexBuffer->bind();

		vertexArray->drawElements(indexCount, BeginMode::Triangles);

		indexBuffer->unbind();
		vertexArray->unbind();

		for (uint i = 0; i < textures.size(); i++) {
			textures[i]->unbind(i);
		}

		shader->unbind();

		indexCount = 0;
		textures.clear();

		//increment draw calls
		Renderer::incDC();
	}

	void SpriteRenderer::render(const std::vector<RenderTarget2D> &targets) {
		begin();

		for (const auto &t : targets) {
			submitInternal(t);
		}

		releaseBuffer();
		flush();
	}

	void SpriteRenderer::submit(const Sprite *sprite, const Transform2D *transform) {
		if (!sprite->isVisible()) return;

		if (sprite->hasTransparency()) {
			transparentTargets.emplace_back(sprite, transform);
		} else {
			targets.emplace_back(sprite, transform);
		}
	}

	void SpriteRenderer::submitInternal(const RenderTarget2D &target) {
		const std::array<vec3, 4> &vertices = target.transform->getVertices();
		const uint color = target.sprite->getColor();
		const std::array<vec2, 4> &uv = target.sprite->getUVs();
		const Texture *texture = target.sprite->getTexture();

		float textureSlot = 0.0f;
		if (texture) {
			textureSlot = submitTexture(texture);
		}

		buffer->vertex = mat4::translateVec(*transformationBack, vertices[0]);
		buffer->uv = uv[0];
		buffer->tid = textureSlot;
		buffer->color = color;
		buffer++;

		buffer->vertex = mat4::translateVec(*transformationBack, vertices[3]);
		buffer->uv = uv[1];
		buffer->tid = textureSlot;
		buffer->color = color;
		buffer++;

		buffer->vertex = mat4::translateVec(*transformationBack, vertices[2]);
		buffer->uv = uv[2];
		buffer->tid = textureSlot;
		buffer->color = color;
		buffer++;

		buffer->vertex = mat4::translateVec(*transformationBack, vertices[1]);
		buffer->uv = uv[3];
		buffer->tid = textureSlot;
		buffer->color = color;
		buffer++;

		indexCount += 6;
	}

}