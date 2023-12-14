#version 330 core
layout (location = 0) in vec2 aPos;

out vec3 ourColor;

uniform vec3 in_color;
uniform mat4 orthoMat;
uniform mat4 modelMat;

void main()
{
	gl_Position = orthoMat*modelMat*vec4(aPos,0.0f, 1.0);
	ourColor = in_color;
}