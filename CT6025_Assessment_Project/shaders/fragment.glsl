#version 150

in vec4 vPosition;
in vec4 vNormal;
in vec4 vColour;
in vec2 vUV;
in vec4 vLightDir;
in vec4 vViewDir;

out vec4 FragColour;

uniform vec4 LightColour = vec4(1.0, 0.85, 0.05, 0.1);
uniform vec4 LightAmbient = vec4(0.4, 0.4, 0.4, 0.0);
uniform vec4 LightSpecular = vec4(0.0, 0.0, 0.0, 0.0);

uniform float specularPower = 1.0;
uniform float normalPower = 1.0;

uniform int useNormalMap = 1;

uniform sampler2D DiffuseTexture;
uniform sampler2D NormalTexture;


void main()
{
	vec4 albedo = texture(DiffuseTexture, vUV);
	vec4 normalTex;
	if(useNormalMap == 1)
	{
		normalTex = texture(NormalTexture, vUV) * 2.0 - 1.0;
	}
	else
	{
		normalTex = vNormal;
	}

	normalTex = normalTex * normalPower;

	vec4 ambient = LightAmbient;

	float Ndl = max(0.0, dot(normalTex, -vLightDir));
	vec4 diffuse = LightColour * Ndl;
	vec4 R = reflect(vLightDir, normalTex);
	float specularTerm = pow(max(0.0, dot(R, vViewDir)), specularPower);
	vec4 specular = LightSpecular * specularTerm;

	FragColour = vec4(albedo.rgb * (albedo.rgb + diffuse.rgb + specular.rgb), 1.0);


}