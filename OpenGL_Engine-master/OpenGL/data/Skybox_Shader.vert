#version 410

layout (location=0) in vec3 position;

out vec3 TexCoords;

uniform mat4 ProjectionView;

uniform mat4 Projection;
uniform mat4 View;
void main()
{
	vec4 pos = Projection * mat4(mat3(View)) * vec4(position, 1.0f);
	gl_Position = pos.xyzw;
	TexCoords = position;
}