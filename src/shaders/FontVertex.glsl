#version 410 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
uniform mat4 orthoMat;
uniform mat4 modelMat;

void main()
{
	gl_Position = orthoMat*modelMat*vec4(aPos,0.0f, 1.0);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}