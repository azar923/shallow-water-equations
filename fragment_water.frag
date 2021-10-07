#version 400                                                                        
                                                                                    
                                                            
                         
in vec3 Position_FS_in;
in float Bottom_FS_in;
in vec3 Normal_FS_in;
in vec3 WorldPos_FS_in;
in vec2 TexCoord_FS_in;
in float Lava_FS_in;
in vec4 ProjCoords;
out vec4 FragColor; 

uniform samplerCube skybox;

uniform int NumLights;

uniform mat4 gWorld;

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

uniform float AlphaFactor;
uniform float ReflectionFactor;

uniform sampler2D reflection;
uniform sampler2D refraction;

vec4 CalculatePointLight(PointLight Light)
{
	vec3 Normal = normalize(Normal_FS_in);

	vec3 WorldLightPosition = vec3(gWorld * vec4(Light.Position, 1.0) ).xyz;
    
	vec3 LightDirection = WorldPos_FS_in - WorldLightPosition;
	
	float distance = length(LightDirection);

	float attenuation = Light.Constant + Light.Linear * distance + Light.Exp * distance * distance;
	
	LightDirection = normalize(LightDirection);

	float DiffuseFactor = dot(Normal, LightDirection);

	vec4 AmbientColor = Light.AmbientIntensity * vec4(Light.Color, 1.0);

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
				SpecularColor = vec4(Light.Color, 1.0) * gMatSpecularIntensity * SpecularFactor; 			                          
		}   
	}	

	vec4 PointColor = (AmbientColor + DiffuseColor + SpecularColor );
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
	vec3 Normal = normalize(Normal_FS_in);

	vec4 PointColorTotal = vec4(0.0, 0.0, 0.0, 0.0);
	
	for (int i = 0; i < NumLights; i++)
	{
		PointColorTotal += CalculatePointLight(gPointLight[i]);
	}


	vec4 TotalLight = DirectionalColor + PointColorTotal;     

	vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos_FS_in);
	vec3 ReflectDir = reflect(VertexToEye, Normal);
	vec4 CubeMapColor = texture(skybox, ReflectDir);

	vec2 vDistort = 0.1 * Normal.xz ;

	 vec4 ReflectionColor = vec4(texture2D(reflection, TexCoord_FS_in + vDistort).xyz, 1.0);
   // vec4 ReflectionColor = vec4(0.4, 0.4, 1.0, 1.0);
	float height = Position_FS_in.z - Bottom_FS_in;

	float alpha =  1.0 - exp( -15.0 * height) ;

	float fresnel = 1.0 - dot(VertexToEye, Normal);

	vec4 WaterColor = mix(ReflectionColor , CubeMapColor, ReflectionFactor) ;

	WaterColor *= TotalLight;
	WaterColor = vec4(WaterColor.xyz, alpha);


	FragColor = WaterColor;


	

	
}		
	




	
                                            




