#version 330 core
precision highp float;

in vec3 vPosition;
in vec3 vNormal;

layout(location = 0) out vec4 FragColor;

uniform mat4 uViewMatrix;
uniform vec3 uLightDirection;

void main()
{
	vec3 lightDir = (uViewMatrix * vec4(uLightDirection, 1.0)).xyz;
	vec3 normal = vNormal;

	if(!gl_FrontFacing) normal = -normal;

	normal = normalize(normal);

	FragColor = vec4(dot(lightDir, normal) * vec3(0.2,0.7,1.0), 1.0);
}