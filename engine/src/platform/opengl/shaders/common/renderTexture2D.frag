R"(
out vec4 color;

in vec2 uv;

uniform sampler2D sampler0;

uniform int channel;

void main() {
	vec4 tex = texture(sampler0, uv);

	color = vec4((channel < 0) ? tex : vec4(tex[channel]));
}
)"
