#include "frame_buffer_image.h"

#include <memory>
#include <fstream>
#include <vector>

extern std::ofstream log_file;
using namespace ag;

FrameBufferImage::~FrameBufferImage() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if(_fbo) glDeleteFramebuffers(1, &_fbo);

    glBindTexture(GL_TEXTURE_2D, 0);
    if(_fboTexture) glDeleteTextures(1, &_fboTexture);

//    if(_drb) glDeleteRenderbuffers(1, &_drb);
    if(_pbo) glDeleteBuffers(1, &_pbo);
}

void FrameBufferImage::init(GLsizei width, GLsizei height) {
    _width = width; _height = height;

    // создаем FBO
	glGenFramebuffers(1, &_fbo);
	// делаем созданный FBO текущим
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

    //Создадим текстуру
    glGenTextures(1, &_fboTexture);
    glBindTexture(GL_TEXTURE_2D, _fboTexture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F,
                _width, _height, 0, GL_RGB, GL_FLOAT, NULL);

	// указываем для текущего FBO текстуру, куда следует производить рендер
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _fboTexture, 0);

//    glGenRenderbuffers(1, &_drb);
//    glBindRenderbuffer(GL_RENDERBUFFER, _drb);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _width, _height);
//    glBindRenderbuffer(GL_RENDERBUFFER, 0);
//    // attach the renderbuffer to depth attachment point
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _drb);

    glGenBuffers(1, &_pbo);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, _pbo);
    glBufferData(GL_PIXEL_PACK_BUFFER, 3*_width*_height*sizeof(GLfloat), NULL, GL_STATIC_READ);

 	// проверим текущий FBO на корректность
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
        log_file<<"Error off Check Framebuffer Status "<<status<<std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
}

void FrameBufferImage::save_16bit(const char* file_name) {
    glBindBuffer(GL_PIXEL_PACK_BUFFER, _pbo);
    glReadPixels(0, 0, _width, _height, GL_RGB, GL_FLOAT, 0);

    GLfloat* buffer = (GLfloat*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
    if(buffer)
    {
        std::string name = file_name;   name += ".txt";
        std::ofstream outFileTxt(name.c_str(), std::ios::out);        
//        outFileTxt.setf( std::ios::dec, std::ios::basefield );

        outFileTxt.setf( std::ios::fixed );
        outFileTxt.precision(8);

        for(int y = _height-1; y > -1; --y) {
            for(int x = 0; x < 3*_width; ++x) {
                GLfloat *ptr = buffer + y*_width*3 + x;
                outFileTxt<<ptr[0]<<" ";
            }
            outFileTxt<<std::endl;
        }

        outFileTxt.close();

        glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
    }
    else log_file<<"Error mapper"<<std::endl;
}

void FrameBufferImage::begin_apply(void) const {
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameBufferImage::end_apply(void) const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDrawBuffer(GL_BACK);
    glReadBuffer(GL_BACK);
}
