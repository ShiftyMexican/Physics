#version 410

layout(location=0) in vec4 Position;
layout(location=1) in vec2 TexCoord;
layout(location=2) in vec4 Normal;
layout(location=3) in vec4 Tangent;

out vec3 vNormal;
out vec4 vPosition;
out vec2 vTexCoord;
out vec3 vTangent;
out vec3 vBiTangent;

uniform mat4 ProjectionView;

void main() 
{
	vNormal = Normal.xyz;
	vTangent = Tangent.xyz;
	vPosition = Position;
	vTexCoord = TexCoord; 
	vBiTangent = cross(vNormal, vTangent);
	gl_Position = ProjectionView * Position;
};