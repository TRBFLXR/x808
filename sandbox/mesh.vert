layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec2 uv;

uniform mat4 projection;
uniform mat4 view;

out vec2 uv0;

void main() {
  gl_Position = (projection * view) * vec4(position, 1.0);

  uv0 = uv;
}
