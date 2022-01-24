#version 460 core

in vec2 vTexCoords;

out vec4 fragColor;

uniform sampler2D texture_diffuse0;

void main()
{
	fragColor = vec4(texture(texture_diffuse0, vTexCoords).rgb, 1.f);
}
