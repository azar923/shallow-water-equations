#version 400                                                                        

layout (location = 0) in vec3 Position;  
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 Tex;
layout (location = 3) in float bottom; 
layout (location = 4) in float lava;



out vec3 Position_FS_in;
out float Bottom_FS_in;
out vec3 Normal_FS_in;
out vec3 WorldPos_FS_in;
out vec2 TexCoord_FS_in;
out float Lava_FS_in;

uniform mat4 gWVP;    
uniform mat4 gWorld; 

uniform int sandbox_mode; 

mat4 OrthoMatrix(float left,float right, float bottom, float top, float zNear, float zFar)
{
	float tx = - ( (right + left) / (right - left) );
	float ty = - ( (top + bottom) / (top - bottom) );
	float tz = - ( (zFar + zNear) / (zFar - zNear) );


	return mat4(2.0 / (right - left), 0.0, 0.0, tx,
				0.0, 2.0 / (top - bottom), 0.0, ty,
				0.0, 0.0, -2.0 / (zFar - zNear), tz,
				0.0, 0.0, 0.0, 1.0);
}                                                                    
                                                                        
void main()                                                                         
{     
  Position_FS_in = Position;             
  Bottom_FS_in = bottom;   
  
  if (sandbox_mode == 1)
  {                                                             
	 gl_Position = OrthoMatrix(-1.0, 1.0, -1.0, 1.0, -2.0, 2.0)  * vec4(Position.x, Position.y, Position.z, 1.0); 
  }

  else
  {
	 gl_Position = gWVP * vec4(Position.x, Position.y, Position.z, 1.0);
  }

  Normal_FS_in = normalize(vec3(gWorld * vec4(Normal, 0.0)).xyz);
  WorldPos_FS_in = vec3(gWorld * vec4(Position, 1.0)).xyz;
  TexCoord_FS_in = Tex;
  Lava_FS_in = lava;
               
}
