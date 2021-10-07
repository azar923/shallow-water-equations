#version 400

in vec2 TexFrag;

uniform sampler2D H_x;
uniform sampler2D H_y;
uniform sampler2D source;
uniform sampler2D values;
uniform sampler2D U_prev;

uniform float dt;
uniform int step;

layout (location = 0) out vec4 U_previous;
layout (location = 1) out vec4 U_current;

float width = 322;
float height = 242;
float dx_tex = 1.0 / width;
float dy_tex = 1.0 / height;

float dx_render = 2.0 / (width - 2 - 1);
float dy_render = 2.0 / (height - 2 - 1);


void main()
{
	U_previous = texture(values, TexFrag);
	vec3 U;

	if (step == 1)
	{
		U = texture(values, TexFrag).rgb + dt * ( -( texture(H_x, TexFrag).rgb - texture(H_x, TexFrag - vec2(dx_tex, 0.0) ).rgb ) / dx_render
			- ( texture(H_y, TexFrag).rgb - texture(H_y, TexFrag - vec2(0.0, dy_tex) ).rgb) / dy_render + texture(source, TexFrag).rgb);

		U_current = vec4(U, 1.0);
	}

	else
	{
		U = 0.5 * texture(values, TexFrag).rgb + 0.5 * (texture(U_prev,TexFrag).rgb 
		
		+ dt * ( -( texture(H_x, TexFrag).rgb -texture(H_x, TexFrag - vec2(dx_tex, 0.0 )).rgb) / dx_render
	 - ( texture(H_y, TexFrag).rgb - texture(H_y, TexFrag - vec2(0.0, dy_tex)).rgb   ) / dy_render + texture(source, TexFrag).rgb));


		U_current = vec4(U, 1.0);
	}
}