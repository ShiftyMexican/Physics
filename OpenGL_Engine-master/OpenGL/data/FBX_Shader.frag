#version 410

in vec3 frag_normal;
in vec2 frag_texcoord;
in vec4 frag_position;
in vec4 frag_tangent;
in vec4 frag_bitangent;
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
	mat3 TBN = mat3( normalize(frag_tangent), normalize(frag_bitangent), normalize(frag_normal));

	vec3 N = texture(normal, frag_texcoord).xyz * 2 - 1;
	
	//vec3 lightDir = normalize(frag_position.xyz - LightPos);
	
	float d = max(0.0, dot(normalize(frag_normal), normalize(LightDir) ) );	
	
	vec3 E = normalize( CameraPos - frag_position.xyz );
	
	vec3 R = reflect( -LightDir, frag_normal.xyz );
	
	float s = max( 0, dot( E, R ) );
	
	s = pow( s, SpecPow );	
	
	FragColor = texture(diffuse, frag_texcoord) * vec4(LightColour * d + LightColour * s, 1); 
	
};
