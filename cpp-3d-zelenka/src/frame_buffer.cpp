#include "frame_buffer.h"
#include "texture.h"
#include <assert.h>

using namespace ag;
extern std::ofstream log_file;

void texture_location(const unsigned int program_id, GLuint depthTexture, GLint unit);

class FBexception : public std::exception
{
    std::string msg;
    public:
    FBexception(const char* str):msg(str) {}
    ~FBexception() throw() {}
    const char* what() const throw() { return msg.c_str(); }
};

FrameBuffer::~FrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if(depthFBO) glDeleteFramebuffers(1, &depthFBO);

    glBindTexture(GL_TEXTURE_2D, 0);
    if(depthTexture) glDeleteTextures(1, &depthTexture);
}

void FrameBuffer::init(GLsizei iWidth, GLsizei iHeight)
{
    assert(iHeight);

    width = iWidth;
    height = iHeight;
    depthTexture = get_textureDepthID(width, height);

    // создаем FBO для рендера глубины в текстуру
	glGenFramebuffers(1, &depthFBO);
	// делаем созданный FBO текущим
	glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);

	// отключаем вывод цвета в текущий FBO
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// указываем для текущего FBO текстуру, куда следует производить рендер глубины
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

	// проверим текущий FBO на корректность
	GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        throw FBexception("Error off Check Framebuffer Status");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::begin_apply(const unsigned int program_id) const
{
    glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
	glViewport(0, 0, width, height);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_TRUE);
	glClear(GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);

    texture_location(program_id, depthTexture, 0);
}

void FrameBuffer::end_apply(void) const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_BACK);
}

void FrameBuffer::location(const GLuint program_id) const
{
    texture_location(program_id, depthTexture, 1);
}

void texture_location(const unsigned int program_id, GLuint depthTexture, GLint unit)
{
    glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glUniform1i(glGetUniformLocation(program_id, "depthTexture"), unit);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
}
