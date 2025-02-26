#version 410 core
layout(location = 0) in vec3 position;
layout(location = 1) in float length;
layout(location = 2) in vec3 normal;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 model;
mat4 mvp = proj * view * model;
uniform mat4 tv;
out float fragmentLength;
out float fragnormal;
void main ()
{
	fragmentLength = length;
	//fragnormal = normal.y;
	gl_Position = mvp * vec4 (position, 1.0);
}