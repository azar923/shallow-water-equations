#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

#include <fstream>

#include "math_3d.h"

void loadFile(const char* fn, std::string& str)                                         
{
	std::ifstream in(fn);
    if(!in.is_open())
    {
		std::cout << "The file " << fn << " cannot be opened\n";
        return;
    }
	else
		std::cout << "The file " << fn << " opened successfully" << std::endl;
        char tmp[300];

    while(!in.eof())
    {
		in.getline(tmp,300);
        str+=tmp;
        str+='\n';
    }
}

void LoadRawFile (LPSTR strName, int nSize, BYTE *pHeightMap)
{
	FILE *pFile = NULL;
	pFile = fopen( strName, "rb" );
	fread( pHeightMap, 1, nSize, pFile );
	fclose(pFile);
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

float min_2 (float a, float b)
{
	if (a < b)
		return a;
	else if (a > b)
		return b;
	else
		return a;
}

float max_3(float a, float b, float c)
{
	float array[3] = {a, b, c};

	float maximum = array[0];

	for (int i = 0; i < 3; i++)
		if (array[i] > maximum)
			maximum = array[i];

	return maximum;
}

float max_4(float a, float b, float c, float d)
{
	float array[4] = {a, b, c, d};
	
	float maximum = array[0];

	for (int i = 0; i < 4; i++)
		if (array[i] > maximum)
			maximum = array[i];

	return maximum;
}

float min_3(float a, float b, float c)
{
	float array[3] = {a, b, c};
	float min = array[0];

		for (int i = 0; i < 3; i++)
		if (array[i] < min)
			min = array[i];

	return min;
}


float max_elem(float *a, int length)
{
	float maximum = a[0];
	for (int i = 1; i < length; i++)
	{
		if (a[i] > maximum)
			maximum = a[i];
	}
	return maximum;
}

float min_elem(float *a, int length)
{
	float min = a[0];
	for (int i = 0; i < length; i++)
	{
		if (a[i] < min)
			min = a[i];
	}
	return min;
}



Vector3f minmod(float teta, float delta, Vector3f prev, Vector3f curr, Vector3f fut)
{
	Vector3f result;


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

unsigned int chain(unsigned int i, unsigned int j, const int width)
{
	return i * width + j;
}

void CreateTexture(GLenum unit, GLuint& pointer)
{
	glActiveTexture(unit);
	glGenTextures(1, &pointer);
	glBindTexture(GL_TEXTURE_2D, pointer);	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void CreateRenderTexture(GLuint& pointer, GLenum unit, GLenum ColorAttachment, const int width, const int height)
{

	glGenTextures(1, &pointer);
	glActiveTexture(unit);
	glBindTexture(GL_TEXTURE_2D, pointer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, ColorAttachment, GL_TEXTURE_2D, pointer, 0);

}

unsigned int loadShader(std::string& source,unsigned int mode)
{
	unsigned int id;
    id=glCreateShader(mode);
    const char* csource=source.c_str();
    glShaderSource(id,1,&csource,NULL);
    glCompileShader(id);
    char error[1000];
    glGetShaderInfoLog(id,1000,NULL,error);
    std::cout << "Compile status: \n" << error << std::endl;
    return id;
}


void CreateShaderProgram(const char* vname,const char* fname, unsigned int& v, unsigned int& f, unsigned int& p)
{	
    std::string source;
    loadFile(vname,source);
    v=loadShader(source,GL_VERTEX_SHADER);
    source="";
    loadFile(fname,source);
    f=loadShader(source,GL_FRAGMENT_SHADER);
       
    p=glCreateProgram();
    glAttachShader(p,v);
    glAttachShader(p,f);

	glLinkProgram(p);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glGetProgramiv(p, GL_LINK_STATUS, &Success);
	if (Success == 0)
		{
	glGetProgramInfoLog(p, sizeof(ErrorLog), NULL, ErrorLog);
	fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
	//exit(1);
		}
	else
		std::cout << "Shader program linked successfully" << "\n";

	glValidateProgram(p);
	glGetProgramiv(p, GL_VALIDATE_STATUS, &Success);
	if (!Success) 
		{
	glGetProgramInfoLog(p, sizeof(ErrorLog), NULL, ErrorLog);
	fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
	//exit(1);
		}
	else
		std::cout << "Shader program validated successfully" << std::endl;

}

void CreateSkybox(GLenum unit, GLuint& pointer, char* right, char* left, char* front, char* back, char* bottom, char* top)
{
	glActiveTexture(unit);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glGenTextures(1, &pointer);
	glBindTexture(GL_TEXTURE_CUBE_MAP, pointer);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	unsigned char header[54];                // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;                    // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;                  // = width*height*3
									
	unsigned char * data;                    // Actual RGB data

	FILE * file = fopen(right,"rb");

	if (!file)                             
		{
			printf("Image could not be opened\n");
			printf(right);
		
		}

	if ( fread(header, 1, 54, file)!=54 )
		{   
			printf("Not a correct BMP file\n");
		
		}

	if ( header[0]!='B' || header[1]!='M' )
		{
			printf("Not a correct BMP file\n");
		
		}

	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);

	if (imageSize==0)    imageSize=width*height*3;		// 3 : one byte for each Red, Green and Blue component
	if (dataPos==0)      dataPos=54;					// The BMP header is done that way

	data = new unsigned char[imageSize];
 

	fread(data,1,imageSize,file);
	fclose(file);


	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);


	 file = fopen(left,"rb");

	if (!file)                             
		{
			printf("Image could not be opened\n");
			printf(left);
		
		}

	if ( fread(header, 1, 54, file)!=54 )
		{   
			printf("Not a correct BMP file\n");
		
		}

	if ( header[0]!='B' || header[1]!='M' )
		{
			printf("Not a correct BMP file\n");
		
		}

	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);

	if (imageSize==0)    imageSize=width*height*3;		// 3 : one byte for each Red, Green and Blue component
	if (dataPos==0)      dataPos=54;					// The BMP header is done that way

	data = new unsigned char[imageSize];
 

	fread(data,1,imageSize,file);
	fclose(file);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);


	
	 file = fopen(front,"rb");

	if (!file)                             
		{
			printf("Image could not be opened\n");
			printf(front);
		
		}

	if ( fread(header, 1, 54, file)!=54 )
		{   
			printf("Not a correct BMP file\n");
		
		}

	if ( header[0]!='B' || header[1]!='M' )
		{
			printf("Not a correct BMP file\n");
		
		}

	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);

	if (imageSize==0)    imageSize=width*height*3;		// 3 : one byte for each Red, Green and Blue component
	if (dataPos==0)      dataPos=54;					// The BMP header is done that way

	data = new unsigned char[imageSize];
 

	fread(data,1,imageSize,file);
	fclose(file);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	 file = fopen(back,"rb");

	if (!file)                             
		{
			printf("Image could not be opened\n");
			printf("top.bmp");
		
		}

	if ( fread(header, 1, 54, file)!=54 )
		{   
			printf("Not a correct BMP file\n");
		
		}

	if ( header[0]!='B' || header[1]!='M' )
		{
			printf("Not a correct BMP file\n");
		
		}

	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);

	if (imageSize==0)    imageSize=width*height*3;		// 3 : one byte for each Red, Green and Blue component
	if (dataPos==0)      dataPos=54;					// The BMP header is done that way

	data = new unsigned char[imageSize];
 

	fread(data,1,imageSize,file);
	fclose(file);


	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);


	 file = fopen(bottom,"rb");

	if (!file)                             
		{
			printf("Image could not be opened\n");
			printf("top.bmp");
		
		}

	if ( fread(header, 1, 54, file)!=54 )
		{   
			printf("Not a correct BMP file\n");
		
		}

	if ( header[0]!='B' || header[1]!='M' )
		{
			printf("Not a correct BMP file\n");
		
		}

	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);

	if (imageSize==0)    imageSize=width*height*3;		// 3 : one byte for each Red, Green and Blue component
	if (dataPos==0)      dataPos=54;					// The BMP header is done that way

	data = new unsigned char[imageSize];
 

	fread(data,1,imageSize,file);
	fclose(file);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	 file = fopen(top,"rb");

	if (!file)                             
		{
			printf("Image could not be opened\n");
			printf("top.bmp");
		
		}

	if ( fread(header, 1, 54, file)!=54 )
		{   
			printf("Not a correct BMP file\n");
		
		}

	if ( header[0]!='B' || header[1]!='M' )
		{
			printf("Not a correct BMP file\n");
		
		}

	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);

	if (imageSize==0)    imageSize=width*height*3;		// 3 : one byte for each Red, Green and Blue component
	if (dataPos==0)      dataPos=54;					// The BMP header is done that way

	data = new unsigned char[imageSize];
 

	fread(data,1,imageSize,file);
	fclose(file);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

}