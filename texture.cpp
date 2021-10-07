#include <iostream>
#include "texture.h"


Texture::Texture(GLenum TextureTarget, char* FileName)
{
    m_textureTarget = TextureTarget;
    m_fileName      = FileName;
}

Texture::Texture(GLenum TextureTarget)
{
	m_textureTarget = TextureTarget;
}



bool Texture::Load()
{
	unsigned char header[54];                // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;                    // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;                  // = width*height*3
									
	unsigned char * data;                    // Actual RGB data

	FILE * file = fopen(m_fileName,"rb");

	if (!file)                             
		{
			printf("Image could not be opened\n");
			printf(m_fileName);
			return 0;
		}

	if ( fread(header, 1, 54, file)!=54 )
		{   
			printf("Not a correct BMP file\n");
			return false;
		}

	if ( header[0]!='B' || header[1]!='M' )
		{
			printf("Not a correct BMP file\n");
			return 0;
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


    glGenTextures(1, &m_textureObj);
    glBindTexture(m_textureTarget, m_textureObj);
    glTexImage2D(m_textureTarget, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	
	glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}



void Texture::Bind(GLenum TextureUnit)
{
    glActiveTexture(TextureUnit);
    glBindTexture(m_textureTarget, m_textureObj);
}


