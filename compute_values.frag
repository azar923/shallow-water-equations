#version 420

in vec2 TexFrag;

layout (location = 0) out vec4 U_e_out;
layout (location = 1) out vec4 U_w_out;
layout (location = 2) out vec4 U_s_out;
layout (location = 3) out vec4 U_n_out;
layout (location = 4) out vec4 u_out;
layout (location = 5) out vec4 v_out;
layout (location = 6) out vec4 h_out;

uniform sampler2D values;
uniform sampler2D bottom;

int width = 322;
int height = 242;

float dx_tex = 1.0 / width;
float dy_tex = 1.0 / height;

float dx_render = 2.0 / (width  - 2 - 1);
float dy_render = 2.0 / (height - 2 - 1);

float teta = 1.3;
float epsilon = dx_render  * dx_render ;
float g = 9.8;

// Helper functions

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

// Kurganov-Petrova functions

vec3 MinMod(vec3 prev, vec3 curr, vec3 fut, float delta, float teta)
{
	vec3 result;

	// For x

	if (  (  (teta * ( (curr.x - prev.x) / delta )) > 0)  && (  ( (fut.x - prev.x) / (2 * delta) ) > 0) && (  (teta * ( (fut.x - curr.x) / delta )) > 0))
	{
		result.x = min_3(prev.x, curr.x, fut.x);
	}

	else if   (  (  (teta * ( (curr.x - prev.x) / delta )) < 0)  && (  ( (fut.x - prev.x) / (2 * delta) ) < 0) && (  (teta * ( (fut.x - curr.x) / delta )) < 0))
	{
		result.x = max_3(prev.x, curr.x, fut.x);
	}

	else 
		result.x = 0.0;

	//For y
	
	if (  (  (teta * ( (curr.y - prev.y) / delta )) > 0)  && (  ( (fut.y - prev.y) / (2 * delta) ) > 0) && (  (teta * ( (fut.y - curr.y) / delta )) > 0))
	{
		result.y = min_3(prev.y, curr.y, fut.y);
	}

	else if   (  (  (teta * ( (curr.y - prev.y) / delta )) < 0)  && (  ( (fut.y - prev.y) / (2 * delta) ) < 0) && (  (teta * ( (fut.y - curr.y) / delta )) < 0))
	{
		result.y = max_3(prev.y, curr.y, fut.y);
	}

	else 
		result.y = 0.0;

	//For z

	if (  (  (teta * ( (curr.z - prev.z) / delta )) > 0)  && (  ( (fut.z - prev.z) / (2 * delta) ) > 0) && (  (teta * ( (fut.z - curr.z) / delta )) > 0))
	{
		result.z = min_3(prev.z, curr.z, fut.z);
	}

	else if   (  (  (teta * ( (curr.z - prev.z) / delta )) < 0)  && (  ( (fut.z - prev.z) / (2 * delta) ) < 0) && (  (teta * ( (fut.z - curr.z) / delta )) < 0))
	{
		result.z = max_3(prev.z, curr.z, fut.z);
	}

	else 
		result.z = 0.0;


	return result;
}


void main()
{

	// Calculate minmod

	vec3 U_curr = texture(values, TexFrag).rgb;
	vec3 U_fut  = texture(values, TexFrag + vec2(dx_tex, 0.0)).rgb;
	vec3 U_prev = texture(values, TexFrag - vec2(dx_tex, 0.0)).rgb;

	vec3 Ux = MinMod(U_prev, U_curr, U_fut, dx_render, teta);
	
	U_fut   = texture(values, TexFrag + vec2(0, dy_tex)).rgb;
	U_prev  = texture(values, TexFrag - vec2(0, dy_tex)).rgb;

	vec3 Uy = MinMod(U_prev, U_curr, U_fut, dy_render, teta);

	// Calculate u_edges;

	vec3 U_e = U_curr + ((dx_render) / 2 ) * Ux;
	vec3 U_w = U_curr - ((dx_render) / 2 ) * Ux;
	vec3 U_n = U_curr - ((dy_render) / 2 ) * Uy;
	vec3 U_s = U_curr + ((dy_render) / 2 ) * Uy;

	float B_e = texture(bottom, TexFrag).x;
	float B_w = texture(bottom, TexFrag).y;
	float B_n = texture(bottom, TexFrag).z;
	float B_s = texture(bottom, TexFrag).w;

	if (U_w.x < B_w)
		{
			U_e.x = 2 * U_curr.x - B_w; 			
			U_w.x = B_w;		
		}

	if (U_e.x < B_e)
		{
			U_e.x = B_e;
			U_w.x = 2 * U_curr.x - B_e;
		}


		if (U_n.x < B_n)
		{				
			U_n.x = B_n;
			U_s.x = 2 * U_curr.x - B_n;
		}
			
		if (U_s.x < B_s)
		{	
			U_n.x = 2 * U_curr.x - B_s;
			U_s.x = B_s;
		}
		
	
	float h_e = U_e.x - B_e;
	float h4 = h_e * h_e * h_e * h_e;

	float u_e = (1.41 * h_e * U_e.y) / (sqrt(h4 + max_2(epsilon, h4) ) );
	float v_e = (1.41 * h_e * U_e.z) / (sqrt(h4 + max_2(epsilon, h4) ) );

	U_e.y = h_e * u_e;
	U_e.z = h_e * v_e;

	
	float h_w = U_w.x - B_w;
	h4 = h_w * h_w * h_w * h_w;

	float u_w = (1.41 * h_w * U_w.y) / (sqrt(h4 + max_2(epsilon, h4)));
	float v_w = (1.41 * h_w * U_w.z) / (sqrt(h4 + max_2(epsilon, h4)));

	U_w.y = h_w * u_w;
	U_w.z = h_w * v_w;

	
	float h_n = U_n.x - B_n;
	h4 = h_n * h_n * h_n * h_n;

	float u_n = (1.41 * h_n * U_n.y) / (sqrt(h4 + max_2(epsilon, h4)));
	float v_n = (1.41 * h_n * U_n.z) / (sqrt(h4 + max_2(epsilon, h4)));

	U_n.y = h_n * u_n;
	U_n.z = h_n * v_n;


	float h_s = U_s.x - B_s;
	h4 = h_s * h_s * h_s * h_s;

	float u_s = (1.41 * h_s * U_s.y) / (sqrt(h4 + max_2(epsilon, h4)));
	float v_s = (1.41 * h_s * U_s.z) / (sqrt(h4 + max_2(epsilon, h4)));

	U_s.y = h_s * u_s;
	U_s.z = h_s * v_s;

	// Filling output vectors

	U_e_out = vec4(U_e, 1.0);
	U_w_out = vec4(U_w, 1.0);
	U_n_out = vec4(U_n, 1.0);
	U_s_out = vec4(U_s, 1.0);

	u_out = vec4(u_e, u_w, u_n, u_s);
	v_out = vec4(v_e, v_w, v_n, v_s);
	h_out = vec4(h_e, h_w, h_n, h_s);

}