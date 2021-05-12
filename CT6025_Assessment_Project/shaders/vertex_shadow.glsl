#version 150

in vec4 Position;
in vec4 Colour;
in vec4 Normal;
in vec2 Tex1;

uniform mat4 ShadowProjectionView;
uniform mat4 ModelMatrix;

void main()
{
	gl_Position = ShadowProjectionView * ModelMatrix * Position;
}