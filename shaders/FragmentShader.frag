#version 410 core

// Input variable for the fragment length
in float fragmentLength;

// Output variable for the fragment color
out vec4 fragmentColor;
void main ()
{
	// Calculate the color based on the fragment length
	fragmentColor = vec4 (0.0, 0.0, fragmentLength, 1.0);
}