#version 410 core
out vec4 FragColor;

in vec3 ourColor;
uniform int counter;

void main()
{
    vec4 color = vec4(ourColor,1.0);
    if(mod(counter,250)<75)
    {
        color = vec4(0.0);
    }
    FragColor = color;
}
