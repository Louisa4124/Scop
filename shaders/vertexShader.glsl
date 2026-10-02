#version 330 core
layout (location = 0) in vec3 aPos; // La variable position a l'attribut de position 0
  
out vec4 vertexColor;             // Nous définirons la couleur dans cette variable
uniform mat4 u_Model;

void main()
{
    gl_Position = u_Model * vec4(aPos, 1.0); // un vec3 est utilisé pour construire un vec4
    vertexColor = vec4(1.0, 0.5, 0.2, 1.0); // Couleur rouge foncé
}