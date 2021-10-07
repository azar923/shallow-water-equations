#include "texture.h"
#include "camera.h"
#include <GL\glew.h>

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

Pipeline p;
Camera* pGameCamera = NULL;


const int width = 320;
const int height = 240;

const int gridWidth = width + 2;
const int gridHeight = height + 2;

float  B_prev[width * height];

struct DirectionalLight
{
	Vector3f Color;
	float AmbientIntensity;
	Vector3f Direction;
	float DiffuseIntensity;
};

struct PointLight
{
	Vector3f Color;
	Vector3f Position;
	float AmbientIntensity;
	float DiffuseIntensity;
	
	float constant;
	float linear;
	float exp;
};

Texture* grass = 0;
Texture* sand = 0;
Texture* stone = 0;
Texture* snow = 0;
Texture* water_tex = 0;
Texture* smoke = 0;


float AlphaFactor = 25.0;
GLuint AlphaFactorLocation;
GLuint AlphaFactorLocation_lava;

bool			isFilterZero = 1;
bool			isFilterGradient = 0;
bool			isFilterMedian = 1;
bool			isFilterFMM = 1;

int waterflow = 0;

const int		_d_const_max_depth = 255;
bool			key_states[256];


unsigned char	raw[width * height];
unsigned char	data[width * height];
unsigned char	data_delta = 1;
unsigned char	history_scan_depth = 7;
unsigned char	radius_median = 8;
	
const int		_r_cycle_count = 1000;
int				_r_cycle = 0;
unsigned int	_r_max_value = 0;
unsigned char	_rdata[width * height * _r_cycle_count];

float ReflectionFactor = 0.5;

float time_melting = 0.0;
GLuint time_melting_location;
int Melting = 0;
GLuint melting_location;
GLuint ReflectionFactorLocation;
GLuint ReflectionFactorLocation_lava;

GLuint grassLocation;
GLuint sandLocation;
GLuint stoneLocation;
GLuint snowLocation;
GLuint waterLocation;

GLuint VBO_water;
GLuint VBO_terrain;
GLuint VBO_skybox;
GLuint VBO_calculate;
GLuint VBO_lava;
GLuint VBO_mesh;
GLuint VBO_particles;
GLuint VBO_calculate_normals;

GLuint IBO;
GLuint IBO_calculate;
GLuint IBO_skybox;

GLuint FBO;
GLuint FBO_water;
GLuint FBO_shadow;
GLuint FBO_normals;



GLuint reflection_ind;
GLuint reflection_location;
GLuint reflection_location_lava;
GLuint reflection_location_mesh;

GLuint stepLocation;

GLuint bottom_ind;
GLuint values_ind;
GLuint values_lava_ind;

GLuint values_location;
GLuint values_lava_location;

GLuint values_location_fluxes;
GLuint values_location_runge;
GLuint values_lava_location_fluxes;
GLuint values_lava_location_runge;

GLuint bottom_location;
GLuint bottom_location_fluxes;
GLuint bottom_location_runge;


GLuint u_location;
GLuint u_lava_location;

GLuint v_location;
GLuint v_lava_location;

GLuint h_location;
GLuint h_lava_location;


GLuint H_x_location;
GLuint H_x_lava_loation;

GLuint H_y_location;
GLuint H_y_lava_location;

GLuint source_location;
GLuint source_lava_location;

GLuint U_prev_ind;
GLuint U_prev_lava_ind;

GLuint U_prev_location;
GLuint U_prev_lava_location;


GLuint U_e_location_eigen;
GLuint U_e_location_fluxes;

GLuint U_e_lava_location_eigen;
GLuint U_e_lava_location_fluxes;

GLuint U_w_location_eigen;
GLuint U_w_location_fluxes;

GLuint U_w_lava_location_eigen;
GLuint U_w_lava_location_fluxes;

GLuint U_s_location_eigen;
GLuint U_s_location_fluxes;

GLuint U_s_lava_location_eigen;
GLuint U_s_lava_location_fluxes;

GLuint U_n_location_eigen;
GLuint U_n_location_fluxes;

GLuint U_n_lava_location_eigen;
GLuint U_n_lava_location_fluxes;

GLuint speeds_location;
GLuint speeds_lava_location;


GLuint render0_ind;
GLuint render1_ind;
GLuint render2_ind;
GLuint render3_ind;
GLuint render4_ind;
GLuint render5_ind;
GLuint render6_ind;




GLuint timeLocation;

GLuint skybox_location;
GLuint skybox_location_water;
GLuint skybox_location_lava;
GLuint skybox_ind;



BYTE g_HeightMap[width * height];

unsigned int vs_terrain, fs_terrain, program_terrain;

unsigned int vs_water, fs_water, program_water;

unsigned int vs_lava, fs_lava, program_lava;

unsigned int vs_values, fs_values, program_values;

unsigned int vs_eigen, fs_eigen, program_eigen;

unsigned int vs_fluxes, fs_fluxes, program_fluxes;

unsigned int vs_runge, fs_runge, program_runge;

unsigned int vs_skybox, fs_skybox, program_skybox;

unsigned int vs_mesh, fs_mesh, program_mesh;

unsigned int vs_particles, fs_particles, program_particles;

unsigned int vs_normals_pass_one, fs_normals_pass_one, program_normals_pass_one;

struct Point
{
	Vector3f U;  
	
	float B;
	float B_w;
	float B_e;
	float B_s;
	float B_n;
};

Point grid[gridWidth * gridHeight];
Point grid_lava[gridWidth * gridHeight];


float dt = 0.0;
float dt_lava = 0.0;
float g = 9.8;
float teta = 1.3;

float dx = 2.0 / (width-1);
float dy = 2.0 / (height-1);

struct Vertex_water
{
	Vector3f pos;
	Vector3f normal;
	Vector2f tex;
	float bottom_pos;
	float lava_pos;
	float bottom_prev;
};

struct Vertex_lava
{
	Vector3f pos;
	Vector3f normal;
	Vector2f tex;
	float bottom_pos;
	float water_pos;
};

struct Vertex_terrain
{
	Vector3f pos;
	Vector3f normal;
	Vector2f tex;

};

struct Vertex_calculate
{
	Vector3f pos;
	Vector2f tex;
};

struct Vertex_mesh
{
	Vector3f pos;
	Vector2f tex;
};

struct Vertex_particle
{
	Vector3f InitialVelocity;
	float StartTime;
};

GLuint dirLightColorLocation;
GLuint dirLightAmbientIntensityLocation;
GLuint dirLightDirectionLocation;
GLuint dirLightDiffuseIntensityLocation;


GLuint dirLightColorLocation_water;
GLuint dirLightAmbientIntensityLocation_water;
GLuint dirLightDirectionLocation_water;
GLuint dirLightDiffuseIntensityLocation_water;

GLuint dirLightColorLocation_lava;
GLuint dirLightAmbientIntensityLocation_lava;
GLuint dirLightDirectionLocation_lava;
GLuint dirLightDiffuseIntensityLocation_lava;

PointLight PointLight[3];
DirectionalLight Light;


int sandbox_mode = 0;
GLuint sandbox_mode_location_terrain;
GLuint sandbox_mode_location_water;

int fog_mode = 0;
GLuint fog_mode_location;

float MatSpecularIntensity = 0.0;
float MatSpecularIntensityWater = 1.0;
GLuint MatSpecularIntensityLocation;
GLuint MatSpecularIntensityLocation_terrain;
GLuint MatSpecularIntensityLocation_lava;

float SpecularPower = 16.0;
float SpecularPowerWater = 16.0;

GLuint SpecularPowerLocation;
GLuint SpecularPowerLocation_terrain;
GLuint SpecularPowerLocation_lava;

GLuint EyePosLocation;
GLuint EyePosLocation_terrain;
GLuint EyePosLocation_lava;



Vertex_water Vertices_water[width * height];
Vertex_lava Vertices_lava[width * height];
Vertex_terrain Vertices_terrain[width * height];
Vertex_calculate Vertices_calculate[gridWidth * gridHeight];
Vertex_mesh Vertices_mesh[4];

Vertex_calculate Vertices_calculate_normals[width * height];


unsigned int Indices[(width - 1) * (height - 1) * 6];
unsigned int Indices_calculate[(gridWidth - 1) * (gridHeight - 1) * 6];

Vector3f Normals[gridWidth * gridHeight];
GLuint gWVPLocation;
GLuint gWVPLocation_water;
GLuint gWVPLocation_skybox;
GLuint gWVPLocation_lava;
GLuint gWVPLocation_particles;



GLuint gWorldLocation;
GLuint gWorldLocation_water;
GLuint gWorldLocation_lava;
GLuint gWorldLocation_particles;


Vector3f	pos = Vector3f(-100.0f, 300.0f, -50.0f);
Vector3f	target = Vector3f(0.0f, -0.6f, 0.4f);
Vector3f	up = Vector3f(0.0f, 1.0f, 0.0f);

Vector3f reflection_pos = Vector3f(-100.0f, 350.0f, 100.0f);
Vector3f reflection_target = Vector3f(0.0f, -0.95f, 0.05f);
Vector3f reflection_up = Vector3f(0.0f, 1.0f, 0.0f);


bool water = true;
bool bottom = false;
bool lava = false;

bool second_liquid = false;

bool left_button_pressed = false;
bool right_button_pressed = false;



const int NumLights = 3;
GLuint NumLightsLocation;
GLuint NumLightsLocationWater;

Vector3f PointLightPositions[NumLights] = { Vector3f(-0.5,-0.5, 2.0), Vector3f(0.5, 0.5, 2.0), Vector3f(1.0, -1.0, 2.0) };
Vector3f PointLightColors[NumLights] = { Vector3f(1.0, 0.0, 0.0), Vector3f(0.0, 1.0, 0.0), Vector3f(0.0, 0.0, 1.0) };

GLuint pointLightColorLocation[NumLights];
GLuint pointLightPositionLocation[NumLights];
GLuint pointLightAmbientIntensityLocation[NumLights];
GLuint pointLightDiffuseIntensityLocation[NumLights];
GLuint pointLightAttenConstantLocation[NumLights];
GLuint pointLightAttenLinearLocation[NumLights];
GLuint pointLightAttenExpLocation[NumLights];


GLuint pointLightColorLocation_water[NumLights];
GLuint pointLightPositionLocation_water[NumLights];
GLuint pointLightAmbientIntensityLocation_water[NumLights];
GLuint pointLightDiffuseIntensityLocation_water[NumLights];
GLuint pointLightAttenConstantLocation_water[NumLights];
GLuint pointLightAttenLinearLocation_water[NumLights];
GLuint pointLightAttenExpLocation_water[NumLights];

GLuint refraction_ind;
GLuint refraction_location;

//Keys

bool f_pressed = false;
bool s_pressed = false;
bool one_pressed = false;
bool two_pressed = false;
bool three_pressed = false;

float scale;


//for particles

float LifeTime = 50.0;
float time_particles = 0.0;
Vector3f gravity = Vector3f(0.0, 0.0, 9.8);
Vector3f InitialPosition = Vector3f(0.0, 0.0, 0.0);

GLuint InitialPositionLocation;
GLuint LifeTimeLocation;
GLuint time_particles_location;
GLuint gravity_location;

GLuint feedback_A;
GLuint feedback_B;

GLuint VBO_particles_A;
GLuint VBO_particles_B;

GLuint smoke_location;

GLuint normals_terrain_ind;
GLuint normals_water_ind;
GLuint normals_terrain_location;
GLuint normals_water_location;

GLuint sampler_location;


GLuint normals_for_terrain;
GLuint normals_for_terrain_location;