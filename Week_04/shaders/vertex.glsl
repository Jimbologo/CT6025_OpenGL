#version 150

in vec4 Position;
in vec4 Colour;
in vec4 Normal;
in vec4 Tangent;
in vec4 BiTangent;
in vec2 Tex1;

out vec4 vPosition;
out vec4 vNormal;
out vec4 vColour;
out vec2 vUV;
out vec4 vLightDir;
out vec4 vViewDir;
//out mat3 vTBN;

uniform mat4 ProjectionView;
uniform mat4 ModelMatrix;
uniform mat4 NormalMatrix;
uniform mat4 ViewMatrix;

uniform vec4 CameraPosition;
uniform vec4 LightDirection;

uniform int useNormalMap = 1;

void main()
{
	vPosition = ViewMatrix * ModelMatrix * Position;
	vNormal = NormalMatrix * Normal;

	vec4 N = NormalMatrix * Normal;
	vec4 T = NormalMatrix * Tangent;
	vec4 B = NormalMatrix * BiTangent;

	mat3 TBN = transpose(mat3(T.xyz,B.xyz, N.xyz));
	vec4 E = normalize((ViewMatrix * ModelMatrix * CameraPosition) - vPosition);
	if(useNormalMap == 1)
	{
		vViewDir = vec4(TBN * E.xyz, 0.0);
		vLightDir = vec4(TBN * (ViewMatrix * ModelMatrix * LightDirection).xyz, 0.0);
	}
	else
	{
		vViewDir = vec4(E.xyz, 0.0);
		vLightDir = vec4((ViewMatrix * ModelMatrix * LightDirection).xyz, 0.0);
	}
	vColour = Colour;
	vUV = Tex1;
	gl_Position = ProjectionView * ModelMatrix * Position;
}