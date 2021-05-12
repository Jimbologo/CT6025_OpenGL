#version 150

in vec4 Position;
in vec4 Colour;
in vec4 Normal;
in vec2 Tex1;

out vec2 texCoord;
out vec3 vNormal;
out vec4 wPos;

uniform mat4 ProjectionView;
uniform mat4 Model;
uniform mat4 NormalMatrix;
uniform mat4 ViewMatrix;

void main() 
{ 
	wPos = Model * Position;
	vNormal = (ViewMatrix * NormalMatrix * Normal).xyz;
	texCoord = Tex1;
}