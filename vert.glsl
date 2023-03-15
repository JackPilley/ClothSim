#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

out vec3 vNormal;

uniform mat4 uProjectionMatrix = mat4(1.0);
uniform mat4 uModelViewMatrix = mat4(1.0);
uniform mat3 uNormalMatrix = mat3(1.0);

void main()
{
	vNormal = uNormalMatrix * aNormal;
	gl_Position = uProjectionMatrix * uModelViewMatrix * vec4(aPosition, 1.0);
}