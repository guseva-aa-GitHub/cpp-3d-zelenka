#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <string>
#include "gl_extentions.h"

GLuint get_textureID(const std::string& name);
GLuint get_textureDepthID(GLsizei width, GLsizei height);

#endif // TEXTURE_H_INCLUDED
