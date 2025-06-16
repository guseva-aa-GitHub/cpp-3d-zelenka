#pragma once

#include "gl_extentions.h"

namespace ag {
class FrameBufferImage {
private:
    GLuint _fbo;
    GLuint _pbo;
    GLuint _fboTexture;
    GLuint _drb;
    GLsizei _width, _height;
    GLsizei _w_min, _h_min;
    GLsizei _size;

public:
    FrameBufferImage()
    : _fbo(0), _pbo(0), _fboTexture(0), _drb(0), _width(0), _height(0),
    _w_min(0), _h_min(0), _size(0) { }
    
    ~FrameBufferImage();

    void init(GLsizei width, GLsizei height);
    void begin_apply(void) const;
    void end_apply(void) const;

    // void save_8bit(const char* file_name) { }
    void save_16bit(const char* file_name);
};
}

