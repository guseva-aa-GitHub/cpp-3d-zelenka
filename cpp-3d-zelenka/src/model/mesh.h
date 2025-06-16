#pragma once

#include <vector>
#include "gl_extentions.h"
#include "shader.h"

#include "buffers.h"
#include "material.h"

namespace ag {

class Mesh {
public:
    GLint _numFaces;
    ag::Material _material;

    Mesh();
    ~Mesh();

    static void Location(const GLuint program_id);

    // void CreateVertexVBOShaderC(const GLfloat *buffer, GLsizei buffer_size);

    void CreateVertexVBO(const GLfloat *buffer, GLsizei buffer_size);
    void CreateNormalVBO(const GLfloat *buffer, GLsizei buffer_size);
    void CreateTexelVBO(const GLfloat *buffer, GLsizei buffer_size);
    void CreateIndexVBO(const GLuint *buffer, GLsizei buffer_size);
    void CreateColorVBO(const GLfloat *buffer, GLsizei buffer_size);

    void Draw();
    void BeginApply() const    { glBindVertexArray(_vao); }
    void EndApply() const      { glBindVertexArray(0); }

private:
    static GLint _vertex_loc;
    static GLint _normal_loc;
    static GLint _texel_loc;
    static GLint _color_loc;

    GLuint _vao;
    ag::BufferVBO<GLfloat> _vertex;
    ag::BufferVBO<GLfloat> _normal;
    ag::BufferVBO<GLfloat> _texel;
    ag::BufferVBO<GLuint>  _index;
    ag::BufferVBO<GLfloat> _color;

    void EnableVBO();
    void DisableVBO();
};

} 

