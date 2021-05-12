#version 330
//inputs from previous stages of the pipline
in vec4 vColour;
//output from fragment stage
out vec4 fragColour;

//use a uniform to get cpu side data into the shader
uniform float u_time;
uniform vec2 u_resolution;
const float PI = 3.14159265;

float Circle(in vec2 a_uv, in float a_radius)
{
	vec2 pos = vec2(0.5) - a_uv;
	a_radius *= 0.75;
	return 1.0 - smoothstep( a_radius - (a_radius * 0.05), a_radius + (a_radius * 0.05), dot(pos, pos) * PI);
}

void main()
{
	vec2 uv = gl_FragCoord.xy / u_resolution;
	uv *= 4.0;
	uv = fract(uv);
	float s = sin(u_time);
	vec4 tColour = vColour;
	if(s < 0)
	{
		tColour = vColour.gbra;
	}

	fragColour = mix( vec4(0.f,0.f,0.f,1.f), vColour, Circle(uv, abs(sin(u_time + (gl_FragCoord.x * -0.1)))));
}