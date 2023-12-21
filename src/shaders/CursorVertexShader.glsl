#version 330 core
layout (location = 0) in vec2 aPos;

out vec3 ourColor;

uniform vec3 in_color;
uniform mat4 orthoMat;
uniform mat4 modelMat;

void main()
{
        vec4 Position = orthoMat*modelMat*vec4(aPos,0.0, 1.0);
	Position.z = -1.0;
	gl_Position = Position;
	ourColor = in_color;
}
