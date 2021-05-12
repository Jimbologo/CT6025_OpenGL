#version 150

#define NO_POINT_LIGHTS 1 
#define NO_SPOT_LIGHTS 1 

in vec4 Position;
in vec4 Colour;
in vec4 Normal;
in vec2 Tex1;

out Vertex
{
	vec4 Pos;
	vec4 Normal;
	vec4 Colour;
	vec2 UV;
	vec3 HalfDir;
	vec3 LightDir;
	vec4 DirShadowCoord;
	vec4 PointShadowCoords[NO_POINT_LIGHTS];
	vec4 SpotShadowCoords[NO_SPOT_LIGHTS];
}vertex;



uniform mat4 ProjectionView;
uniform mat4 Model;
uniform mat4 NormalMatrix;
uniform mat4 ViewMatrix;
uniform mat4 DirShadowProjectionView;
uniform mat4 PointShadowProjectionView[NO_POINT_LIGHTS];
uniform mat4 SpotShadowProjectionView[NO_SPOT_LIGHTS];

uniform vec4 CameraPosition;
uniform vec4 LightDirection;

const mat4 SHADOW_BIAS_MATRIX = mat4(
					0.5, 0.0, 0.0, 0.0,
					0.0, 0.5, 0.0, 0.0,
					0.0, 0.0, 0.5, 0.0,
					0.5, 0.5, 0.5, 1.0
					);

void main()
{
	vertex.Pos = Model * Position;
	vertex.Normal = ViewMatrix * NormalMatrix * Normal;
	vertex.Colour = Colour;
	vertex.UV = Tex1;

	vertex.LightDir = (ViewMatrix * -LightDirection).xyz;

	vec3 eyeDir = (ViewMatrix * normalize(CameraPosition - vertex.Pos)).xyz;
	vertex.HalfDir = normalize(eyeDir + vertex.LightDir);

	vertex.DirShadowCoord = SHADOW_BIAS_MATRIX * DirShadowProjectionView * Position;

	for	(int i=0; i<NO_POINT_LIGHTS; i++)
	{
		vertex.PointShadowCoords[i] = SHADOW_BIAS_MATRIX * PointShadowProjectionView[i] * Position;
	}
	for	(int i=0; i<NO_SPOT_LIGHTS; i++)
	{
		vertex.SpotShadowCoords[i] = SHADOW_BIAS_MATRIX * SpotShadowProjectionView[i] * Position;
	}

	gl_Position = ProjectionView * Model * Position;
}