#version 410

in vec4 frag_normal;
in vec2 frag_texcoord;
in vec4 frag_position;
in vec4 frag_tangent;
in vec4 frag_bitangent;
in vec2 vTexCoord;

in vec3 vNormal;
in vec4 vPosition;
in vec3 vTangent;
in vec3 vBiTangent;

out vec4 FragColor;	

uniform sampler2D diffuse;
uniform sampler2D normal;	

uniform vec3 LightDir;
uniform vec3 LightPos;
uniform vec3 LightColour;
uniform vec3 CameraPos;
uniform float SpecPow;

void main() 
{
	mat3 TBN = mat3( normalize(vTangent), normalize(vBiTangent), normalize(vNormal));

	vec3 N = texture(normal, vTexCoord).xyz * 2 - 1;
	
	float d = max(0.0, dot(normalize(vNormal), normalize(LightDir) ) );	
	
	vec3 E = normalize( CameraPos - vPosition.xyz );
	
	vec3 R = reflect( -LightDir, vNormal.xyz );

	float s = max( 0, dot( E, R ) );
	
	s = pow( s, SpecPow );

	FragColor = texture(diffuse, vTexCoord) * vec4( LightColour * d + LightColour * s, 1); //texture(diffuse, vTexCoord) * vec4( LightColour * d + LightColour * s, 1);//vec4(0.2,0.2,0.2,1); 

}