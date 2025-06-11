#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "gl_extentions.h"

namespace ag {

class FrameBuffer {
public:
    FrameBuffer(): depthFBO(0), depthTexture(0), width(0), height(0) {}
    ~FrameBuffer();
    void init(GLsizei width, GLsizei height);
    void begin_apply(const unsigned int program_id) const;
    void end_apply(void) const;
    void location(const GLuint program_id) const;
    GLuint get_depthTexture(void) { return depthTexture; }

private:
    GLuint depthFBO;
    GLuint depthTexture;
    GLsizei width;
    GLsizei height;
};

} //end namespace ag

#endif // FRAMEBUFFER_H
