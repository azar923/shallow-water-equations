#include "math_3d.h"
#include "helper.h"
#include "pipeline.h"
#include "globals.h"
#include "kernel.h"
#include "camera.h"

using namespace std;

	const Matrix4f* WVP;
	const Matrix4f* World;
	Vector3f Direction;



void initialize(Point *grid, Vertex_water *vertices)
{
	for (int i = 0; i < height ; i++)
		for (int j = 0; j < width ; j++)
		{
			int idx = i * width + j;

			Vertices_water[idx].pos.z = Vertices_terrain[idx].pos.z;
		}
	
	for (int i = 1, m = 0; i < gridHeight - 1 ; i ++, m++)
	{
		for (int j = 1, k = 0; j < gridWidth - 1; j ++, k++)
		{	
			
			grid[i * gridWidth + j].U.x = Vertices_water[m * width + k].pos.z;
			grid[i * gridWidth + j].B	= Vertices_terrain[m * width + k].pos.z;
		
			grid[i * gridWidth + j].U.y = 0.0;
			grid[i * gridWidth + j].U.z = 0.0;
		}
	}

	// Top and bottom edges
	for (int i = 0; i < gridWidth; i++)
	{
		int basic = i;
		grid[basic].B = grid[basic  + gridWidth ].B;

		int verticalShift = (gridHeight - 1) * gridWidth;
		basic = i + verticalShift;
		grid[basic].B = grid[basic - gridWidth].B;		
	}

	//Right and left edges
	for (int i = 0;  i < gridHeight ; i++)
	{
		int basic = i * gridWidth;
		grid[basic].B = grid[basic + 1].B;

		int horisontalShift = gridWidth - 1;
		basic = i * gridWidth + horisontalShift;
		grid[basic].B = grid[basic - 1].B;
	}
	
	for (int i = 1; i < gridHeight - 1; i++)
		for (int j = 1; j < gridWidth - 1; j++)
		{
			int idx = i * gridWidth + j;
			grid[idx].B_w = (grid[idx].B + grid[idx - 1].B) / 2;
			grid[idx].B_n = (grid[idx].B + grid[idx - gridWidth].B) / 2;
			grid[idx].B_e = (grid[idx].B + grid[idx + 1].B) / 2;
			grid[idx].B_s = (grid[idx].B + grid[idx + gridWidth].B) / 2;
		}

	for (int i = 0; i < gridWidth; i++)
	{
		int basic = i;
		grid[basic].B_s = grid[basic  + gridWidth ].B_n;

		int verticalShift = (gridHeight - 1) * gridWidth;
		basic = i + verticalShift;
		grid[basic].B_n = grid[basic - gridWidth].B_s;		
	}

	//Right and left edges
	for (int i = 0;  i < gridHeight ; i++)
	{
		int basic = i * gridWidth;
		grid[basic].B_e = grid[basic + 1].B_w;

		int horisontalShift = gridWidth - 1;
		basic = i * gridWidth + horisontalShift;
		grid[basic].B_w = grid[basic - 1].B_e;
	}
			
}


void initializeBottom()
{
	for (int i = 1, m = 0; i < gridHeight - 1 ; i ++, m++)
	{
		for (int j = 1, k = 0; j < gridWidth - 1; j ++, k++)
		{	
			grid[i * gridWidth + j].B	= Vertices_terrain[m * width + k].pos.z;
			grid[i * gridWidth + j].U.x = Vertices_water[m * width + k].pos.z;

		}
	}

	// Top and bottom edges
	for (int i = 0; i < gridWidth; i++)
	{
		int basic = i;
		grid[basic].B = grid[basic  + gridWidth ].B;

		int verticalShift = (gridHeight - 1) * gridWidth;
		basic = i + verticalShift;
		grid[basic].B = grid[basic - gridWidth].B;	
	}

	//Right and left edges
	for (int i = 0;  i < gridHeight ; i++)
	{
		int basic = i * gridWidth;
		grid[basic].B = grid[basic + 1].B;
		grid_lava[basic].B = grid_lava[basic + 1].B;

		int horisontalShift = gridWidth - 1;
		basic = i * gridWidth + horisontalShift;
		grid[basic].B = grid[basic - 1].B;
		grid_lava[basic].B = grid_lava[basic - 1].B;
	}
	
	for (int i = 1; i < gridHeight - 1; i++)
		for (int j = 1; j < gridWidth - 1; j++)
		{
			int idx = i * gridWidth + j;
			grid[idx].B_w = (grid[idx].B + grid[idx - 1].B) / 2;
			grid[idx].B_n = (grid[idx].B + grid[idx - gridWidth].B) / 2;
			grid[idx].B_e = (grid[idx].B + grid[idx + 1].B) / 2;
			grid[idx].B_s = (grid[idx].B + grid[idx + gridWidth].B) / 2;

			grid_lava[idx].B_w = (grid_lava[idx].B + grid_lava[idx - 1].B) / 2;
			grid_lava[idx].B_n = (grid_lava[idx].B + grid_lava[idx - gridWidth].B) / 2;
			grid_lava[idx].B_e = (grid_lava[idx].B + grid_lava[idx + 1].B) / 2;
			grid_lava[idx].B_s = (grid_lava[idx].B + grid_lava[idx + gridWidth].B) / 2;
		}

	for (int i = 0; i < gridWidth; i++)
	{
		int basic = i;
		grid[basic].B_s = grid[basic  + gridWidth ].B_n;
		grid_lava[basic].B_s = grid_lava[basic  + gridWidth ].B_n;

		int verticalShift = (gridHeight - 1) * gridWidth;
		basic = i + verticalShift;
		grid[basic].B_n = grid[basic - gridWidth].B_s;		
		grid_lava[basic].B_n = grid_lava[basic - gridWidth].B_s;	
	}

	//Right and left edges
	for (int i = 0;  i < gridHeight ; i++)
	{
		int basic = i * gridWidth;
		grid[basic].B_e = grid[basic + 1].B_w;
		grid_lava[basic].B_e = grid_lava[basic + 1].B_w;

		int horisontalShift = gridWidth - 1;
		basic = i * gridWidth + horisontalShift;
		grid[basic].B_w = grid[basic - 1].B_e;
		grid_lava[basic].B_w = grid_lava[basic - 1].B_e;
	}
}

void SetLighting()
{
	Light.AmbientIntensity = 0.35;
	Light.Color = Vector3f(1.0, 1.0, 1.0);
	Light.DiffuseIntensity = 1.0;
	Light.Direction = Vector3f(0.1,1.0,0.1);

	for (int i = 0; i < NumLights; i++)
	{
		PointLight[i].AmbientIntensity = 0.0;
		PointLight[i].Color = PointLightColors[i];
		PointLight[i].DiffuseIntensity = 0.0;
		PointLight[i].Position = PointLightPositions[i];
		PointLight[i].constant = 0.0;
		PointLight[i].linear = 0.001;
		PointLight[i].exp = 0.00001;
	}
}

void setBoundary()
{
		//Right and left edges
	for (int i = 0; i < gridHeight; i++)
	{
		int basic = i * gridWidth;
		grid[basic].U.x =  grid[basic + 1].U.x;
		grid[basic].U.y = -grid[basic + 1].U.y;
		grid[basic].U.z = -grid[basic + 1].U.z;

		grid_lava[basic].U.x =  grid_lava[basic + 1].U.x;
		grid_lava[basic].U.y = -grid_lava[basic + 1].U.y;
		grid_lava[basic].U.z = -grid_lava[basic + 1].U.z;

		int horisontalShift = gridWidth - 1;
		basic = i * gridWidth + horisontalShift;
		grid[basic].U.x = grid[basic - 1].U.x;
		grid[basic].U.y = -grid[basic - 1].U.y;
		grid[basic].U.z = -grid[basic - 1].U.z;

		grid_lava[basic].U.x = grid_lava[basic - 1].U.x;
		grid_lava[basic].U.y = -grid_lava[basic - 1].U.y;
		grid_lava[basic].U.z = -grid_lava[basic - 1].U.z;
	}
	
	// Top and bottom edges
	for (int i = 0; i < gridWidth; i++)
	{
		int basic = i;
		grid[basic].U.x =  grid[basic  + gridWidth].U.x;
		grid[basic].U.y = -grid[basic  + gridWidth].U.y;
		grid[basic].U.z = -grid[basic  + gridWidth].U.z;

		grid_lava[basic].U.x =  grid_lava[basic  + gridWidth].U.x;
		grid_lava[basic].U.y = -grid_lava[basic  + gridWidth].U.y;
		grid_lava[basic].U.z = -grid_lava[basic  + gridWidth].U.z;

		int verticalShift = (gridHeight - 1) * gridWidth;
		basic = i + verticalShift;
		grid[basic].U.x =  grid[basic - gridWidth].U.x;
		grid[basic].U.y = -grid[basic - gridWidth].U.y;
		grid[basic].U.z = -grid[basic - gridWidth].U.z;

		grid_lava[basic].U.x =  grid_lava[basic - gridWidth].U.x;
		grid_lava[basic].U.y = -grid_lava[basic - gridWidth].U.y;
		grid_lava[basic].U.z = -grid_lava[basic - gridWidth].U.z;
	}

}

void Kurganov_Petrova(Point *grid)
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glDisable(GL_BLEND);

	glViewport(0,0, gridWidth, gridHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLenum buffers[7] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6};
	glDrawBuffers(7, buffers);
	
	static float data[gridWidth * gridHeight * 4];

	for (int i = 0, k = 0; i < gridWidth * gridHeight * 4, k < gridWidth * gridHeight; i+=4, k++)
	{
		data[i]   = grid[k].U.x;
		data[i+1] = grid[k].U.y;
		data[i+2] = grid[k].U.z;
		data[i+3] = grid[k].B;
	}

	glBindTexture(GL_TEXTURE_2D, values_ind);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, gridWidth, gridHeight, 0, GL_RGBA, GL_FLOAT, data);

	for (int i = 0, k = 0; i < gridWidth * gridHeight * 4, k < gridWidth * gridHeight; i+=4, k++)
	{
		data[i]   = grid[k].B_e;
		data[i+1] = grid[k].B_w;
		data[i+2] = grid[k].B_n;
		data[i+3] = grid[k].B_s;
	}
	
	glBindTexture(GL_TEXTURE_2D, bottom_ind);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, gridWidth, gridHeight, 0, GL_RGBA, GL_FLOAT, data);

	int step = 1;

	glUseProgram(program_values);

	values_location = glGetUniformLocation(program_values, "values");
	glUniform1i(values_location, 14);

	bottom_location = glGetUniformLocation(program_values, "bottom");
	glUniform1i(bottom_location, 15);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_calculate);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_calculate), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_calculate), (const GLvoid*)12);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_calculate);

    glDrawElements(GL_TRIANGLES, (gridWidth - 1) * (gridHeight - 1) * 6, GL_UNSIGNED_INT, 0);


	glUseProgram(NULL);

	glUseProgram(program_eigen);

	U_e_location_eigen = glGetUniformLocation(program_eigen, "U_e");
	glUniform1i(U_e_location_eigen, 0);

	U_w_location_eigen = glGetUniformLocation(program_eigen, "U_w");
	glUniform1i(U_w_location_eigen, 1);

	U_s_location_eigen = glGetUniformLocation(program_eigen, "U_s");
	glUniform1i(U_s_location_eigen, 2);

	U_n_location_eigen = glGetUniformLocation(program_eigen, "U_n");
	glUniform1i(U_n_location_eigen, 3);

	u_location = glGetUniformLocation(program_eigen, "velocities_u");
	glUniform1i(u_location, 4);

	v_location = glGetUniformLocation(program_eigen, "velocities_v");
	glUniform1i(v_location, 5);

	h_location = glGetUniformLocation(program_eigen, "heights");
	glUniform1i(h_location, 6);


    glDrawElements(GL_TRIANGLES, (gridWidth - 1) * (gridHeight - 1) * 6, GL_UNSIGNED_INT, 0);

	glUseProgram(NULL);


    glBindTexture(GL_TEXTURE_2D, render5_ind);
	static float u_eigen[4 * gridWidth * gridHeight];
	glGetTexImage(GL_TEXTURE_2D,0, GL_RGBA, GL_FLOAT, u_eigen);
	
	glBindTexture(GL_TEXTURE_2D, render6_ind);
	static float v_eigen[4 * gridWidth * gridHeight];
	glGetTexImage(GL_TEXTURE_2D,0, GL_RGBA, GL_FLOAT, v_eigen);

	float max_u = max_elem(u_eigen, 4 * gridWidth * gridHeight);
	float max_v = max_elem(v_eigen, 4 * gridWidth * gridHeight);

	if ( (max_u > 0.0) && (max_v > 0.0) )
	{
		float one = dx / max_u;
		float two = dy / max_v;
	
		dt =  0.45 * min_2(one, two);
	}

	else
		dt = 0.001;

    glUseProgram(program_fluxes);

	U_e_location_fluxes = glGetUniformLocation(program_fluxes, "U_east");
	glUniform1i(U_e_location_fluxes, 0);

	U_w_location_fluxes = glGetUniformLocation(program_fluxes, "U_west");
	glUniform1i(U_w_location_fluxes, 1);

	U_s_location_fluxes = glGetUniformLocation(program_fluxes, "U_south");
	glUniform1i(U_s_location_fluxes, 2);

	U_n_location_fluxes = glGetUniformLocation(program_fluxes, "U_north");
	glUniform1i(U_n_location_fluxes, 3);

	speeds_location = glGetUniformLocation(program_fluxes, "speeds");
	glUniform1i(speeds_location, 4);

	values_location_fluxes = glGetUniformLocation(program_fluxes, "values");
	glUniform1i(values_location_fluxes, 14);

	bottom_location_fluxes = glGetUniformLocation(program_fluxes, "bottom");
	glUniform1i(bottom_location_fluxes, 15);

    glDrawElements(GL_TRIANGLES, (gridWidth - 1) * (gridHeight - 1) * 6, GL_UNSIGNED_INT, 0);

	glUseProgram(NULL);

	glUseProgram(program_runge);

	H_x_location = glGetUniformLocation(program_runge, "H_x");
	glUniform1i(H_x_location, 0);

	H_y_location = glGetUniformLocation(program_runge, "H_y");
	glUniform1i(H_y_location, 1);

	source_location = glGetUniformLocation(program_runge, "source");
	glUniform1i(source_location, 2);

	values_location_runge = glGetUniformLocation(program_runge, "values");
	glUniform1i(values_location_runge, 14);

	timeLocation = glGetUniformLocation(program_runge, "dt");
	glUniform1f(timeLocation, dt);

	stepLocation = glGetUniformLocation(program_runge, "step");
	glUniform1i(stepLocation, step);

	U_prev_location = glGetUniformLocation(program_runge, "U_prev");
	glUniform1i(U_prev_location, 16);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

    glDrawElements(GL_TRIANGLES, (gridWidth - 1) * (gridHeight - 1) * 6, GL_UNSIGNED_INT, 0);

	glUseProgram(NULL);

	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glBindTexture(GL_TEXTURE_2D, U_prev_ind);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 0, 0, gridWidth, gridHeight, 0);

	glBindTexture(GL_TEXTURE_2D, render1_ind);
	static float U_curr[4 * gridWidth * gridHeight];
	glGetTexImage(GL_TEXTURE_2D,0, GL_RGBA, GL_FLOAT, U_curr);

	for (int i = 0, k = 0; i < gridWidth * gridHeight * 4, k < gridWidth * gridHeight; i+=4, k++)
	{
		grid[k].U.x  = U_curr[i];
		grid[k].U.y =  U_curr[i+1];
		grid[k].U.z =  U_curr[i+2];	
	}
	
	setBoundary();

	for (int i = 0, k = 0; i < gridWidth * gridHeight * 4, k < gridWidth * gridHeight; i+=4, k++)
	{
		data[i]   = grid[k].U.x;
		data[i+1] = grid[k].U.y;
		data[i+2] = grid[k].U.z;
		data[i+3] = grid[k].B;
	}

	glBindTexture(GL_TEXTURE_2D, values_ind);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, gridWidth, gridHeight, 0, GL_RGBA, GL_FLOAT, data);

	step = 2;
	
	glUseProgram(program_values);

    glDrawElements(GL_TRIANGLES, (gridWidth - 1) * (gridHeight - 1) * 6, GL_UNSIGNED_INT, 0);

	glUseProgram(NULL);


	glUseProgram(program_eigen);

    glDrawElements(GL_TRIANGLES, (gridWidth - 1) * (gridHeight - 1) * 6, GL_UNSIGNED_INT, 0);

	glUseProgram(NULL);



	glUseProgram(program_fluxes);

    glDrawElements(GL_TRIANGLES, (gridWidth - 1) * (gridHeight - 1) * 6, GL_UNSIGNED_INT, 0);

	glUseProgram(NULL);

	
	glUseProgram(program_runge);

    glDrawElements(GL_TRIANGLES, (gridWidth - 1) * (gridHeight - 1) * 6, GL_UNSIGNED_INT, 0);

	glUseProgram(NULL);

	glBindTexture(GL_TEXTURE_2D, render1_ind);
	U_curr[4 * gridWidth * gridHeight];
	glGetTexImage(GL_TEXTURE_2D,0, GL_RGBA, GL_FLOAT, U_curr);
     	
	for (int i = 0, k = 0; i < gridWidth * gridHeight * 4, k < gridWidth * gridHeight; i+=4, k++)
	{
		grid[k].U.x = U_curr[i];
		grid[k].U.y = U_curr[i+1];
		grid[k].U.z = U_curr[i+2];
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0,0,WINDOW_WIDTH, WINDOW_HEIGHT);
}


void RungeCutta()
{
	Kurganov_Petrova(grid);

	setBoundary();


	for (int i = 1, m = 0; i < gridHeight - 1 ; i ++, m++)
	{
		for (int j = 1, k = 0; j < gridWidth - 1; j ++, k++)
		{
			Vertices_water[m * width + k].pos.z = grid[i * gridWidth + j].U.x;
			Vertices_water[m * width + k].bottom_pos = grid[i * gridWidth + j].B;

		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBO_water);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices_water), Vertices_water);

}



void ComputeNormals()
{

	for (int i = 0; i < (width-1) * (height-1) * 6; i += 3)
	{
		unsigned int Index0 = Indices[i];
		unsigned int Index1 = Indices[i + 1];
		unsigned int Index2 = Indices[i + 2];

		Vector3f v1 = Vertices_water[Index1].pos - Vertices_water[Index0].pos;
		Vector3f v2 = Vertices_water[Index2].pos - Vertices_water[Index1].pos; 

		Vector3f Normal = v1.Cross(v2);
		Normal.Normalize();

		Vertices_water[Index0].normal += Normal;
		Vertices_water[Index1].normal += Normal;
		Vertices_water[Index2].normal += Normal;


		v1 = Vertices_terrain[Index1].pos - Vertices_terrain[Index0].pos;
		v2 = Vertices_terrain[Index2].pos - Vertices_terrain[Index1].pos;
		Normal = v1.Cross(v2);
		Normal.Normalize();
		
		Vertices_terrain[Index0].normal += Normal;
		Vertices_terrain[Index1].normal += Normal;
		Vertices_terrain[Index2].normal += Normal;		
	}
	
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			Vertices_water[i * width + j].normal.Normalize();			 			
			Vertices_terrain[i * width + j].normal.Normalize();		
		}
		


	glBindBuffer(GL_ARRAY_BUFFER, VBO_water);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices_water), Vertices_water);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_terrain);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices_terrain), Vertices_terrain);



}


void LoadTextures()
{
	grass     =  new Texture (GL_TEXTURE_2D, "terrain/grass.bmp" );
	sand      =  new Texture (GL_TEXTURE_2D, "terrain/sand.bmp"  );
	stone     =  new Texture (GL_TEXTURE_2D, "terrain/stone.bmp" );
	snow      =  new Texture (GL_TEXTURE_2D, "terrain/snow.bmp"  );
	water_tex =  new Texture (GL_TEXTURE_2D, "terrain/water.bmp" );
	
	grass      ->    Load();
	sand       ->    Load();
	stone      ->    Load();
	snow       ->    Load();
	water_tex  ->    Load();
	

}

void BindTextures()
{
	grass      ->   Bind(GL_TEXTURE17);
	sand       ->   Bind(GL_TEXTURE18);
	stone      ->   Bind(GL_TEXTURE19);
	snow       ->   Bind(GL_TEXTURE20);
	water_tex  ->   Bind(GL_TEXTURE24);
}

void CreateShallowTextures()
{
	CreateTexture(GL_TEXTURE14,  values_ind);
	CreateTexture(GL_TEXTURE15,  bottom_ind);
	CreateTexture(GL_TEXTURE16,  U_prev_ind);

}

void CreateFrameBuffer()
{	
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	CreateRenderTexture(render0_ind,  GL_TEXTURE0,  GL_COLOR_ATTACHMENT0,  gridWidth, gridHeight);
	CreateRenderTexture(render1_ind,  GL_TEXTURE1,  GL_COLOR_ATTACHMENT1,  gridWidth, gridHeight);
	CreateRenderTexture(render2_ind,  GL_TEXTURE2,  GL_COLOR_ATTACHMENT2,  gridWidth, gridHeight);
	CreateRenderTexture(render3_ind,  GL_TEXTURE3,  GL_COLOR_ATTACHMENT3,  gridWidth, gridHeight);
	CreateRenderTexture(render4_ind,  GL_TEXTURE4,  GL_COLOR_ATTACHMENT4,  gridWidth, gridHeight);
	CreateRenderTexture(render5_ind,  GL_TEXTURE5,  GL_COLOR_ATTACHMENT5,  gridWidth, gridHeight);
	CreateRenderTexture(render6_ind,  GL_TEXTURE6,  GL_COLOR_ATTACHMENT6,  gridWidth, gridHeight);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenFramebuffers(1, &FBO_water);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO_water);

	glGenTextures(1, &reflection_ind);
	glActiveTexture(GL_TEXTURE22);
	glBindTexture(GL_TEXTURE_2D, reflection_ind);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, reflection_ind, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CreateSkyboxBuffer()
{
		GLfloat CubeVertices[] = {-1.0, 1.0, 1.0,
							  -1.0, -1.0, 1.0,
							  1.0, -1.0, 1.0,
							  1.0, 1.0, 1.0,
							  -1.0, 1.0, -1.0,
							  -1.0, -1.0, -1.0,
							  1.0, -1.0, -1.0,
							  1.0, 1.0, -1.0};
	glGenBuffers(1, &VBO_skybox);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_skybox);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);

	unsigned int CubeIndices[] = {0, 1, 2, 3,
							  3, 2, 6, 7,
							  7, 6, 5, 4,
							  4, 5, 1, 0,
							  0, 3, 7, 4,
							  1, 2, 6, 5};

	glGenBuffers(1, &IBO_skybox);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_skybox);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(CubeIndices), CubeIndices, GL_STATIC_DRAW);
}

void melting()
{
	if (time_melting < 1.0)
	{
		time_melting += 0.001;
	
		for (int i = 0; i < height ; i++)
			for (int j = 0; j < width ; j++)
			{
				int idx = i * width + j;

				if (Vertices_terrain[idx].pos.z > 0.85) 
						Vertices_water[idx].pos.z += 0.0002;					
			}
	}
	

	glBindBuffer(GL_ARRAY_BUFFER, VBO_water);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices_water), Vertices_water);
}


void RenderReflection()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO_water);
		
	p.SetCamera(Vector3f(-100.0f, 300.0f, 100.0f), Vector3f(0.0, -0.95, 0.05), Vector3f(0.0f, 1.0f, 0.0f) );

	Vector3f pos = pGameCamera->GetPos();
	  
    p.SetPerspectiveProj(3.5f, WINDOW_WIDTH, WINDOW_HEIGHT, 100.0f, 500.0f);

	WVP = p.GetWVPTrans();
	World = p.GetWorldTrans();

	Direction = Light.Direction;
	Direction.Normalize();

	

	glUseProgram(program_terrain);

	time_melting_location = glGetUniformLocation(program_terrain, "time_melting");
	glUniform1f(time_melting_location, time_melting);
	
	glUniform1i(fog_mode_location, fog_mode);
    glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)WVP);
	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)World);
	
	glUniform3f(EyePosLocation_terrain, pos.x, pos.y, pos.z);
	glUniform1f(dirLightDiffuseIntensityLocation, Light.DiffuseIntensity);
	glUniform3f(dirLightDirectionLocation, Direction.x, Direction.y, Direction.z);

	for (int i = 0; i < NumLights; i++)
	{

		glUniform3f(pointLightColorLocation[i] , PointLight[i].Color.x, PointLight[i].Color.y, PointLight[i].Color.z);
		glUniform3f(pointLightPositionLocation[i] , PointLight[i].Position.x, PointLight[i].Position.y, PointLight[i].Position.z);

		glUniform1f(pointLightAmbientIntensityLocation[i] , PointLight[i].AmbientIntensity);
		glUniform1f(pointLightDiffuseIntensityLocation[i] , PointLight[i].DiffuseIntensity);

		glUniform1f(pointLightAttenConstantLocation[i] , PointLight[i].constant);
		glUniform1f(pointLightAttenLinearLocation[i] , PointLight[i].linear);
		glUniform1f(pointLightAttenExpLocation[i] , PointLight[i].exp);
	}

    glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);


    glBindBuffer(GL_ARRAY_BUFFER, VBO_terrain);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_terrain), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_terrain), (const GLvoid*)12);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_terrain), (const GLvoid*)24);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    glDrawElements(GL_TRIANGLES, (width - 1) * (height - 1) * 6, GL_UNSIGNED_INT, 0);




	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glBindTexture(GL_TEXTURE_2D, reflection_ind);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTerrain()
{
	p.SetCamera(pGameCamera->GetPos(), pGameCamera->GetTarget(), pGameCamera->GetUp() );

	p.SetPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 1000.0f);
	
	WVP = p.GetWVPTrans();
	World = p.GetWorldTrans();

	pos = pGameCamera->GetPos();

	glUseProgram(program_terrain);
	
    glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)WVP);
	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)World);


	glUniform1i(fog_mode_location, fog_mode);
	glUniform1i(sandbox_mode_location_terrain, sandbox_mode);
	glUniform3f(EyePosLocation_terrain, pos.x, pos.y, pos.z);
	glUniform1f(dirLightDiffuseIntensityLocation, Light.DiffuseIntensity);
	glUniform3f(dirLightDirectionLocation, Direction.x, Direction.y, Direction.z);


	for (int i = 0; i < NumLights; i++)
	{

		glUniform3f(pointLightColorLocation[i] , PointLight[i].Color.x, PointLight[i].Color.y, PointLight[i].Color.z);
		glUniform3f(pointLightPositionLocation[i] , PointLight[i].Position.x, PointLight[i].Position.y, PointLight[i].Position.z);

		glUniform1f(pointLightAmbientIntensityLocation[i] , PointLight[i].AmbientIntensity);
		glUniform1f(pointLightDiffuseIntensityLocation[i] , PointLight[i].DiffuseIntensity);

		glUniform1f(pointLightAttenConstantLocation[i] , PointLight[i].constant);
		glUniform1f(pointLightAttenLinearLocation[i] , PointLight[i].linear);
		glUniform1f(pointLightAttenExpLocation[i] , PointLight[i].exp);
	}


    glBindBuffer(GL_ARRAY_BUFFER, VBO_terrain);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_terrain), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_terrain), (const GLvoid*)12);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_terrain), (const GLvoid*)24);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    glDrawElements(GL_TRIANGLES, (width - 1) * (height - 1) * 6, GL_UNSIGNED_INT, 0);


	glUseProgram(NULL);
}

void RenderWater()
{
	
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	
	glUseProgram(program_water);

	reflection_location = glGetUniformLocation(program_water, "reflection");
	glUniform1i(reflection_location, 22);

    glUniformMatrix4fv(gWVPLocation_water, 1, GL_TRUE, (const GLfloat*)WVP);
	glUniformMatrix4fv(gWorldLocation_water, 1, GL_TRUE, (const GLfloat*)World);

	pos = pGameCamera->GetPos();

	glUniform3f(EyePosLocation, pos.x, pos.y, pos.z);
	glUniform1f(dirLightDiffuseIntensityLocation_water, Light.DiffuseIntensity);
	glUniform1f(AlphaFactorLocation, AlphaFactor);
	glUniform1f(ReflectionFactorLocation, ReflectionFactor);
	glUniform3f(dirLightDirectionLocation_water, Direction.x, Direction.y, Direction.z);

	glUniform1i(sandbox_mode_location_water, sandbox_mode);

	for (int i = 0; i < NumLights; i++)
	{
		glUniform3f(pointLightColorLocation_water[i] , PointLight[i].Color.x, PointLight[i].Color.y, PointLight[i].Color.z);
		glUniform3f(pointLightPositionLocation_water[i] , PointLight[i].Position.x, PointLight[i].Position.y, PointLight[i].Position.z);

		glUniform1f(pointLightAmbientIntensityLocation_water[i] , PointLight[i].AmbientIntensity);
		glUniform1f(pointLightDiffuseIntensityLocation_water[i] , PointLight[i].DiffuseIntensity);

		glUniform1f(pointLightAttenConstantLocation_water[i] , PointLight[i].constant);
		glUniform1f(pointLightAttenLinearLocation_water[i] , PointLight[i].linear);
		glUniform1f(pointLightAttenExpLocation_water[i] , PointLight[i].exp);
	}

    glBindBuffer(GL_ARRAY_BUFFER, VBO_water);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_water), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_water), (const GLvoid*)12);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_water), (const GLvoid*)24);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex_water), (const GLvoid*)32);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex_water), (const GLvoid*)36);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    glDrawElements(GL_TRIANGLES, (width - 1) * (height - 1) * 6, GL_UNSIGNED_INT, 0);


	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
}

void RenderSkybox()
{
	GLint OldCullFaceMode;
	glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
	GLint OldDepthFuncMode;
	glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);
	p.Scale(1000.0 , 1000.0, 1000.0);
    p.Rotate(180.0f, 0.0f, 0.0f);
	p.WorldPos(pGameCamera->GetPos().x, pGameCamera->GetPos().y, pGameCamera->GetPos().z);

	p.SetCamera(pGameCamera->GetPos(), pGameCamera->GetTarget(), pGameCamera->GetUp() );
 
    p.SetPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 1000.0f);

	glUseProgram(program_skybox);

	skybox_location = glGetUniformLocation(program_skybox, "skybox");
	glUniform1i(skybox_location, 21);

	gWVPLocation_skybox = glGetUniformLocation(program_skybox, "gWVP");

	glUniformMatrix4fv(gWVPLocation_skybox, 1, GL_TRUE, (const GLfloat*)p.GetWVPTrans());
	
    glBindBuffer(GL_ARRAY_BUFFER, VBO_skybox);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_skybox);

    glDrawElements(GL_QUADS, 24 , GL_UNSIGNED_INT, 0);

	glCullFace(OldCullFaceMode);
	glDepthFunc(OldDepthFuncMode);
	
	glUseProgram(NULL);
}

void RenderScene()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Update();
	initializeBottom();
	

	RungeCutta();		
	
	
	//melting();
	ComputeNormals();


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	p.Scale(200.0 , 200.0, 200.0);
    p.Rotate(-90.0f, 0.0f, 0.0f);
    p.WorldPos(-100.0f, 0.0f, 100.0f);

	RenderReflection();
	RenderTerrain();
	RenderWater();
	RenderSkybox();

    glutSwapBuffers();

}



void SetUniforms()
{

	glUseProgram(program_terrain);

	grassLocation  =   glGetUniformLocation(program_terrain, "grass" );
	sandLocation   =   glGetUniformLocation(program_terrain, "sand"  );
	stoneLocation  =   glGetUniformLocation(program_terrain, "stone" );	
	snowLocation   =   glGetUniformLocation(program_terrain, "snow");
	waterLocation  =   glGetUniformLocation(program_terrain, "water");

	gWVPLocation =   glGetUniformLocation(program_terrain, "gWVP");
	gWorldLocation = glGetUniformLocation(program_terrain, "gWorld");


	dirLightColorLocation = glGetUniformLocation(program_terrain, "gDirectionalLight.Color");
	dirLightAmbientIntensityLocation = glGetUniformLocation(program_terrain, "gDirectionalLight.AmbientIntensity");
	dirLightDiffuseIntensityLocation = glGetUniformLocation(program_terrain, "gDirectionalLight.DiffuseIntensity");
	dirLightDirectionLocation = glGetUniformLocation(program_terrain, "gDirectionalLight.Direction");

	for (int i = 0; i < NumLights; i++)
	{
		char name[128];
		memset(name, 0, sizeof(name));

		_snprintf(name, sizeof(name), "gPointLight[%d].Color", i);
		pointLightColorLocation[i] = glGetUniformLocation(program_terrain, name);

		_snprintf(name, sizeof(name), "gPointLight[%d].Position", i);
		pointLightPositionLocation[i] = glGetUniformLocation(program_terrain, name);

		_snprintf(name, sizeof(name), "gPointLight[%d].AmbientIntensity", i);
		pointLightAmbientIntensityLocation[i] = glGetUniformLocation(program_terrain, name);

		_snprintf(name, sizeof(name), "gPointLight[%d].DiffuseIntensity", i);
		pointLightDiffuseIntensityLocation[i] = glGetUniformLocation(program_terrain, name);

		_snprintf(name, sizeof(name), "gPointLight[%d].Constant", i);
		pointLightAttenConstantLocation[i] = glGetUniformLocation(program_terrain, name);

		_snprintf(name, sizeof(name), "gPointLight[%d].Linear", i);
		pointLightAttenLinearLocation[i] = glGetUniformLocation(program_terrain, name);

		_snprintf(name, sizeof(name), "gPointLight[%d].Exp", i);
		pointLightAttenExpLocation[i] = glGetUniformLocation(program_terrain, name);
	}


	MatSpecularIntensityLocation_terrain = glGetUniformLocation(program_terrain, "gMatSpecularIntensity");

	SpecularPowerLocation_terrain = glGetUniformLocation(program_terrain, "gSpecularPower");

    EyePosLocation_terrain = glGetUniformLocation(program_terrain, "gEyeWorldPos");

	fog_mode_location = glGetUniformLocation(program_terrain, "fog_mode");

	sandbox_mode_location_terrain = glGetUniformLocation(program_terrain, "sandbox_mode");

	NumLightsLocation = glGetUniformLocation(program_terrain, "NumLights");


	glUniform3f(dirLightColorLocation, Light.Color.x, Light.Color.y, Light.Color.z);
	glUniform1f(dirLightAmbientIntensityLocation, Light.AmbientIntensity);
	glUniform1f(dirLightDiffuseIntensityLocation, Light.DiffuseIntensity);
	Vector3f Direction = Light.Direction;
	Direction.Normalize();

	glUniform3f(dirLightDirectionLocation, Direction.x, Direction.y, Direction.z);

	glUniform1f(MatSpecularIntensityLocation_terrain, MatSpecularIntensity);
	glUniform1f(SpecularPowerLocation_terrain, SpecularPower);

	glUniform1i(grassLocation,  17);
	glUniform1i(sandLocation,   18);
	glUniform1i(stoneLocation,  19);
	glUniform1i(snowLocation,   20);
	glUniform1i(waterLocation,  24);

	glUniform1i(NumLightsLocation, NumLights);
	

	glUseProgram(NULL);

	glUseProgram(program_water);
	gWVPLocation_water = glGetUniformLocation(program_water, "gWVP");
	gWorldLocation_water = glGetUniformLocation(program_water, "gWorld");
	AlphaFactorLocation = glGetUniformLocation(program_water, "AlphaFactor");
	ReflectionFactorLocation = glGetUniformLocation(program_water, "ReflectionFactor");

	dirLightColorLocation_water = glGetUniformLocation(program_water, "gDirectionalLight.Color");
	dirLightAmbientIntensityLocation_water = glGetUniformLocation(program_water, "gDirectionalLight.AmbientIntensity");
	dirLightDiffuseIntensityLocation_water = glGetUniformLocation(program_water, "gDirectionalLight.DiffuseIntensity");
	dirLightDirectionLocation_water = glGetUniformLocation(program_water, "gDirectionalLight.Direction");

	MatSpecularIntensityLocation = glGetUniformLocation(program_water, "gMatSpecularIntensity");

	SpecularPowerLocation = glGetUniformLocation(program_water, "gSpecularPower");

	EyePosLocation = glGetUniformLocation(program_water, "gEyeWorldPos");

	skybox_location_water = glGetUniformLocation(program_water, "skybox");

	sandbox_mode_location_water = glGetUniformLocation(program_water, "sandbox_mode");

	NumLightsLocationWater = glGetUniformLocation(program_water, "NumLights");



	for (int i = 0; i < NumLights; i++)
	{
		char name[128];
		memset(name, 0, sizeof(name));

		_snprintf(name, sizeof(name), "gPointLight[%d].Color", i);
		pointLightColorLocation_water[i] = glGetUniformLocation(program_water, name);

		_snprintf(name, sizeof(name), "gPointLight[%d].Position", i);
		pointLightPositionLocation_water[i] = glGetUniformLocation(program_water, name);

		_snprintf(name, sizeof(name), "gPointLight[%d].AmbientIntensity", i);
		pointLightAmbientIntensityLocation_water[i] = glGetUniformLocation(program_water, name);

		_snprintf(name, sizeof(name), "gPointLight[%d].DiffuseIntensity", i);
		pointLightDiffuseIntensityLocation_water[i] = glGetUniformLocation(program_water, name);

		_snprintf(name, sizeof(name), "gPointLight[%d].Constant", i);
		pointLightAttenConstantLocation_water[i] = glGetUniformLocation(program_water, name);

		_snprintf(name, sizeof(name), "gPointLight[%d].Linear", i);
		pointLightAttenLinearLocation_water[i] = glGetUniformLocation(program_water, name);

		_snprintf(name, sizeof(name), "gPointLight[%d].Exp", i);
		pointLightAttenExpLocation_water[i] = glGetUniformLocation(program_water, name);
	}

	glUniform3f(dirLightColorLocation_water, Light.Color.x, Light.Color.y, Light.Color.z);
	glUniform1f(dirLightAmbientIntensityLocation_water, Light.AmbientIntensity);
	glUniform1f(dirLightDiffuseIntensityLocation_water, Light.DiffuseIntensity);
	Direction.Normalize();
	glUniform3f(dirLightDirectionLocation_water, Direction.x, Direction.y, Direction.z);

	glUniform1f(MatSpecularIntensityLocation, MatSpecularIntensityWater);
	glUniform1f(SpecularPowerLocation, SpecularPowerWater);

	glUniform1i(skybox_location_water, 21);

	glUniform1i(NumLightsLocationWater, NumLights);

	glUseProgram(NULL);



}


void CreateVertexBuffer()
{	


	float dx_tex = 1.0 / (width-1);
	float dy_tex = 1.0 / (height-1);

	for (int i = 0; i < height ; i++)
		for (int j = 0; j < width; j++)
		{
			int idx = i * width + j;

			Vertices_terrain[idx].pos = Vector3f(-1.0 + j * dx , 1 - i * dy,  0.0);
			Vertices_terrain[idx].pos.z = g_HeightMap[idx] / 255.0 + 0.3 ;

			Vertices_terrain[idx].tex = Vector2f(j * dx_tex, 1 - i * dy_tex);
	
			Vertices_water[idx].pos = Vector3f(-1.0 + j * dx,1 - i * dy, 0.0 );
			Vertices_water[idx].pos.z =	 Vertices_terrain[idx].pos.z;

			Vertices_water[idx].tex = Vector2f(j * dx_tex, 1 - i * dy_tex);

			Vertices_lava[idx].pos = Vector3f(-1.0 + j * dx,1 - i * dy, 0.0 );
			Vertices_lava[idx].pos.z =	Vertices_terrain[idx].pos.z  ;

			Vertices_lava[idx].tex = Vector2f(j * dx_tex, 1 - i * dy_tex);

			Vertices_calculate_normals[idx].pos = Vector3f(-1.0 + j * dx , 1 - i * dy,  0.0);
			Vertices_calculate_normals[idx].tex = Vector2f(j * dx_tex, 1 - i * dy_tex);

		}

			
	dx_tex = 1.0 / (gridWidth - 1);
	dy_tex = 1.0 / (gridHeight - 1);

	float dx_calculate = 2.0 / (gridWidth - 1);
	float dy_calculate = 2.0 / (gridHeight - 1);

	for (int i = 0; i < gridHeight ; i++)
		for (int j = 0; j < gridWidth; j++)
		{
			int idx = i * gridWidth + j;
			Vertices_calculate[idx].pos = Vector3f(-1.0 + j * dx_calculate , 1 - i * dy_calculate, 0.0 );
	
			Vertices_calculate[idx].tex = Vector2f(j * dx_tex , 1 - i * dy_tex);
		}
	
	int ind = 0;

	for(int i = 0; i < height - 1; i++)
	{
		for(int j = 0; j < width - 1; j++)
		{
			Indices[ind] = chain(i, j, width);
			Indices[ind + 1] = chain(i, j + 1, width);
			Indices[ind + 2] = chain(i + 1, j + 1, width);
			Indices[ind + 3] = chain(i + 1, j + 1, width);
			Indices[ind + 4] = chain(i + 1, j, width);
			Indices[ind + 5] = chain(i, j , width);

			ind += 6;
		}
	}

	ind = 0;

	for(int i = 0; i < gridHeight - 1; ++i)
	{
		for(int j = 0; j < gridWidth - 1; ++j)
		{
			Indices_calculate[ind++] = chain(i, j, gridWidth);
			Indices_calculate[ind++] = chain(i, j + 1, gridWidth);
			Indices_calculate[ind++] = chain(i + 1, j + 1, gridWidth);
			Indices_calculate[ind++] = chain(i, j, gridWidth);
			Indices_calculate[ind++] = chain(i + 1, j, gridWidth);
			Indices_calculate[ind++] = chain(i + 1, j + 1, gridWidth);
		}
	}



	glGenBuffers(1, &VBO_water);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_water);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_water), 0, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices_water), Vertices_water);

	glGenBuffers(1, &VBO_calculate);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_calculate);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_calculate), Vertices_calculate, GL_STATIC_DRAW);

	glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	glGenBuffers(1, &IBO_calculate);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_calculate);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices_calculate), Indices_calculate, GL_STATIC_DRAW);


	glGenBuffers(1, &VBO_terrain);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_terrain);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_terrain), 0, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices_terrain), Vertices_terrain);
}



static void SpecialKeyboardCB(int Key, int x, int y)
{
    pGameCamera->OnKeyboard(Key);

}



static void KeyboardCB(unsigned char Key, int x, int y)
{
    switch (Key) {
        case 27:
			{
				exit(0);
			}

		case 'w':
			{
				water = true;
				bottom = false;
			
				break;
			}

		case 'b':
			{
				water = false;
				bottom = true;
			
				break;
			}

		case '=':
			{
				Light.DiffuseIntensity += 0.05;
				if (Light.DiffuseIntensity >= 1.0)
					Light.DiffuseIntensity = 1.0;
				break;
			}
		case '-':
			{
				Light.DiffuseIntensity -= 0.05;
				if (Light.DiffuseIntensity <= 0.0)
					Light.DiffuseIntensity = 0.0;
				break;
			}

		case 'i':
			{
				AlphaFactor += 1;
				break;
			}
		case 'o':
			{
				AlphaFactor -= 1;
				break;
			}
		case 'j':
			{
				ReflectionFactor += 0.05;
				break;
			}
		case 'k':
			{
				ReflectionFactor -= 0.05;
				break;
			}

		case'n':
			{
				Light.Direction.x += 0.01;
				break;
			}
		case 'm':
			{
				Light.Direction.x -= 0.01;
				break;
			}


			case 'r':
			{
				for (int i = 0; i < height ; i++)
					for (int j = 0; j < width ; j++)
					{
						int idx = i * width + j;
						Vertices_terrain[idx].pos.z = g_HeightMap[idx] / 255.0 + 0.5;
						Vertices_water[idx].pos.z = Vertices_terrain[idx].pos.z;
						Vertices_lava[idx].pos.z = Vertices_terrain[idx].pos.z;
					}

				glBindBuffer(GL_ARRAY_BUFFER, VBO_terrain);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices_terrain), Vertices_terrain);	

				glBindBuffer(GL_ARRAY_BUFFER, VBO_water);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices_water), Vertices_water);

				break;
			}

			case 'f':
			{
				if (f_pressed == false)
				{
					fog_mode = 1;
					f_pressed = true;
				}

				else
				{
					fog_mode = 0;
					f_pressed = false;
				}
				break;
			}

			case 's':
				{
					if (s_pressed == false)
					{
						sandbox_mode = 1;
						s_pressed = true;
					}

					else
					{
						sandbox_mode = 0;
						s_pressed = false;
					}
					break;
				}
			case '1':
				{
					if (one_pressed == false)
					{
						PointLight[0].DiffuseIntensity = 1.0;
						one_pressed = true;
					}

					else
					{
						PointLight[0].DiffuseIntensity = 0.0;
						one_pressed = false;
					}
					break;
				}
			case '2':
				{
					if (two_pressed == false)
					{
						PointLight[1].DiffuseIntensity = 1.0;
						two_pressed = true;
					}

					else
					{
						PointLight[1].DiffuseIntensity = 0.0;
						two_pressed = false;
					}
					break;
				}

			case '3':
				{
					if (three_pressed == false)
					{
						PointLight[2].DiffuseIntensity = 1.0;
						three_pressed = true;
					}

					else
					{
						PointLight[2].DiffuseIntensity = 0.0;
						three_pressed = false;
					}
					break;
				}

			case 'g':
				{
					initialize(grid, Vertices_water);
					waterflow = 1;
				}
			}
}


static void PassiveMouseCB(int x, int y)
{
    pGameCamera  ->OnMouse(x, y);

}


void MousePressed(int button, int state, int ax, int ay)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		{
			left_button_pressed = true;
			right_button_pressed = false;

			if (water)
			{
			float x = (float(ax) / WINDOW_WIDTH) * 2.0 - 1.0;
			float y = (float(ay) / WINDOW_HEIGHT) * 2.0 - 1.0;


			for (int i = 0; i < height ; i++)
				for (int j = 0; j < width ; j++)
					{
						int idx = i * width + j;


						if ( abs(Vertices_terrain[idx].pos.x - x) < 0.03 )
						{
							if ( abs(Vertices_terrain[idx].pos.y - y) < 0.03 )
							Vertices_water[idx].pos.z += 0.03;	
						
						}
						
					}		
			glBindBuffer(GL_ARRAY_BUFFER, VBO_water);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices_water), Vertices_water);	
			break;
			}

			else if (bottom)
			{

				float x = (float(ax) / WINDOW_WIDTH) * 2.0 - 1.0;
				float y = (float(ay) / WINDOW_HEIGHT) * 2.0 - 1.0;

				for (int i = 0; i < height ; i++)
					for (int j = 0; j < width ; j++)
					{
						int idx = i * width + j;

						float r = 0.2;

						float distance = sqrtf(abs(Vertices_terrain[idx].pos.x - x) * abs(Vertices_terrain[idx].pos.x - x) + abs(Vertices_terrain[idx].pos.y - y) * abs(Vertices_terrain[idx].pos.y - y) );
						
						if ( r >= distance)
						{
							float up_value = 0.015 * abs(distance - r);

							Vertices_terrain[idx].pos.z += up_value;
							Vertices_water[idx].pos.z += up_value;
						}
					}

			glBindBuffer(GL_ARRAY_BUFFER, VBO_terrain);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices_terrain), Vertices_terrain);	

			glBindBuffer(GL_ARRAY_BUFFER, VBO_water);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices_water), Vertices_water);	
			break;
			}
		}


		case GLUT_RIGHT_BUTTON:
		{		
			left_button_pressed = false;
			right_button_pressed = true;
			if (water)
			{
		
				float x = (float(ax) / WINDOW_WIDTH) * 2.0 - 1.0;
				float y = (float(ay) / WINDOW_HEIGHT) * 2.0 - 1.0;

				for (int i = 0; i < height ; i++)
					for (int j = 0; j < width ; j++)
					{
						int idx = i * width + j;
					
						if ( (abs(Vertices_water[idx].pos.x - x) < 0.15) && (abs(Vertices_water[idx].pos.y - y) < 0.15)  )
						{
								Vertices_water[idx].pos.z = Vertices_terrain[idx].pos.z;
						}
					}

			glBindBuffer(GL_ARRAY_BUFFER, VBO_water);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices_water), Vertices_water);	
			break;

			}

			else if (bottom)
			{
				float x = (float(ax) / WINDOW_WIDTH) * 2.0 - 1.0;
				float y = (float(ay) / WINDOW_HEIGHT) * 2.0 - 1.0;

				for (int i = 0; i < height ; i++)
					for (int j = 0; j < width ; j++)
					{
						int idx = i * width + j;

						float r = 0.2;

						float distance = sqrtf(abs(Vertices_terrain[idx].pos.x - x) * abs(Vertices_terrain[idx].pos.x - x) + abs(Vertices_terrain[idx].pos.y - y) * abs(Vertices_terrain[idx].pos.y - y) );
						
						if ( r >= distance)
						{
							float down_value = 0.025 * abs(distance - r);
							Vertices_terrain[idx].pos.z -= down_value;
							Vertices_water[idx].pos.z -= down_value;
						}
					}

			glBindBuffer(GL_ARRAY_BUFFER, VBO_terrain);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices_terrain), Vertices_terrain);	

			glBindBuffer(GL_ARRAY_BUFFER, VBO_water);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices_water), Vertices_water);


			break;
		}

	}

				}
}

void MousePressedMove(int ax, int ay)
{
	if (left_button_pressed)
	{
	if (water)
	{
		float x = (float(ax) / WINDOW_WIDTH) * 2.0 - 1.0;
		float y = (float(ay) / WINDOW_HEIGHT) * 2.0 - 1.0;

		for (int i = 0; i < height ; i++)
			for (int j = 0; j < width ; j++)
			{
				int idx = i * width + j;
				
				if ( abs(Vertices_terrain[idx].pos.x - x) < 0.03 )
				{
					if ( abs(Vertices_terrain[idx].pos.y - y) < 0.03 )
						Vertices_water[idx].pos.z += 0.03;	
				}
			}

			glBindBuffer(GL_ARRAY_BUFFER, VBO_water);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices_water), Vertices_water);	
	}

	else if (bottom)
	{
				float x = (float(ax) / WINDOW_WIDTH) * 2.0 - 1.0;
				float y = (float(ay) / WINDOW_HEIGHT) * 2.0 - 1.0;

				for (int i = 0; i < height ; i++)
					for (int j = 0; j < width ; j++)
					{
						int idx = i * width + j;

						float r = 0.2;

						float distance = sqrtf(abs(Vertices_terrain[idx].pos.x - x) * abs(Vertices_terrain[idx].pos.x - x) + abs(Vertices_terrain[idx].pos.y - y) * abs(Vertices_terrain[idx].pos.y - y) );
						
						if ( r >= distance)
						{
							float up_value = 0.015 * abs(distance - r);

							Vertices_terrain[idx].pos.z += up_value;
							Vertices_water[idx].pos.z += up_value;
						}
					}

			glBindBuffer(GL_ARRAY_BUFFER, VBO_terrain);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices_terrain), Vertices_terrain);	

			glBindBuffer(GL_ARRAY_BUFFER, VBO_water);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices_water), Vertices_water);		
	}
	}

	else if (right_button_pressed)
	{
	if (water)
	{
		float x = (float(ax) / WINDOW_WIDTH) * 2.0 - 1.0;
		float y = (float(ay) / WINDOW_HEIGHT) * 2.0 - 1.0;

			
				for (int i = 0; i < height ; i++)
					for (int j = 0; j < width ; j++)
					{
						int idx = i * width + j;
					
						if ( (abs(Vertices_water[idx].pos.x - x) < 0.15) && (abs(Vertices_water[idx].pos.y - y) < 0.15)  )
						{
								Vertices_water[idx].pos.z = Vertices_terrain[idx].pos.z;
						}
					}
			

			glBindBuffer(GL_ARRAY_BUFFER, VBO_water);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices_water), Vertices_water);
	}

	else if (bottom)
	{
				float x = (float(ax) / WINDOW_WIDTH) * 2.0 - 1.0;
				float y = (float(ay) / WINDOW_HEIGHT) * 2.0 - 1.0;

				for (int i = 0; i < height ; i++)
					for (int j = 0; j < width ; j++)
					{
						int idx = i * width + j;

						float r = 0.2;

						float distance = sqrtf(abs(Vertices_terrain[idx].pos.x - x) * abs(Vertices_terrain[idx].pos.x - x) + abs(Vertices_terrain[idx].pos.y - y) * abs(Vertices_terrain[idx].pos.y - y) );
						
						if ( r >= distance)
						{
							float down_value = 0.025 * abs(distance - r);
							Vertices_terrain[idx].pos.z -= down_value;
							Vertices_water[idx].pos.z -= down_value;
						}
					}

			glBindBuffer(GL_ARRAY_BUFFER, VBO_terrain);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices_terrain), Vertices_terrain);	

			glBindBuffer(GL_ARRAY_BUFFER, VBO_water);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices_water), Vertices_water);	
	}
	}
}


void InitializeGlutCallbacks()
{

    glutDisplayFunc(RenderScene);
    glutIdleFunc(RenderScene);
	
	glutSpecialFunc(SpecialKeyboardCB);
    glutPassiveMotionFunc(PassiveMouseCB);
    glutKeyboardFunc(KeyboardCB);
	glutMouseFunc(MousePressed);
	glutMotionFunc(MousePressedMove);


}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);	
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Sandbox");
	InitializeGlutCallbacks();
	glEnable(GL_DEPTH_TEST);
	//glutFullScreen();

	pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, pos, target, up);
	

    GLenum res = glewInit();
    if (res != GLEW_OK) {
      fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
      return 1;
    }

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);


	LoadRawFile("map3.raw", width * height, g_HeightMap);
	
	CreateVertexBuffer();

	CreateFrameBuffer();
	CreateShallowTextures();
	LoadTextures();
	BindTextures();

	CreateSkyboxBuffer();
	CreateSkybox(GL_TEXTURE21, skybox_ind, "sea/right.bmp", "sea/left.bmp", "sea/front.bmp","sea/back.bmp","sea/bottom.bmp", "sea/top.bmp");

	SetLighting();
	



	CreateShaderProgram("vertex_terrain.vert", "fragment_terrain.frag", vs_terrain, fs_terrain, program_terrain);
	CreateShaderProgram("vertex_water.vert", "fragment_water.frag", vs_water, fs_water, program_water);
	CreateShaderProgram("compute_values.vert", "compute_values.frag",vs_values, fs_values, program_values);
	CreateShaderProgram("compute_eigen.vert", "compute_eigen.frag", vs_eigen, fs_eigen, program_eigen);
	CreateShaderProgram("compute_fluxes.vert", "compute_fluxes.frag", vs_fluxes, fs_fluxes, program_fluxes);
	CreateShaderProgram("runge_kutta.vert", "runge_kutta.frag", vs_runge, fs_runge, program_runge);
	CreateShaderProgram("skybox.vert", "skybox.frag", vs_skybox, fs_skybox, program_skybox);

	SetUniforms();

	cout << endl << endl;
	cout << " +/- Increase/Decrease lighting" << endl;
	cout << " I/O Increase/Decrease Alpha Factor" << endl;
	cout << " J/K Increase/Decrease Blending Factor" << endl;
	cout << " W/B/L Set left/right mouse button to raise/put down water/bottom/liquid" << endl;
	cout << " N/M Move light source" << endl;
	cout << " Z/X Enable/Disable second liquid simulation" << endl;
	cout << " R Reset to origin" << endl;
	cout << " S Enable/disable sandbox mode" << endl;
	cout << " 1,2,3 Enable/disable point light sources" << endl;

    glutMainLoop();

    return 0;
}