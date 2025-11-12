#version 330 core
in vec3 TexCoords;
out vec4 FragColor;

uniform samplerCube skybox;
uniform float uDarken;   // 1.0 = normal, 0.0 = negro

void main()
{
    vec3 col = texture(skybox, TexCoords).rgb;
    col *= uDarken;                // oscurece el skybox
    FragColor = vec4(col, 1.0);
}
