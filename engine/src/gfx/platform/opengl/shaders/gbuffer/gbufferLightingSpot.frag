layout(location = 0) out vec4 diffuse;
layout(location = 1) out vec4 specular;

uniform sampler2D sampler0;  // Normal
uniform sampler2D sampler1;  // Specular
uniform sampler2D sampler2;  // Position

uniform vec3 lightPosition;
uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform float lightIntensity;
uniform float lightFalloff;
uniform float lightSpotAngle;
uniform float lightSpotBlur;

void main() {
  vec2 resolution = textureSize(sampler0, 0);
  vec2 uv = gl_FragCoord.xy / resolution;

  vec3 normal = texture(sampler0, uv).xyz;
  vec4 specularProperties = texture(sampler1, uv);
  vec3 worldPosition = texture(sampler2, uv).xyz;

  vec3 L;
  vec4 tempDiffuse;
  vec4 tempSpecular;

  calcLighting(uv, worldPosition, normal.xyz, camPosition.xyz, lightPosition, lightColor, lightIntensity,
               lightFalloff, specularProperties, L, tempDiffuse, tempSpecular);

  float visibility = calcSpotLightCone(L, lightDirection, lightSpotAngle, lightSpotBlur);

  // diffuse = vec4(worldPosition, 1.0);
  diffuse = tempDiffuse;
  // diffuse = tempDiffuse * visibility;
  // specular = tempSpecular * visibility;
  specular = vec4(0, 0, 0, 0);
}
