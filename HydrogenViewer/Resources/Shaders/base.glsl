#type:Vertex
#version 330 core

layout(location=0) in vec3 Position;
layout(location=1) in vec3 Color;
out vec3 InColor;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection; 

void main()
{
	InColor = Color;
	gl_Position = Projection * View * Model* vec4(Position, 1.0f);
}

#type:Fragment
#version 330 core
in vec3 InColor;
out vec4 FragColor;
void main ()
{
	FragColor = vec4(InColor, 1.0f);
}
