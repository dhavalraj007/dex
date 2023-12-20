#version 410 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
uniform mat4 orthoMat;
uniform mat4 modelMat;

const float pos_infinity = uintBitsToFloat(0x7F800000);
const float neg_infinity = uintBitsToFloat(0xFF800000);
void main() {
  // if vertices are those that we dont want to draw(or not be visible)
  if (aPos.x == pos_infinity && aPos.y == pos_infinity &&
      aTexCoord.x == pos_infinity && aTexCoord.y == pos_infinity) {
    // move outside [-1,1] cube
    gl_Position = vec4(2.0, 2.0, 2.0, 1.0);
    TexCoord = vec2(0.0, 0.0);
  } else {
    gl_Position = orthoMat * modelMat * vec4(aPos, 0.0f, 1.0);
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
  }
}
