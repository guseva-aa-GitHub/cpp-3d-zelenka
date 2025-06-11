#include "mesh.h"

#include <fstream>
extern std::ofstream log_file;

using namespace ag;

GLint Mesh::_vertex_loc;
GLint Mesh::_normal_loc;
GLint Mesh::_texel_loc;
GLint Mesh::_color_loc;

Mesh::Mesh(): _vao(0), _numFaces(0) {
    //запросим у OpenGL свободный индекс VAO
	glGenVertexArrays(1, &_vao);
}

Mesh::~Mesh() {
    if(_vao) glDeleteVertexArrays(1, &_vao);
}

void Mesh::Location(const GLuint program_id) {
    _vertex_loc = glGetAttribLocation(program_id, "vertex");
    _normal_loc = glGetAttribLocation(program_id, "normal");
    _texel_loc  = glGetAttribLocation(program_id, "texel");
    _color_loc  = glGetAttribLocation(program_id, "color");
}

void Mesh::CreateVertexVBO(const GLfloat *buffer, GLsizei buffer_size) {
    _vertex.create(GL_ARRAY_BUFFER, buffer, buffer_size);
}

void Mesh::CreateNormalVBO(const GLfloat *buffer, GLsizei buffer_size) {
    _normal.create(GL_ARRAY_BUFFER, buffer, buffer_size);
}

void Mesh::CreateTexelVBO(const GLfloat *buffer, GLsizei buffer_size) {
    _texel.create(GL_ARRAY_BUFFER, buffer, buffer_size);
}

void Mesh::CreateIndexVBO(const GLuint *buffer, GLsizei buffer_size) {
    _index.create(GL_ELEMENT_ARRAY_BUFFER, buffer, buffer_size);
}

void Mesh::CreateColorVBO(const GLfloat *buffer, GLsizei buffer_size) {
    _color.create(GL_ARRAY_BUFFER, buffer, buffer_size);
}

void Mesh::EnableVBO() {
    //укажем параметры вершинного атрибута для текущего активного VBO
    if (_vertex_loc != -1) {
        glEnableVertexAttribArray(_vertex_loc);
        glBindBuffer(GL_ARRAY_BUFFER, _vertex.getID());
        glVertexAttribPointer(_vertex_loc, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
    }

    //укажем параметры атрибута нормали для текущего активного VBO
    if (_normal_loc != -1) {
        glEnableVertexAttribArray(_normal_loc);
        glBindBuffer(GL_ARRAY_BUFFER, _normal.getID());
        glVertexAttribPointer(_normal_loc, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
    }

    if( !_texel.empty() && _texel_loc != -1 ) {
        glEnableVertexAttribArray(_texel_loc);
        glBindBuffer(GL_ARRAY_BUFFER, _texel.getID());
        glVertexAttribPointer(_texel_loc, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
    }

    if( !_color.empty() && _color_loc != -1 ) {
        glEnableVertexAttribArray(_color_loc);
        glBindBuffer(GL_ARRAY_BUFFER, _color.getID());
        glVertexAttribPointer(_color_loc, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0);
    }
}

void Mesh::DisableVBO() {
    if (_vertex_loc != -1)   glDisableVertexAttribArray(_vertex_loc);
    if (_normal_loc != -1)   glDisableVertexAttribArray(_normal_loc);
    if (_texel_loc != -1)    glDisableVertexAttribArray(_texel_loc);
    if (_color_loc != -1)    glDisableVertexAttribArray(_color_loc);
}

void Mesh::Draw() {
    BeginApply();
    EnableVBO();

    _material.BeginApply();
    glDrawElements(GL_TRIANGLES, _numFaces, GL_UNSIGNED_INT, 0);
    _material.EndApply();

    DisableVBO();
    EndApply();
}
