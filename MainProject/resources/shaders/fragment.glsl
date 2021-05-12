#version 150
#extension GL_EXT_gpu_shader4 : require

#define NO_POINT_LIGHTS 1 
#define NO_SPOT_LIGHTS 1 

in Vertex
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

out vec4 FragColor;

struct DirLight
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dirLight;
uniform sampler2DShadow dirShadowTexture;

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float range;
	float cutoff;
};  
uniform PointLight pointLights[NO_POINT_LIGHTS];
uniform sampler2DShadow pointShadowTextures[NO_POINT_LIGHTS];

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float cutoff;
	float outerCutOff;
	float range;
	float maxDistance;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
    float linear;
    float quadratic;
};
uniform SpotLight spotLights[NO_SPOT_LIGHTS];

uniform sampler2DShadow spotShadowTextures[NO_SPOT_LIGHTS];

uniform sampler2D DiffuseTexture;

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

vec3 CalculateDirLighting(DirLight light);
float calculateShadows();
vec3 CalcPointLight(PointLight light);
vec3 CalcSpotLight(SpotLight light);

void main()
{
	//Calc albedo
	vec3 albedo = texture(DiffuseTexture, vertex.UV).xyz;

	//Calc Directional lighting
	vec3 outputLighting = CalculateDirLighting(dirLight);

	//Calc Point lighting
	for(int i = 0; i < NO_POINT_LIGHTS; i++)
	{
        outputLighting += CalcPointLight(pointLights[i]);  
	}

	//Calc Spot lighting
	for(int i = 0; i < NO_SPOT_LIGHTS; i++)
	{
        outputLighting += CalcSpotLight(spotLights[i]);  
	}

	vec3 linearColour = albedo * dirLight.ambient + albedo * (outputLighting) * calculateShadows();

	//gama correction
	vec3 gammaCorrected = pow(linearColour, vec3(1.0/2.2));

	FragColor = vec4(gammaCorrected, 1.0f);
}

vec3 CalculateDirLighting(DirLight light)
{
	vec3 ambient = light.ambient;
	//Calculate diffuse component
	vec3 diffuse = max(0, dot(vertex.Normal.xyz, vertex.LightDir)) * light.diffuse;
	//Calculate specular component
	vec3 specular = pow(max(0, dot(vertex.HalfDir, vertex.Normal.xyz)), specularTerm) * light.specular;
	return (ambient + diffuse + specular);
}

float calculateShadows()
{
	//calculate shadow by testing depth & Poisen Sampling
	float bias = max(0.005 * (1.0 - dot(vec3(vertex.Normal.x, vertex.Normal.y, vertex.Normal.z), vertex.LightDir)), 0.005);  
	float visibility = 1;
	float diskRadius = 0.0007;


	for (int i=0;i<samples;i++){
		if ( texture(dirShadowTexture, vertex.DirShadowCoord.xyz + sampleOffsetDirections[i] * diskRadius )  <  vertex.DirShadowCoord.z-bias ){
			visibility-=0.05;
		}
	}

	for (int l=0;l<NO_POINT_LIGHTS;l++){
		vec3 lightDir = normalize(pointLights[l].position - vertex.Pos.xyz);
		bias = max(0.005 * (1.0 - dot(vec3(vertex.Normal.x, vertex.Normal.y, vertex.Normal.z), lightDir)), 0.005);  
		diskRadius = 0.0007;
		for (int i=0;i<samples;i++){
			if ( shadow2D(pointShadowTextures[l], vertex.PointShadowCoords[l].xyz + sampleOffsetDirections[i] * diskRadius ).r  <  vertex.PointShadowCoords[l].z-bias ){
				visibility-=0.05;
			}
		}	
	}
	
	for (int l=0;l<NO_SPOT_LIGHTS;l++){
		vec3 lightDir = normalize(spotLights[l].position - vertex.Pos.xyz);
		float theta = dot(lightDir, normalize(-spotLights[l].direction));
		float epsilon   = spotLights[l].cutoff - (spotLights[l].outerCutOff - 0.000f);
		float intensity = clamp(((spotLights[l].outerCutOff  - 0.001f)- theta) / epsilon, 0.0, 1.0);  
		float distance = length(spotLights[l].position - vertex.Pos.xyz);

		

		if(theta > spotLights[l].cutoff || distance > (spotLights[l].maxDistance + 0.25))
		{       
			float d = max(distance - (spotLights[l].range), 0);

			// calculate basic attenuation
			float attenuation = 1 / (distance*distance);

			bias = max(0.005 * (1.0 - dot(vec3(vertex.Normal.x, vertex.Normal.y, vertex.Normal.z), spotLights[l].direction)), 0.005);  
			diskRadius = 0.0007;
			for (int i=0;i<samples;i++){
				if ( texture(spotShadowTextures[l], (vertex.SpotShadowCoords[l].xyz + sampleOffsetDirections[i] * diskRadius))  <  (vertex.SpotShadowCoords[l].z-bias)  ){
					visibility-=(0.05 * intensity);
				}
			}
		}

		
	}

	visibility = clamp(visibility, 0 , 1);

	return visibility;
}

vec3 CalcPointLight(PointLight light)
{
    vec3 lightDir = normalize(light.position - vertex.Pos.xyz);
    // diffuse shading
    float diff = max(dot(vertex.Normal.xyz, vertex.LightDir), 0.0);
    // specular shading
    vec3 specular = pow(max(0, dot(vertex.HalfDir, vertex.Normal.xyz)), specularTerm) * light.specular;

    // combine results
    vec3 ambient = light.ambient;
    vec3 diffuse = max(0, dot(vertex.Normal.xyz, vertex.LightDir)) * light.diffuse;

	float distance = length(light.position - vertex.Pos.xyz);
	float d = max(distance - light.range, 0);

	// calculate basic attenuation
    float denom = d/light.range + 1;
    float attenuation = 1 / (denom*denom);

	attenuation = (attenuation - light.cutoff) / (1 - light.cutoff);
    attenuation = max(attenuation, 0);

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

vec3 CalcSpotLight(SpotLight light)
{
    vec3 lightDir = normalize(light.position - vertex.Pos.xyz);
    // diffuse shading
    float diff = max(dot(vertex.Normal.xyz, vertex.LightDir), 0.0);
    // specular shading
    vec3 specular = pow(max(0, dot(vertex.HalfDir, vertex.Normal.xyz)), specularTerm) * light.specular;

    // combine results
    vec3 ambient = light.ambient;
    vec3 diffuse = max(0, dot(vertex.Normal.xyz, vertex.LightDir)) * light.diffuse;

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon   = light.cutoff - light.outerCutOff;
	float intensity = clamp((light.outerCutOff - theta) / epsilon, 0.0, 1.0);  
    float distance = length(light.position - vertex.Pos.xyz);

	ambient  *= intensity;
	diffuse  *= intensity;
	specular *= intensity;

	if(theta < light.cutoff || distance > (light.maxDistance + 0.25))
	{       
		float d = max(distance - (light.range), 0);

		// calculate basic attenuation
		float attenuation = 1 / (distance*distance);

		ambient  *= attenuation;
		diffuse  *= attenuation;
		specular *= attenuation;
	}

	


	
    return (ambient + diffuse + specular);
} 