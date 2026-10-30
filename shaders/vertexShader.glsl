#version 330 core
layout (location = 0) in vec3 aPos; // La variable position a l'attribut de position 0
layout (location = 1) in vec2 aTexCoord; // Coordonnées de texture (u, v)

out vec2 TexCoord; 

uniform mat4 u_Model;

void main()
{
    gl_Position = u_Model * vec4(aPos, 1.0);
    TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);
}