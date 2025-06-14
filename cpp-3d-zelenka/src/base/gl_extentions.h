#ifndef GL_EXTENTIONS_H_
#define GL_EXTENTIONS_H_

#if _WIN32 || _WIN64
    #include <windows.h>
    #include <GL/gl.h>
    #include "GL_ext/glext.h"
    #define glGetProcAddress(a) wglGetProcAddress(a)
#else
    #include <GL/gl.h>
    #include <GL/glx.h>
    #include "GL_ext/glext.h"
    #define glGetProcAddress(a) glXGetProcAddress(reinterpret_cast<const unsigned char*>(a))
#endif

#include <fstream>

// расширения OpenGL
// Texture
#if _WIN32 || _WIN64
extern PFNGLACTIVETEXTUREPROC glActiveTexture;
#endif
// VAO
extern PFNGLGENVERTEXARRAYSPROC    glGenVertexArrays;
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC    glBindVertexArray;
// VBO
extern PFNGLGENBUFFERSPROC              glGenBuffers;
extern PFNGLDELETEBUFFERSPROC           glDeleteBuffers;
extern PFNGLBINDBUFFERPROC              glBindBuffer;
extern PFNGLBUFFERDATAPROC              glBufferData;
extern PFNGLBUFFERSUBDATAPROC           glBufferSubData;
extern PFNGLMAPBUFFERPROC               glMapBuffer;
extern PFNGLUNMAPBUFFERPROC             glUnmapBuffer;
extern PFNGLISBUFFERPROC                glIsBuffer;
extern PFNGLGETBUFFERSUBDATAPROC        glGetBufferSubData;
extern PFNGLGETBUFFERPARAMETERIVPROC    glGetBufferParameteriv;
extern PFNGLGETBUFFERPOINTERVPROC       glGetBufferPointerv;
extern PFNGLMULTIDRAWELEMENTSPROC       glMultiDrawElements;

extern PFNGLBINDBUFFERBASEPROC          glBindBufferBase;

// Shaders
extern PFNGLCREATEPROGRAMPROC     glCreateProgram;
extern PFNGLDELETEPROGRAMPROC     glDeleteProgram;
extern PFNGLLINKPROGRAMPROC       glLinkProgram;
extern PFNGLVALIDATEPROGRAMPROC   glValidateProgram;
extern PFNGLUSEPROGRAMPROC        glUseProgram;
extern PFNGLGETPROGRAMIVPROC      glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern PFNGLCREATESHADERPROC      glCreateShader;
extern PFNGLDELETESHADERPROC      glDeleteShader;
extern PFNGLSHADERSOURCEPROC      glShaderSource;
extern PFNGLCOMPILESHADERPROC     glCompileShader;
extern PFNGLATTACHSHADERPROC      glAttachShader;
extern PFNGLDETACHSHADERPROC      glDetachShader;
extern PFNGLGETSHADERIVPROC       glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC  glGetShaderInfoLog;

// Shaders Compute
extern PFNGLDISPATCHCOMPUTEPROC glDispatchCompute;
extern PFNGLMEMORYBARRIERPROC	glMemoryBarrier;

// Attributes
extern PFNGLGETATTRIBLOCATIONPROC        glGetAttribLocation;
extern PFNGLVERTEXATTRIBPOINTERPROC      glVertexAttribPointer;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC  glEnableVertexAttribArray;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
// Uniforms
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLUNIFORMMATRIX3FVPROC   glUniformMatrix3fv;
extern PFNGLUNIFORMMATRIX4FVPROC   glUniformMatrix4fv;
extern PFNGLUNIFORM1IPROC          glUniform1i;
extern PFNGLUNIFORM1FPROC          glUniform1f;
extern PFNGLUNIFORM1FVPROC         glUniform1fv;
extern PFNGLUNIFORM3FVPROC         glUniform3fv;
extern PFNGLUNIFORM4FVPROC         glUniform4fv;
// FBO
extern PFNGLBINDFRAMEBUFFERPROC        glBindFramebuffer;
extern PFNGLDELETEFRAMEBUFFERSPROC     glDeleteFramebuffers;
extern PFNGLGENFRAMEBUFFERSPROC        glGenFramebuffers;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
extern PFNGLFRAMEBUFFERTEXTUREPROC     glFramebufferTexture;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC     glFramebufferTexture2D;

namespace ag
{
	bool opengl_extension_init(void);
	bool is_opengl_extension_load(void);
    void opengl_contex_info(std::ofstream& file);
}
#endif //GL_EXTENTIONS_H_
