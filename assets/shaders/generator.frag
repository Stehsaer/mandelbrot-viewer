#version 420

in vec2 texCoord;
out vec4 color;

uniform sampler1D palette;

uniform int max_iter;
uniform int palette_cycle;
uniform dvec2 center;
uniform dvec2 size;

void main()
{
	dvec2 z = dvec2(0.0lf, 0.0lf);
	dvec2 c = dvec2(double(texCoord.x), double(texCoord.y)) * size / 2.0 + center;

	int i;
	for (i = 0; i < max_iter; i++) {
	    double zx = z.x;
		z.x = z.x * z.x - z.y*z.y + c.x;
		z.y = 2 * zx * z.y + c.y;
		if (z.x*z.x + z.y*z.y >= 4.0)
		{
			break;
		}
	}

	if(i == max_iter)
		color = vec4(0.0);
	else
		color = vec4(texture(palette, (i % palette_cycle) / float(palette_cycle)).xyz, 1.0);
}