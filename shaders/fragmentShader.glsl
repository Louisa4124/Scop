#version 330 core
out vec4 FragColor;

float rnd(float x) { return fract(sin(x) * 43758.5453123); }
float grayFromID(int id) {
    float f = float(id);
    float g = rnd(f * 0.61803398875 + 1.0);
    g = 0.2 + 0.7 * g;
    return g;
}

void main()
{
    int pid = gl_PrimitiveID;
    float gray = grayFromID(pid);
    FragColor = vec4(vec3(gray), 1.0);
}