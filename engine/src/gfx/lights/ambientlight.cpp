//
// Created by FLXR on 7/13/2018.
//

#include "ambientlight.hpp"

namespace xe { namespace gfx {

	AmbientLight::AmbientLight(api::Shader *shader, const vec3 &ambientIntensity) :
			BaseLight(shader) {

		setUniform("sys_AmbientIntensity", &ambientIntensity, sizeof(vec3), api::ShaderType::FRAG);
	}

}}