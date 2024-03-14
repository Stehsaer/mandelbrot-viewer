#version 420

in vec2 texCoord;
out vec4 color;

uniform sampler1D palette;

void main()
{
	color = vec4(texture(palette, texCoord.x).xyz, 1.0);
	//color = vec4(1.0);
}