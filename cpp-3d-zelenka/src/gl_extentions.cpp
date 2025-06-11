#include "gl_extentions.h"

extern std::ofstream log_file;

// расширения OpenGL
// Texture
#if _WIN32 || _WIN64
PFNGLACTIVETEXTUREPROC glActiveTexture = NULL;
#endif
// VAO
PFNGLGENVERTEXARRAYSPROC    glGenVertexArrays    = NULL;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = NULL;
PFNGLBINDVERTEXARRAYPROC    glBindVertexArray    = NULL;
// VBO
PFNGLGENBUFFERSPROC             glGenBuffers           = NULL;
PFNGLDELETEBUFFERSPROC          glDeleteBuffers        = NULL;
PFNGLBINDBUFFERPROC             glBindBuffer           = NULL;
PFNGLBUFFERDATAPROC             glBufferData           = NULL;
PFNGLBUFFERSUBDATAPROC          glBufferSubData        = NULL;
PFNGLMAPBUFFERPROC              glMapBuffer            = NULL;
PFNGLUNMAPBUFFERPROC            glUnmapBuffer          = NULL;
PFNGLISBUFFERPROC               glIsBuffer             = NULL;
PFNGLGETBUFFERSUBDATAPROC       glGetBufferSubData     = NULL;
PFNGLGETBUFFERPARAMETERIVPROC   glGetBufferParameteriv = NULL;
PFNGLGETBUFFERPOINTERVPROC      glGetBufferPointerv    = NULL;
PFNGLMULTIDRAWELEMENTSPROC      glMultiDrawElements    = NULL;

PFNGLBINDBUFFERBASEPROC         glBindBufferBase       = NULL;

// Shaders
PFNGLCREATEPROGRAMPROC     glCreateProgram     = NULL;
PFNGLDELETEPROGRAMPROC     glDeleteProgram     = NULL;
PFNGLLINKPROGRAMPROC       glLinkProgram       = NULL;
PFNGLVALIDATEPROGRAMPROC   glValidateProgram   = NULL;
PFNGLUSEPROGRAMPROC        glUseProgram        = NULL;
PFNGLGETPROGRAMIVPROC      glGetProgramiv      = NULL;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = NULL;
PFNGLCREATESHADERPROC      glCreateShader      = NULL;
PFNGLDELETESHADERPROC      glDeleteShader      = NULL;
PFNGLSHADERSOURCEPROC      glShaderSource      = NULL;
PFNGLCOMPILESHADERPROC     glCompileShader     = NULL;
PFNGLATTACHSHADERPROC      glAttachShader      = NULL;
PFNGLDETACHSHADERPROC      glDetachShader      = NULL;
PFNGLGETSHADERIVPROC       glGetShaderiv       = NULL;
PFNGLGETSHADERINFOLOGPROC  glGetShaderInfoLog  = NULL;

PFNGLDISPATCHCOMPUTEPROC   glDispatchCompute   = NULL;
PFNGLMEMORYBARRIERPROC     glMemoryBarrier     = NULL;

// Attributes
PFNGLGETATTRIBLOCATIONPROC        glGetAttribLocation        = NULL;
PFNGLVERTEXATTRIBPOINTERPROC      glVertexAttribPointer      = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC  glEnableVertexAttribArray  = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = NULL;
// Uniforms
PFNGLGETUNIFORMLOCATIONPROC         glGetUniformLocation      = NULL;
PFNGLUNIFORMMATRIX3FVPROC           glUniformMatrix3fv        = NULL;
PFNGLUNIFORMMATRIX4FVPROC           glUniformMatrix4fv        = NULL;
PFNGLUNIFORM1IPROC                  glUniform1i               = NULL;
PFNGLUNIFORM1FPROC                  glUniform1f               = NULL;
PFNGLUNIFORM1FVPROC                 glUniform1fv              = NULL;
PFNGLUNIFORM3FVPROC                 glUniform3fv              = NULL;
PFNGLUNIFORM4FVPROC                 glUniform4fv              = NULL;
// FBO
PFNGLBINDFRAMEBUFFERPROC        glBindFramebuffer        = NULL;
PFNGLDELETEFRAMEBUFFERSPROC     glDeleteFramebuffers     = NULL;
PFNGLGENFRAMEBUFFERSPROC        glGenFramebuffers        = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus = NULL;
PFNGLFRAMEBUFFERTEXTUREPROC     glFramebufferTexture     = NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC     glFramebufferTexture2D     = NULL;

static bool Is_OpenGL_Extension_Load=false;

bool ag::is_opengl_extension_load(void)
{
    return Is_OpenGL_Extension_Load;
}
// Return true if extension is succesfully loaded
bool ag::opengl_extension_init(void)
{
    // Texture
#if _WIN32 || _WIN64
	glActiveTexture=(PFNGLACTIVETEXTUREPROC)glGetProcAddress("glActiveTexture");
#endif
	// VAO
	glGenVertexArrays=(PFNGLGENVERTEXARRAYSPROC)glGetProcAddress("glGenVertexArrays");
	glDeleteVertexArrays=(PFNGLDELETEVERTEXARRAYSPROC)glGetProcAddress("glDeleteVertexArrays");
	glBindVertexArray=(PFNGLBINDVERTEXARRAYPROC)glGetProcAddress("glBindVertexArray");

	bool vao_result = glGenVertexArrays && glDeleteVertexArrays && glBindVertexArray;

	// VBO
	glGenBuffers=(PFNGLGENBUFFERSPROC)glGetProcAddress("glGenBuffers");
	glDeleteBuffers=(PFNGLDELETEBUFFERSPROC)glGetProcAddress("glDeleteBuffers");
	glBindBuffer=(PFNGLBINDBUFFERPROC)glGetProcAddress("glBindBuffer");
	glBufferData=(PFNGLBUFFERDATAPROC)glGetProcAddress("glBufferData");
	glBufferSubData=(PFNGLBUFFERSUBDATAPROC)glGetProcAddress("glBufferSubData");
	glMapBuffer=(PFNGLMAPBUFFERPROC)glGetProcAddress("glMapBuffer");
	glUnmapBuffer=(PFNGLUNMAPBUFFERPROC)glGetProcAddress("glUnmapBuffer");
	glIsBuffer=(PFNGLISBUFFERPROC)glGetProcAddress("glIsBuffer");
	glGetBufferSubData=(PFNGLGETBUFFERSUBDATAPROC)glGetProcAddress("glGetBufferSubData");
	glGetBufferParameteriv=(PFNGLGETBUFFERPARAMETERIVPROC)glGetProcAddress("glGetBufferParameteriv");
    glGetBufferPointerv=(PFNGLGETBUFFERPOINTERVPROC)glGetProcAddress("glGetBufferPointerv");
    glMultiDrawElements=(PFNGLMULTIDRAWELEMENTSPROC)glGetProcAddress("glMultiDrawElements");

    glBindBufferBase=(PFNGLBINDBUFFERBASEPROC)glGetProcAddress("glBindBufferBase");


    bool vbo_result= glGenBuffers && glDeleteBuffers && glBindBuffer && glBufferData &&
                     glBufferSubData && glMapBuffer && glUnmapBuffer && glIsBuffer &&
                     glGetBufferSubData && glGetBufferParameteriv && glGetBufferPointerv &&
                     glMultiDrawElements   && glBindBufferBase;

	// Shaders
	glCreateProgram=(PFNGLCREATEPROGRAMPROC)glGetProcAddress("glCreateProgram");
	glDeleteProgram=(PFNGLDELETEPROGRAMPROC)glGetProcAddress("glDeleteProgram");
	glLinkProgram=(PFNGLLINKPROGRAMPROC)glGetProcAddress("glLinkProgram");
	glValidateProgram=(PFNGLVALIDATEPROGRAMPROC)glGetProcAddress("glValidateProgram");
	glUseProgram=(PFNGLUSEPROGRAMPROC)glGetProcAddress("glUseProgram");
	glGetProgramiv=(PFNGLGETPROGRAMIVPROC)glGetProcAddress("glGetProgramiv");
	glGetProgramInfoLog=(PFNGLGETPROGRAMINFOLOGPROC)glGetProcAddress("glGetProgramInfoLog");
	glCreateShader=(PFNGLCREATESHADERPROC)glGetProcAddress("glCreateShader");
	glDeleteShader=(PFNGLDELETESHADERPROC)glGetProcAddress("glDeleteShader");
	glShaderSource=(PFNGLSHADERSOURCEPROC)glGetProcAddress("glShaderSource");
	glCompileShader=(PFNGLCOMPILESHADERPROC)glGetProcAddress("glCompileShader");
	glAttachShader=(PFNGLATTACHSHADERPROC)glGetProcAddress("glAttachShader");
	glDetachShader=(PFNGLDETACHSHADERPROC)glGetProcAddress("glDetachShader");
	glGetShaderiv=(PFNGLGETSHADERIVPROC)glGetProcAddress("glGetShaderiv");
	glGetShaderInfoLog=(PFNGLGETSHADERINFOLOGPROC)glGetProcAddress("glGetShaderInfoLog");

	glDispatchCompute=(PFNGLDISPATCHCOMPUTEPROC)glGetProcAddress("glDispatchCompute");
	glMemoryBarrier=(PFNGLMEMORYBARRIERPROC)glGetProcAddress("glMemoryBarrier");

	bool shaders_result = glCreateProgram && glDeleteProgram && glLinkProgram &&
                          glValidateProgram && glUseProgram && glGetProgramiv &&
                          glGetProgramInfoLog && glCreateShader && glDeleteShader &&
                          glShaderSource && glCompileShader && glAttachShader &&
                          glDetachShader && glGetShaderiv && glGetShaderInfoLog  &&
                          glDispatchCompute && glMemoryBarrier;

	// Attributes
	glGetAttribLocation=(PFNGLGETATTRIBLOCATIONPROC)glGetProcAddress("glGetAttribLocation");
	glVertexAttribPointer=(PFNGLVERTEXATTRIBPOINTERPROC)glGetProcAddress("glVertexAttribPointer");
	glEnableVertexAttribArray=(PFNGLENABLEVERTEXATTRIBARRAYPROC)glGetProcAddress("glEnableVertexAttribArray");
	glDisableVertexAttribArray=(PFNGLDISABLEVERTEXATTRIBARRAYPROC)glGetProcAddress("glDisableVertexAttribArray");

    bool attributes_result = glGetAttribLocation && glVertexAttribPointer &&
                             glEnableVertexAttribArray && glDisableVertexAttribArray;

	// Uniforms
	glGetUniformLocation=(PFNGLGETUNIFORMLOCATIONPROC)glGetProcAddress("glGetUniformLocation");
	glUniformMatrix3fv=(PFNGLUNIFORMMATRIX3FVPROC)glGetProcAddress("glUniformMatrix3fv");
	glUniformMatrix4fv=(PFNGLUNIFORMMATRIX4FVPROC)glGetProcAddress("glUniformMatrix4fv");
	glUniform1i=(PFNGLUNIFORM1IPROC)glGetProcAddress("glUniform1i");
	glUniform1f=(PFNGLUNIFORM1FPROC)glGetProcAddress("glUniform1f");
	glUniform1fv=(PFNGLUNIFORM1FVPROC)glGetProcAddress("glUniform1fv");
	glUniform3fv=(PFNGLUNIFORM3FVPROC)glGetProcAddress("glUniform3fv");
	glUniform4fv=(PFNGLUNIFORM4FVPROC)glGetProcAddress("glUniform4fv");

    bool uniforms_result = glGetUniformLocation && glUniformMatrix3fv && glUniformMatrix4fv &&
                           glUniform1i && glUniform1f && glUniform1fv && glUniform3fv && glUniform4fv;

    // FBO
	glBindFramebuffer=(PFNGLBINDFRAMEBUFFERPROC)glGetProcAddress("glBindFramebuffer");
	glDeleteFramebuffers=(PFNGLDELETEFRAMEBUFFERSPROC)glGetProcAddress("glDeleteFramebuffers");
	glGenFramebuffers=(PFNGLGENFRAMEBUFFERSPROC)glGetProcAddress("glGenFramebuffers");
	glCheckFramebufferStatus=(PFNGLCHECKFRAMEBUFFERSTATUSPROC)glGetProcAddress("glCheckFramebufferStatus");
	glFramebufferTexture=(PFNGLFRAMEBUFFERTEXTUREPROC)glGetProcAddress("glFramebufferTexture");
    glFramebufferTexture2D=(PFNGLFRAMEBUFFERTEXTURE2DPROC)glGetProcAddress("glFramebufferTexture2D");

    bool fbo_result = glBindFramebuffer && glDeleteFramebuffers && glGenFramebuffers &&
                      glCheckFramebufferStatus && glFramebufferTexture && glFramebufferTexture2D;

    Is_OpenGL_Extension_Load = vao_result && vbo_result && shaders_result &&
                               attributes_result && uniforms_result && fbo_result;
#if _WIN32 || _WIN64
    Is_OpenGL_Extension_Load = Is_OpenGL_Extension_Load && glActiveTexture;
#endif

    return Is_OpenGL_Extension_Load;
}

void ag::opengl_contex_info(std::ofstream& file) {
   GLenum error = glGetError();
   if(error != GL_NO_ERROR)
       log_file<<"ag::opengl_contex_info(), error = "<<error<<std::endl;

	// выведем в лог немного информации о контексте OpenGL
	GLint major, minor, info;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	file<<"OpenGL render context information:"<<std::endl;
	file<<"Renderer: "<< glGetString(GL_RENDERER) <<std::endl;
	file<<"Vendor: "<< glGetString(GL_VENDOR) <<std::endl;
    file<<"Version: "<< glGetString(GL_VERSION) <<std::endl;
    file<<"GLSL version: "<< glGetString(GL_SHADING_LANGUAGE_VERSION) <<std::endl;
	file<<"OpenGL version: "<< major << "." << minor <<std::endl;

	// значения различных параметров OpenGL
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &info);
	file<<"GL_MAX_VERTEX_ATTRIBS: "<< info << std::endl;
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &info);
	file<<"GL_MAX_VERTEX_UNIFORM_COMPONENTS: "<< info << std::endl;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &info);
	file<<"GL_MAX_TEXTURE_IMAGE_UNITS: "<< info << std::endl;

   error = glGetError();
   if(error != GL_NO_ERROR)
       log_file<<"opengl_contex_info(), error = "<<error<<std::endl;
}
