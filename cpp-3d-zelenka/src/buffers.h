#ifndef AG_BUFFERS_H
#define AG_BUFFERS_H

#include "gl_extentions.h"

namespace ag {

template <typename T> class BufferVBO {
private:
    GLuint id;
    void clear();

public:
    BufferVBO() : id(0)   {};
    ~BufferVBO()          { clear(); }

    void create(GLenum target, const T *buffer, GLsizei buffer_size, const GLuint id_b=0);

    bool empty() const      { return (id==0); }
    GLuint getID()          { return id; }
};

template <typename T> void BufferVBO<T>::clear(void) {
    if (id) glDeleteBuffers(1,&id);
    id=0;
}

template <typename T> void BufferVBO<T>::create(GLenum target, const T *buffer, 
                GLsizei buffer_size, const GLuint id_b) {
    clear();

    if(buffer_size) {
        glGenBuffers(1, &id);

        if (target == GL_SHADER_STORAGE_BUFFER) {
            glBindBufferBase(target, id_b, id);
            glBufferData(target, buffer_size*sizeof(T), buffer, GL_DYNAMIC_DRAW);
        }
        else  {
            glBindBuffer(target, id);
            glBufferData(target, buffer_size*sizeof(T), buffer, GL_STATIC_DRAW);
        }
    }
}

}
#endif // AG_BUFFERS_H
