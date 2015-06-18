#version 410

layout(location=0) in vec4 position;
layout(location=1) in vec2 texcoord;
layout(location=2) in vec3 normal;

uniform mat4 ProjectionView;

out vec2 frag_texcoord;
out vec3 vNormal;
out vec4 vPosition;

uniform sampler2D perlin_texture;
uniform float iceLevel;

void main()
{
	vec4 pos = position;

	float height = texture(perlin_texture, texcoord).r;
	if(height <= iceLevel) 
	{
		height = iceLevel;
	}

	pos.y += height * 30;
	frag_texcoord = texcoord;
	vNormal = normal.xyz;
	vPosition = position;
	gl_Position = ProjectionView * pos;
}