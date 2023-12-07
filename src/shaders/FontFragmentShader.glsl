#version 410 core
in vec2 TexCoord;

out vec4 outcolor;

uniform sampler2D ourTexture;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}
const float pxRange = 2.0;
float screenPxRange() {
    vec2 unitRange = vec2(pxRange)/vec2(textureSize(ourTexture, 0));
    vec2 screenTexSize = vec2(1.0)/fwidth(TexCoord);
    return max(0.5*dot(unitRange, screenTexSize), 1.0);
}

void main() {
    vec4 fgColor = vec4(1.0,1.0,1.0,1.0);

    vec3 msd = texture(ourTexture, TexCoord).rgb;
    float sd = median(msd.r, msd.g, msd.b);
    float screenPxDistance = screenPxRange()*(sd - 0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
    if(opacity<=0.1f)
            discard;
    outcolor = mix(vec4(0.0,0.0,0.0,0.0), fgColor, opacity);
}