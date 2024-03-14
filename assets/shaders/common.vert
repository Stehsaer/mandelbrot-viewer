#version 420

layout (location = 0) in vec2 coord;
out vec2 texCoord;

void main()
{
	gl_Position = vec4(coord, 0.0, 1.0);
	texCoord = coord;
}