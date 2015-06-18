#version 410

in vec3 TexCoords;

out vec4 colour;

uniform samplerCube Skybox;

void main()
{
	colour = texture(Skybox, TexCoords);
}