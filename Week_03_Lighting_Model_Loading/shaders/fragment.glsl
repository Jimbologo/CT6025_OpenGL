#version 330

in vec4 vPosition;
in vec4 vColour;
in vec4 vNormal;
in vec2 vTexCoord;

out vec4 fragColour;

uniform vec4 CameraPosition;
uniform vec4 LightDirection;
uniform vec4 LightColour;

uniform vec4 matAmbient;
uniform vec4 matDiffuse;
uniform vec4 matSpecular;

uniform sampler2D DiffuseTexture;

const vec4 ambientLight = vec4(0.4f, 0.4f, 0.4f, 1.f);
const vec4 LightSpecular = vec4(1.0, 1.0, 1.0, 1.0);

void main()
{
	vec4 albedo = texture(DiffuseTexture, vTexCoord);
	vec4 ambient = ambientLight * matAmbient;
	//diffuse Light
	//diffuse * diffuse light colour * n.1
	
	vec4 diffuse = matDiffuse * LightColour * max( 0.f, dot(vNormal, -LightDirection));
	vec4 reflectionVec = reflect(LightDirection, vNormal);
	vec4 eyeDir = normalize(CameraPosition - vPosition);
	vec4 specular = matSpecular * LightSpecular * max(0.f, pow(dot(eyeDir, reflectionVec), 225.0));
	fragColour = albedo * (ambientLight + diffuse + specular);
}