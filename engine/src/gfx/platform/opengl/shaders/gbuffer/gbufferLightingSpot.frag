layout(location = 0) out vec3 diffuse;
layout(location = 1) out vec4 specular;

uniform sampler2D sampler0;       // Normal
uniform sampler2D sampler1;       // Specular
uniform sampler2D sampler2;       // Position
uniform sampler2DArray sampler3;  // Shadow map

uniform vec3 lightPosition;
uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform float lightIntensity;
uniform float lightFalloff;
uniform float lightSpotAngle;
uniform float lightSpotBlur;

uniform int sid;

void main() {
  vec2 resolution = textureSize(sampler0, 0);
  vec2 uv = gl_FragCoord.xy / resolution;

  vec3 normal = texture(sampler0, uv).xyz;
  vec4 specularProperties = texture(sampler1, uv);
  vec3 worldPosition = texture(sampler2, uv).xyz;

  vec3 L;
  vec3 tempDiffuse;
  vec4 tempSpecular;

  float shadowFactor = 1.0;
  if (sid > -1) {
    vec4 suv = spotShadows[sid].projection * spotShadows[sid].view * vec4(worldPosition, 1.0);
    vec2 suvw = (suv.xyz / suv.w).xy * 0.5 + 0.5;
    float d = (spotShadows[sid].view * vec4(worldPosition, 1.0)).z;
    shadowFactor = shadowContributionSpot(vec3(suvw, sid), d, sampler3);
  }

  calcLighting(worldPosition, normal, camPosition.xyz, lightPosition, lightColor, lightIntensity, lightFalloff,
               specularProperties, L, tempDiffuse, tempSpecular);

  float visibility = calcSpotLightCone(L, lightDirection, lightSpotAngle, lightSpotBlur);

  diffuse = tempDiffuse * visibility * shadowFactor;
  specular = tempSpecular * visibility * shadowFactor;
}
