#version 400                                                                        
                                                                                    
in vec3 Position_FS_in;
in vec3 Normal_FS_in;
in vec3 WorldPos_FS_in;
in vec2 TexCoord_FS_in;


out vec4 FragColor; 

uniform mat4 gWorld;

uniform sampler2D grass;
uniform sampler2D sand;
uniform sampler2D stone;
uniform sampler2D snow;  
uniform sampler2D water;

uniform float time_melting;

uniform int fog_mode;
uniform int sandbox_mode;

uniform int NumLights;

struct DirectionalLight 
{
	vec3 Color;
	float AmbientIntensity;
	vec3 Direction;
	float DiffuseIntensity;
};

struct PointLight
{
	vec3 Color;
	float AmbientIntensity;
	vec3 Position;
	float DiffuseIntensity;

	float Constant;
	float Linear;
	float Exp;
};

uniform DirectionalLight gDirectionalLight;
uniform PointLight gPointLight[3];

uniform vec3 gEyeWorldPos;
uniform float gMatSpecularIntensity;
uniform float gSpecularPower;


vec4 CalculatePointLight(PointLight Light)
{
	vec3 Normal = normalize(Normal_FS_in);
	vec4 AmbientColor = vec4(Light.Color, 1.0) * Light.AmbientIntensity;

	vec3 WorldLightPosition = vec3(gWorld * vec4(Light.Position, 1.0) ).xyz;
    
	vec3 LightDirection = WorldPos_FS_in - WorldLightPosition;
	
	float distance = length(LightDirection);

	float attenuation = Light.Constant + Light.Linear * distance + Light.Exp * distance * distance;
	
	LightDirection = normalize(LightDirection);

	float DiffuseFactor = dot(Normal, LightDirection);

	vec4 DiffuseColor = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 SpecularColor = vec4(0.0, 0.0, 0.0, 0.0);                                                          
                                                                                    
    if (DiffuseFactor > 0)
	{                                                        
        DiffuseColor = vec4(Light.Color, 1.0f) * Light.DiffuseIntensity *  DiffuseFactor;
		vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos_FS_in);
		vec3 LightReflect = normalize(reflect(LightDirection, Normal));
		float SpecularFactor = dot(VertexToEye, LightReflect);
		SpecularFactor = pow(SpecularFactor, gSpecularPower);
		
		if (SpecularFactor > 0)
		{
			if (Light.DiffuseIntensity > 0.0)
			{   
				SpecularColor = vec4(Light.Color, 1.0) * gMatSpecularIntensity * SpecularFactor;   
			}                     
		}   
	}	


	vec4 PointColor = (AmbientColor + DiffuseColor + SpecularColor);
	PointColor /= attenuation;

	return PointColor;
}

vec4 CalculateDirectionalLight(DirectionalLight Light)
{
	vec3 Normal = normalize(Normal_FS_in);
	vec4 AmbientColor = vec4(Light.Color, 1.0) * Light.AmbientIntensity;

    
	vec3 LightDirection = -Light.Direction;
	

	float DiffuseFactor = dot(Normal, LightDirection);

	vec4 DiffuseColor = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 SpecularColor = vec4(0.0, 0.0, 0.0, 0.0);                                                          
                                                                                    
    if (DiffuseFactor > 0)
	{                                                        
        DiffuseColor = vec4(Light.Color, 1.0f) * Light.DiffuseIntensity *  DiffuseFactor;
		vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos_FS_in);
		vec3 LightReflect = normalize(reflect(LightDirection, Normal));
		float SpecularFactor = dot(VertexToEye, LightReflect);
		SpecularFactor = pow(SpecularFactor, gSpecularPower);
		
		if (SpecularFactor > 0)
		{
			if (Light.DiffuseIntensity > 0.0)
			{
				SpecularColor = vec4(Light.Color, 1.0) * gMatSpecularIntensity * SpecularFactor;   
			}                     
		}   
	}	


	vec4 DirectionalColor = (AmbientColor + DiffuseColor + SpecularColor);

	return DirectionalColor;
}


void main()                                                                         
{

	vec4 DirectionalColor = CalculateDirectionalLight(gDirectionalLight);

	vec4 PointColorTotal = vec4(0.0, 0.0, 0.0, 0.0);
	
	for (int i = 0; i < NumLights; i++)
	{
		PointColorTotal += CalculatePointLight(gPointLight[i]);
		
	}


	vec4 TotalLight = DirectionalColor + PointColorTotal;

		float waterWeight = clamp (1.0 - abs(Position_FS_in.z - 0.0) / 40.0, 0.0, 1.0);
		float sandWeight =  clamp (1.0 - abs(Position_FS_in.z - 60.0) / 40.0, 0.0, 1.0); 
		float landWeight =  clamp (1.0 - abs(Position_FS_in.z - 120.0) / 50.0, 0.0, 1.0);
		float rockWeight =  clamp (1.0 - abs(Position_FS_in.z - 170.0) / 30.0, 0.0, 1.0);
		float snowWeight =  clamp(1.0 -  abs(Position_FS_in.z - 255.0) / 100.0, 0.0, 1.0);

		float total = snowWeight + rockWeight + landWeight + sandWeight + waterWeight ;

		snowWeight  /= total;
		rockWeight  /= total;
		landWeight  /= total;
		sandWeight  /= total;
		waterWeight /= total;
			
		FragColor =  texture2D(snow,  TexCoord_FS_in.xy)   *  snowWeight;
		FragColor += texture2D(stone, TexCoord_FS_in.xy)   *  rockWeight;
		FragColor += texture2D(grass, TexCoord_FS_in.xy)   *  landWeight;
		FragColor += texture2D(sand,  TexCoord_FS_in.xy)   *  sandWeight;
		FragColor += texture2D(water, TexCoord_FS_in.xy)   *  waterWeight;
	
	
	FragColor *=  TotalLight;                                     

	if (fog_mode == 1)
	{
		float d = length(gEyeWorldPos - WorldPos_FS_in);
		float fog_max = 450;
		float fog_min = 120;
		float fogFactor = (fog_max - d) / (fog_max - fog_min);
		fogFactor = clamp(fogFactor, 0.0, 1.0);
		vec4 FogColor = vec4(0.6, 0.6, 0.6, 0.6);
	
		vec4 TerrainColor = FragColor;	                                   

		FragColor = mix(FogColor, TerrainColor, fogFactor);
	}        
}                                        
