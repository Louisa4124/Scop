#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
flat in vec3 flatPos;

uniform sampler2D texture1;
uniform float u_Transition; 
uniform int u_ColorMode;    

float rnd(float x) { return fract(sin(x) * 43758.5453123); }
float grayFromID(int id) {
    float f = float(id);
    float g = rnd(f * 0.61803398875 + 1.0);
    return 0.2 + 0.7 * g;
}

vec3 pastelRainbowPalette(float t) {
    vec3 a = vec3(0.5f, 0.5f, 0.5f);
    vec3 b = vec3(0.5f, 0.5f, 0.5f);
    vec3 c = vec3(1.0f, 1.0f, 1.0f);
    vec3 d = vec3(0.0f, 0.33f, 0.67f);
    vec3 rgb = a + b * cos(6.28318f * (c * t + d));
    
    return mix(rgb, vec3(1.0f), 0.45f); 
}

void main()
{
    int pid = gl_PrimitiveID;
    vec4 colorWithoutTexture;

    if (u_ColorMode == 0)
    {
        float gray = grayFromID(pid);
        colorWithoutTexture = vec4(vec3(gray), 1.0f);
    }
    else if (u_ColorMode == 1)
    {
        float r = clamp(flatPos.y * 0.5f + 0.5f, 0.0f, 1.0f);
        float g = clamp(flatPos.x * 0.5f + 0.5f, 0.0f, 1.0f);
        float b = clamp(flatPos.z * 0.5f + 0.5f, 0.0f, 1.0f);
        colorWithoutTexture = vec4(r, g, b, 1.0f);
    }
    else if (u_ColorMode == 2)
    {
        float factor = clamp(flatPos.y * 0.4f + 0.5f, 0.0f, 1.0f);
        vec3 colGreen = vec3(0.0f, 1.0f, 0.3f);
        vec3 colPink  = vec3(0.9686f, 0.1451f, 0.5216f); // 247, 37, 133
        colorWithoutTexture = vec4(mix(colPink, colGreen, factor), 1.0f);
    }
    else
    {
        float t = flatPos.y * 1.5f + flatPos.x * 0.5f;
        vec3 rgb = pastelRainbowPalette(t);
        colorWithoutTexture = vec4(rgb, 1.0f);
    }

    vec2 fixedTexCoord = vec2(TexCoord.x, 1.0f - TexCoord.y);
    vec4 colorWithTexture = texture(texture1, fixedTexCoord);

    if (u_Transition >= 0.99f)
        FragColor = colorWithTexture;
    else
        FragColor = mix(colorWithoutTexture, colorWithTexture, u_Transition);
}