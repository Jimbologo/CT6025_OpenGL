#version 150


in Vertex
{
	vec4 Pos;
	vec4 Normal;
	vec4 Colour;
	vec2 UV;
	vec3 HalfDir;
	vec3 LightDir;
	vec4 ShadowCoord;
}vertex;

out vec4 FragColor;

uniform sampler2D DiffuseTexture;
uniform sampler2DShadow ShadowTexture;

struct Light{
vec3 ambient;
vec3 diffuse;
vec3 specular;
};

const Light sceneLight = Light(vec3(0.125, 0.125, 0.125), vec3(1.0, 1.0, 1.0), vec3(1.0, 1.0, 1.0));
const float specularTerm = 32.0;
const float closestDepth = 20.0f;
const int samples = 40;
const vec3 sampleOffsetDirections[40] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1),
   vec3( 0.5,  0.5,  0.5), vec3( 0.5, -0.5,  0.5), vec3(-0.5, -0.5,  0.5), vec3(-0.5,  0.5,  0.5), 
   vec3( 0.5,  0.5, -0.5), vec3( 0.5, -0.5, -0.5), vec3(-0.5, -0.5, -0.5), vec3(-0.5,  0.5, -0.5),
   vec3( 0.5,  0.5,  0), vec3( 0.5, -0.5,  0), vec3(-0.5, -0.5,  0), vec3(-0.5,  0.5,  0),
   vec3( 0.5,  0,  0.5), vec3(-0.5,  0,  0.5), vec3( 0.5,  0, -0.5), vec3(-0.5,  0, -0.5),
   vec3( 0,  0.5,  0.5), vec3( 0, -0.5,  0.5), vec3( 0, -0.5, -0.5), vec3( 0,  0.5, -0.5)
);  

void main()
{
	//calculate shadow by testing depth
	float bias = max(0.005 * (1.0 - dot(vec3(vertex.Normal.x, vertex.Normal.y, vertex.Normal.z), vertex.LightDir)), 0.005);  
	float visibility = 1;
	float diskRadius = 0.0007;
	for (int i=0;i<samples;i++){
		if ( texture(ShadowTexture, vertex.ShadowCoord.xyz + sampleOffsetDirections[i] * diskRadius ).r  <  vertex.ShadowCoord.z-bias ){
			visibility-=0.05;
		}
	}

	visibility = clamp(visibility, 0 , 1);
	//visibility = visibility / float(samples * 0.2f); 
	

	vec3 albedo = texture(DiffuseTexture, vertex.UV).xyz;

	//phong ambient colour
	vec3 ambient = sceneLight.ambient;
	//phong diffuse
	vec3 diffuse = max(0, dot(vertex.Normal.xyz, vertex.LightDir))* sceneLight.diffuse;

	//Calculate specular component
	vec3 specular = pow(max(0, dot(vertex.HalfDir, vertex.Normal.xyz)), specularTerm)* sceneLight.specular;

	vec3 linearColour = albedo * ambient + albedo * (diffuse + specular) * visibility;
	//gama correction
	vec3 gammaCorrected = pow(linearColour, vec3(1.0/2.2));
	FragColor = vec4(gammaCorrected, 1.0f);
}