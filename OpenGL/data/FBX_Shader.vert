#version 410

layout(location=0) in vec4 Position;
layout(location=1) in vec3 Normal;
layout(location=2) in vec4 Tangent;
layout(location=3) in vec2 TexCoord;
layout(location=4) in vec4 Weight;
layout(location=5) in vec4 Indicies;

out vec2 frag_texcoord;
out vec4 frag_position;
out vec4 frag_tangent;
out vec4 frag_bitangent;
out vec3 frag_normal;

uniform mat4 ProjectionView;

const int MAX_BONES = 128;
uniform mat4 bones[MAX_BONES];

void main()
{
	frag_position	= Position;
	frag_position *= 0.1;
	frag_normal		= Normal;
	frag_tangent	= Tangent;
	frag_bitangent	= vec4(cross(Normal.xyz, Tangent.xyz), 0);
	frag_texcoord	= TexCoord;

	ivec4 index = ivec4(Indicies);

	vec4 P = vec4(0, 0, 0, 0); 
	P += bones[ index.x ] * frag_position * Weight.x;
	P += bones[ index.y ] * frag_position * Weight.y;
	P += bones[ index.z ] *	frag_position * Weight.z;
	P += bones[ index.w ] *	frag_position * Weight.w;
	P.w = 1;
	P.x += 100;
	gl_Position = ProjectionView * P;
}