#version 400

in vec2 TexFrag;

layout (location = 0) out vec4 U_e_out;
layout (location = 1) out vec4 U_w_out;
layout (location = 2) out vec4 U_s_out;
layout (location = 3) out vec4 U_n_out;

layout (location = 4) out vec4 speeds;
layout (location = 5) out vec4 u_eigen;
layout (location = 6) out vec4 v_eigen;



uniform sampler2D velocities_u;
uniform sampler2D velocities_v;
uniform sampler2D heights;

uniform sampler2D U_e;
uniform sampler2D U_w;
uniform sampler2D U_s;
uniform sampler2D U_n;

float g = 9.8;

float width  = 322;
float height = 242;

float dx_tex = 1.0 / width;
float dy_tex = 1.0 / height;

// Helper Functions

float min_3(float a, float b, float c)
{
	float array[3];
	array[0] = a;
	array[1] = b;
	array[2] = c;
	float min = array[0];
	for (int i = 1; i < 3; i++)
		if (array[i] < min)
			min = array[i];
	return min;
}

float max_3(float a, float b, float c)
{
	float array[3];
	array[0] = a;
	array[1] = b;
	array[2] = c;
	float max = array[0];
	for (int i = 1; i < 3; i++)
		if (array[i] > max)
			max = array[i];
	return max;
}

float max_2 (float a, float b)
{
	if (a < b)
		return b;
	else if (a > b)
		return a;
	else
		return a;
}

void main()
{
	float eigen_right = sqrt(g * texture(heights, TexFrag).r);
	float eigen_left  = sqrt(g * texture(heights, TexFrag + vec2(dx_tex, 0.0)).g);

	float a_plus  = max_3(texture(velocities_u, TexFrag).r + eigen_right, texture(velocities_u, TexFrag + vec2(dx_tex, 0.0)).g + eigen_left, 0.0);
	float a_minus = min_3(texture(velocities_u, TexFrag).r - eigen_right, texture(velocities_u, TexFrag + vec2(dx_tex, 0.0)).g - eigen_left, 0.0);
			
		
	float eigen_top    = sqrt(g * texture(heights, TexFrag + vec2(0.0, dy_tex)).b);
	float eigen_bottom = sqrt(g * texture(heights, TexFrag).a);

	float b_plus  = max_3(texture(velocities_v, TexFrag).a + eigen_bottom, texture(velocities_v, TexFrag + vec2(0.0, dy_tex)).b + eigen_top, 0.0);
	float b_minus = min_3(texture(velocities_v, TexFrag).a - eigen_bottom, texture(velocities_v, TexFrag + vec2(0.0, dy_tex)).b - eigen_top, 0.0);

	float u_e = texture(velocities_u, TexFrag).r;
	float u_w = texture(velocities_u, TexFrag).g;
	float u_n = texture(velocities_u, TexFrag).b;
	float u_s = texture(velocities_u, TexFrag).a;

	float v_e = texture(velocities_v, TexFrag).r;
	float v_w = texture(velocities_v, TexFrag).g;
	float v_n = texture(velocities_v, TexFrag).b;
	float v_s = texture(velocities_v, TexFrag).a;

	float h_e = texture(heights, TexFrag).r;
	float h_w = texture(heights, TexFrag).g;
	float h_n = texture(heights, TexFrag).b;
	float h_s = texture(heights, TexFrag).a;

		
	float u_e_eigen = max_2(abs(u_e + sqrt(g * h_e)), abs(u_e - sqrt(g * h_e))); 
	float u_w_eigen = max_2(abs(u_w + sqrt(g * h_w)), abs(u_w - sqrt(g * h_w))); 
	float u_n_eigen = max_2(abs(u_n + sqrt(g * h_n)), abs(u_n - sqrt(g * h_n))); 
	float u_s_eigen = max_2(abs(u_s + sqrt(g * h_s)), abs(u_s - sqrt(g * h_s))); 


	float v_e_eigen = max_2(abs(v_e + sqrt(g * h_e)), abs(v_e - sqrt(g * h_e))); 
	float v_w_eigen = max_2(abs(v_w + sqrt(g * h_w)), abs(v_w - sqrt(g * h_w))); 			
	float v_n_eigen = max_2(abs(v_n + sqrt(g * h_n)), abs(v_n - sqrt(g * h_n))); 
	float v_s_eigen = max_2(abs(v_s + sqrt(g * h_s)), abs(v_s - sqrt(g * h_s))); 

	// Creating output vectors

	U_e_out = texture(U_e, TexFrag);
	U_w_out = texture(U_w, TexFrag);
	U_s_out = texture(U_s, TexFrag);
	U_n_out = texture(U_n, TexFrag);

	speeds  = vec4(a_plus, a_minus, b_plus, b_minus);
	u_eigen = vec4(u_e_eigen, u_w_eigen, u_n_eigen, u_s_eigen);
	v_eigen = vec4(v_e_eigen, v_w_eigen, v_n_eigen, v_s_eigen);
}