#version 150

in vec2 UV;
	
out vec4 FragColor;

uniform sampler2D colorTexture;
uniform sampler2D normalTexture;
uniform sampler2D worldTexture;

uniform vec4 cameraPosition;
uniform vec4 lightDirection;

void main()
{
	//vec2 UV = gl_FragCoord.xy / screenSize;
	vec4 DiffuseColor = texture(colorTexture, UV);
	vec3 Normal = normalize(texture(normalTexture, UV).xyz);
	vec3 WorldPosition = texture(worldTexture, UV).xyz;
	vec3 Eye = normalize(cameraPosition.xyz - texture(worldTexture, UV).xyz);
	vec3 Light = normalize(lightDirection.xyz);

	vec3 albedo = vec3( 1.0, 0.4, 0.4 );
	vec3 linearColour = albedo * (Normal + DiffuseColor.xyz + Light + Eye + WorldPosition); 
	//gamma correction
	vec3 gammaCorrected = pow(linearColour, vec3(1.0/2.2));
	FragColor = vec4(gammaCorrected, 1.0f);

}
