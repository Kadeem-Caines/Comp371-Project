#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix= mat4(1.0);
uniform mat4 projectionMatrix= mat4(1.0);

void main()
{
    FragPos = vec3(worldMatrix * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(worldMatrix))) * aNormal;  
    
    gl_Position =projectionMatrix * viewMatrix * vec4(FragPos, 1.0);
}