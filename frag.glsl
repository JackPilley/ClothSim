#version 330 core

in vec3 vNormal;
in vec3 vPosition;

out vec4 FragColor;

uniform mat4 uViewMatrix = mat4(1.0);
uniform vec3 uLightDirection;

void main()
{
	vec3 lightDir = -(uViewMatrix * vec4(uLightDirection, 0.0)).xyz;
	vec3 normal = normalize(vNormal);
	//vec3 normal = normalize(cross(dFdx(vPosition), dFdy(vPosition)));

	vec3 baseColor = vec3(0.3,0.8,1.0);

	if(gl_FrontFacing)
	{
		normal = -normal;
		baseColor = vec3(1.0,0.2,0.5);
	}

	vec3 color = baseColor * max(dot(normal, lightDir), 0.0);
	color = color + baseColor * 0.1;

	//FragColor = vec4(color ,1.0);
	FragColor = vec4(1.0);
}