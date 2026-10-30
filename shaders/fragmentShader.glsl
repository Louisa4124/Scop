#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform float u_Transition;

float rnd(float x) { return fract(sin(x) * 43758.5453123); }
float grayFromID(int id) {
    float f = float(id);
    float g = rnd(f * 0.61803398875 + 1.0);
    g = 0.2 + 0.7 * g;
    return g;
}

void main()
{
    // couleur du modele sans texture
    int pid = gl_PrimitiveID;
    float gray = grayFromID(pid);
    vec4 colorWithoutTexture = vec4(vec3(gray), 1.0f);

    // texture
    vec4 colorWithTexture = texture(texture1, TexCoord);

    // transition
    FragColor = mix(colorWithoutTexture, colorWithTexture, u_Transition);
}