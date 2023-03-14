#version 330 core

in vec3 vNormal;

out vec4 FragColor;

uniform mat4 uViewMatrix;
uniform vec3 uLightDirection;

void main()
{
	vec3 lightDir = (uViewMatrix * vec4(uLightDirection, 0.0)).xyz;
	vec3 normal = normalize(vNormal);

	if(gl_FrontFacing == false) normal = -normal;

	vec3 color = vec3(0.2,0.7,1.0);
	float diffuseFactor = max(dot(lightDir, normal), 0.0);

	color = diffuseFactor * color;

	FragColor = vec4(color, 1.0);
}