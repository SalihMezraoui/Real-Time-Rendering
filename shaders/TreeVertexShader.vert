#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in float length;
layout(location = 2) in vec3 normal;

out Vertex
{
	vec3 vposition;
	float vlength;
	vec3 vnormal;
} vertex;

void main()
{
	vertex.vposition = position;
	vertex.vlength = length;
	vertex.vnormal = normal;

	// Set gl_Position for transform feedback
	gl_Position = vec4(position, 1.0);
}
