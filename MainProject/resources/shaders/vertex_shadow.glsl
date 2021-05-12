#version 150

#define NO_POINT_LIGHTS 1 
#define NO_SPOT_LIGHTS 1 

in vec4 Position;
in vec4 Colour;
in vec4 Normal;
in vec2 Tex1;

uniform mat4 ShadowProjectionView;
uniform mat4 Model;

void main()
{
	gl_Position = (ShadowProjectionView * Model * Position);
}