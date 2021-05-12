#version 150

// params from vertex
in vec2 texCoord;
in vec3 vNormal;
in vec4 wPos;

// 3 out vars for the textureSamples
out vec4 Diffuse;
out vec4 Normal;
out vec4 World;

void main()
{
	Diffuse = vec4(1.0,0.4,0.4,1.0);
	Normal = vec4(vNormal, 0);
	World = wPos;
}
