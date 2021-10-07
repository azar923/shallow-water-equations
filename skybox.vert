#version 400

layout (location = 0) in vec3 Position;   

out vec3 TexCoord0;
uniform mat4 gWVP;    
   
                                                                                                                                  
void main()                                                                         
{    
    
  vec4 WVP_pos = gWVP * vec4(Position, 1.0);                                                                        
  gl_Position = WVP_pos.xyww;
  TexCoord0 = Position;
}