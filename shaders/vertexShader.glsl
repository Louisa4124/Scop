#version 330 core
layout (location = 0) in vec3 aPos; // La variable position a l'attribut de position 0
  
out vec4 FragColor;

uniform mat4 u_Model;
uniform vec4 ourColor; 

void main()
{
    gl_Position = u_Model * vec4(aPos, 1.0);
    FragColor = ourColor;
}