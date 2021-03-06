#version 330 core
layout (location = 0) in vec3 aPos;

		
uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * worldMatrix * vec4(aPos.x,aPos.y,aPos.z, 1.0);
}