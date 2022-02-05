#version 460 core

in vec2 vTexCoords;

out vec4 fragColor;

uniform sampler2D texture_diffuse0;
uniform bool isWireframe = false;

void main()
{
	if(isWireframe)
		fragColor = vec4(0.f,1.f,0.f,1.f);
	else
		fragColor = vec4(texture(texture_diffuse0, vTexCoords).rgb, 1.f);
}
