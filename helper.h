#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <time.h>

#include <fstream>

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

void loadFile(const char* fn, std::string& str);
void LoadRawFile (LPSTR strName, int nSize, BYTE *pHeightMap);

float max_3(float a, float b, float c);
float min_3(float a, float b, float c);

float max_2 (float a, float b);
float min_2 (float a, float b);

float max_4(float a, float b, float c, float d);

float max_elem(float* a, int length);
float min_elem(float* a, int length);

Vector3f minmod(float teta, float delta, Vector3f prev, Vector3f curr, Vector3f fut);

unsigned int chain(unsigned int i, unsigned int j, const int width);

void CreateTexture(GLenum unit, GLuint& pointer);
void CreateRenderTexture(GLuint& pointer, GLenum unit, GLenum ColorAttachment, const int width, const int height);

void CreateShaderProgram(const char* vname,const char* fname, unsigned int& v, unsigned int& f, unsigned int& p);
unsigned int loadShader(std::string& source,unsigned int mode);

void CreateSkybox(GLenum unit, GLuint& pointer, char* right, char* left, char* front, char* back, char* bottom, char* top);

Vector2f offsetTopLeft;
Vector2f offsetTopRight;
Vector2f offsetDownRight;
Vector2f offsetDownLeft;