#ifndef SHADER_H
#define SHADER_H

#include <fstream>
#include <string>
#include "gl_extentions.h"

// типы шейдеров
#define ST_VERTEX     0x01
#define ST_FRAGMENT   0x02

//создает шейдер
GLuint create_shader(const char* file_name, GLenum shader_type, std::ofstream& log_file);

//создает шейдерную программу и загружает шейдеры указанные в vertex_fn и fragment_fn
GLuint create_shader_program(const char *vertex_fn, const char* fragment_fn, std::ofstream& log_file);

//создает шейдерную программу и загружает шейдеры указанные в vertex_fn и fragment_fn
GLuint create_shader_program(const char *compute_fn, std::ofstream& log_file);

//проверка статуса шейдерной программы
GLint shader_program_status(GLuint program, GLenum param, std::ofstream& log_file);

//проверка статуса шейдера
GLint shader_status(GLuint shader, const char* shader_name, std::ofstream& log_file);

//делает указанную шейдерную программу неактивной и удаляет ее
void delete_shader_program(GLuint program);

//линковка шейдерной программы и проверка статуса линковки
bool link_shader_program(GLuint program, std::ofstream& log_file);

//проверка на корректность шейдерной программы
bool validate_shader_program(GLuint program, std::ofstream& log_file);

class ShaderProgram
{
private:
    GLuint program;

public:
    ShaderProgram(void):    program(0){}
    ~ShaderProgram()        { delete_shader_program(program); }

    void Init(const std::string& path, const std::string& name, std::ofstream& log_file);
    // void Init(const char *compute_fn, std::ofstream& log_file);

    void BeginApply() const    { glUseProgram(program); }
    void EndApply() const      { glUseProgram(0); }
    GLuint Id() const          { return program; }

    void ShaderCompute();
};

#endif /* SHADER_H */
