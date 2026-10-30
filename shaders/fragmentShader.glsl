#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform bool u_UseTexture; // pour pouvoir utiliser avec ou sans texture

float rnd(float x) { return fract(sin(x) * 43758.5453123); }
float grayFromID(int id) {
    float f = float(id);
    float g = rnd(f * 0.61803398875 + 1.0);
    g = 0.2 + 0.7 * g;
    return g;
}

void main()
{
    if (u_UseTexture)
    {
        // texture
        FragColor = texture(texture1, TexCoord);
        // vec4 texColor = texture(texture1, TexCoord);
        // FragColor = texColor * baseColor;
    }
    else
    {
        // mode sans texture
        int pid = gl_PrimitiveID;
        float gray = grayFromID(pid);
        FragColor = vec4(vec3(gray), 1.0);
    }
}