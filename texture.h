#ifndef TEXTURE_H
#define	TEXTURE_H

#include <string>

#include <GL/glew.h>

class Texture
{
public:
    Texture(GLenum TextureTarget, char* FileName);
	Texture(GLenum TextureTarget);


    bool Load();

    void Bind(GLenum TextureUnit);

	GLuint m_textureObj;


private:
    char* m_fileName;
    GLenum m_textureTarget;
   

};


#endif	

