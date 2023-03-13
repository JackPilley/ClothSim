#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

out vec3 vPosition;
out vec3 vNormal;

uniform mat4 uModelViewMatrix;
uniform mat3 uNormalMatrix;
uniform mat4 uProjectionMatrix;

void main()
{
	vPosition = (uModelViewMatrix * vec4(aPosition, 1.0)).xyz;
	vNormal = uNormalMatrix * aNormal;
	gl_Position = uProjectionMatrix * uModelViewMatrix * vec4(aPosition, 1.0);
}