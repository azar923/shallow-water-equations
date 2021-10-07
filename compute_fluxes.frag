#version 400

in vec2 TexFrag;

layout (location = 0) out vec4 H_x;
layout (location = 1) out vec4 H_y;
layout (location = 2) out vec4 S;


uniform sampler2D U_east;
uniform sampler2D U_west;
uniform sampler2D U_north;
uniform sampler2D U_south;

uniform sampler2D values;
uniform sampler2D bottom;
uniform sampler2D speeds;

float g = 9.8;

int width = 322;
int height = 242;

float dx_tex = 1.0 / width;
float dy_tex = 1.0 / height;

float dx_render = 2.0 / (width - 2 - 1);
float dy_render = 2.0 / (height - 2 - 1);


vec3 CalculateSource(float B, vec4 Bedges, vec3 U, float g, float dx, float dy)
{
	float B_e = Bedges.x;
	float B_w = Bedges.y;
	float B_n = Bedges.z;
	float B_s = Bedges.w;

	float second = - g * (U.x - B) * ( (B_e - B_w) / dx);
	float third  = - g * (U.x - B) * ( (B_s - B_n) / dy);

	return vec3(0.0, second, third);
}


void main()
{
	float aplus  = texture(speeds, TexFrag).r;
	float aminus = texture(speeds, TexFrag).g;
	float bplus  = texture(speeds, TexFrag).b;
	float bminus = texture(speeds, TexFrag).a;

	vec3 U_e = texture(U_east,  TexFrag).rgb;
	vec3 U_w = texture(U_west,  TexFrag).rgb;
	vec3 U_n = texture(U_north, TexFrag).rgb;
	vec3 U_s = texture(U_south, TexFrag).rgb;

	float B_e = texture(bottom, TexFrag).r;
	float B_w = texture(bottom, TexFrag).g;
	float B_n = texture(bottom, TexFrag).b;
	float B_s = texture(bottom, TexFrag).a;



	vec3 F_first = vec3(0.0, 0.0, 0.0);

	if ( (U_e.x - B_e) > 0.0 )
		F_first  =  vec3( U_e.y , (U_e.y * U_e.y) / (U_e.x - B_e) + 0.5 * g *  (U_e.x - B_e) *  (U_e.x - B_e), (U_e.y * U_e.z) / (U_e.x - B_e)  );	
	else 
		F_first = vec3(0.0,0.0,0.0);
	
	vec3 U_w_next = texture(U_west, TexFrag + vec2(dx_tex, 0.0) ).rgb;
	vec3 F_second = vec3(0.0, 0.0, 0.0);

	if ( (U_w_next.x - B_e) > 0.0)
		F_second = vec3(U_w_next.y, (U_w_next.y * U_w_next.y) / (U_w_next.x - B_e) + 0.5 * g *  (U_w_next.x - B_e) *  (U_w_next.x - B_e), (U_w_next.y * U_w_next.z) / (U_w_next.x - B_e) );
	else
		F_second = vec3(0.0,0.0,0.0);

	vec3 Hx = vec3(0.0, 0.0, 0.0);

	if ( (aplus - aminus) > 0.0 )
		Hx =  ((aplus * F_first - aminus * F_second)  + (aplus * aminus) * (U_w_next - U_e) ) / (aplus - aminus);
	else
		Hx = vec3(0.0, 0.0, 0.0);

	
	vec3 U_n_next = texture(U_north, TexFrag + vec2(0.0, dy_tex)).rgb;

	vec3 G_first = vec3(0.0, 0.0, 0.0);

	if ( (U_s.x - B_s) > 0.0)
	{
		G_first  = vec3(U_s.z, (U_s.y * U_s.z) / (U_s.x - B_s), (U_s.z * U_s.z) / (U_s.x - B_s) + 0.5 * g *  (U_s.x - B_s) *  (U_s.x - B_s));
	}
	else
	{
	G_first = vec3(0.0,0.0,0.0);
	}

	vec3 G_second = vec3(0.0, 0.0, 0.0);

	if ( (U_n_next.x - B_s) > 0.0)
	{
		G_second = vec3(U_n_next.z, (U_n_next.y * U_n_next.z) / (U_n_next.x - B_s), (U_n_next.z * U_n_next.z) / (U_n_next.x - B_s) + 0.5 * g *  (U_n_next.x - B_s) *  (U_n_next.x - B_s));
	}
	else
	{
		G_second = vec3(0.0, 0.0, 0.0);
	}

	vec3 Hy = vec3(0.0, 0.0, 0.0);

	if ( (bplus - bminus) > 0)
	{
		Hy = ( (bplus * G_first - bminus * G_second)  +  (bplus * bminus) * (U_n_next - U_s) ) / (bplus - bminus);
	}
	else 
	{
		Hy = vec3(0.0, 0.0, 0.0);
	}


	float B = texture(values, TexFrag).a;

	vec4 Bedges = vec4(B_e, B_w, B_n, B_s);

	vec3 U = texture(values, TexFrag).rgb;

	vec3 source = CalculateSource(B, Bedges, U, g, dx_render, dy_render);

	// Create output vectors

	

	H_x = vec4(Hx, 1.0);
	H_y = vec4(Hy, 1.0);

	S   = vec4(source, 1.0);



}
