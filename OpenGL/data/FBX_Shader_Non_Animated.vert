#version 410

layout(location=0) in vec4 Position;
layout(location=1) in vec2 TexCoord;
layout(location=2) in vec4 Normal;
layout(location=3) in vec4 Tangent;

out vec2 frag_texcoord;
out vec4 frag_position;
out vec4 frag_tangent;
out vec4 frag_bitangent;
out vec4 frag_normal;

uniform mat4 ProjectionView;

void main()
{
	frag_position	= Position;
	frag_normal		= Normal;
	frag_tangent	= Tangent;
	frag_bitangent	= vec4(cross(Normal.xyz, Tangent.xyz), 0);
	frag_texcoord	= TexCoord;

	gl_Position = ProjectionView * Position;
}