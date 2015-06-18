#version 410

in vec2 frag_texcoord;
in vec3 vNormal;
in vec4 vPosition;

out vec4 out_color;

uniform sampler2D perlin_texture;
uniform sampler2D grass_texture;
uniform sampler2D sand_texture;
uniform sampler2D water_texture;

uniform sampler2D diffuse;
uniform sampler2D normal;	
uniform float iceLevel;

uniform vec3 LightDir;
uniform vec3 LightPos;
uniform vec3 LightColour;
uniform vec3 CameraPos;
uniform float SpecPow;

void main()
{

	// Lighting
	vec3 N = texture(normal, frag_texcoord).xyz * 2 - 1;

	float d = max(0.0, dot(normalize(vNormal), normalize(LightDir) ) );	

	vec3 E = normalize( CameraPos - vPosition.xyz );
	
	vec3 R = reflect( -LightDir, vNormal.xyz );

	float s = max( 0, dot( E, R ) );
	
	s = pow( s, SpecPow );

	float height = texture(perlin_texture, frag_texcoord).r; 
	float blendAmount = 0.6;

	vec4 grass, sand, water;

	grass = texture2D(grass_texture, vec2(frag_texcoord.x * 4, frag_texcoord.y * 4)); // This is now snow
	sand = texture2D(sand_texture, vec2(frag_texcoord.x * 10, frag_texcoord.y * 10)); // This is the Grass now
	water = texture2D(water_texture, vec2(frag_texcoord.x * 5, frag_texcoord.y * 5)); // Ice now

	//out_color = texture(perlin_texture, frag_texcoord).rrrr;
	if(height > 0.7) 
	{
		out_color = grass;
	}
	else if(height > 0.58 && height < 0.7)
	{
		out_color = mix(grass, sand, 0.25);
	}
	else if(height >= iceLevel && height <=0.58 || height >= 0.52 && height <= 0.58)
	{
		out_color = mix(sand, water, 0.7);
	}
	if(height <= iceLevel)
	{
		out_color = water;
	}
	else if(height >= iceLevel && height <= iceLevel + 0.08)
	{
		out_color = mix(sand, water, 0.7);
	}
	else if(height >= iceLevel + 0.08 && height <= iceLevel + 0.16)
	{
		out_color = mix(grass, sand, 0.25);
	}

	// Final Colour
	out_color.a = 1;
	out_color = out_color * vec4( LightColour * d + LightColour * s, 1) * vec4(height, height, height, 1);
}