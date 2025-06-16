#include "shader.h"

#include <filesystem>

#define DEFINE_TO_STR_HELPER(x) #x
#define DEFINE_TO_STR(x)        "#define " #x " " DEFINE_TO_STR_HELPER(x) "\n"

// набор параметров для вершинного шейдера
static const char vertexShaderDefines[] =
    "#version 430 core\n"
    DEFINE_TO_STR(VERT_POSITION)
    DEFINE_TO_STR(VERT_TEXCOORD)
    DEFINE_TO_STR(VERT_NORMAL)
    "\n";

// набор параметров для фрагментного шейдера
static const char fragmentShaderDefines[] =
    "#version 430 core\n"
    DEFINE_TO_STR(FRAG_OUTPUT0)
    "\n";


//проверка статуса шейдерной программы
GLint shader_program_status(GLuint program, GLenum param, std::ofstream& log_file) {
	GLint status, length, buffer_size=0;
	char *buffer = nullptr;

	glGetProgramiv(program, param, &status);

	if (status != GL_TRUE) {
	    log_file<<"Shader program\n";
	    try {
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &buffer_size);
            buffer = new char[buffer_size];
            glGetProgramInfoLog(program, buffer_size, &length, buffer);
            log_file<< buffer <<std::endl;
            delete[] buffer;
	    }
	    catch(std::exception &e) {
	        log_file<<", exception: " << e.what() <<std::endl;
	    }
	}
	return status;
}

//проверка статуса шейдера
GLint shader_status(GLuint shader, const char* shader_name, std::ofstream& log_file) {
	GLint status, length, buffer_size=0;
	char *buffer=nullptr;

	glGetShaderiv(shader,  GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE) {
	    log_file<<"Shader: "<< shader_name << std::endl;
	    try {
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &buffer_size);
            if (buffer_size) {
                buffer = new char[buffer_size];
                glGetShaderInfoLog(shader, buffer_size, &length, buffer);
                log_file<< buffer <<std::endl;
                delete[] buffer;
            }
            else log_file<<"INFO LOG LENGTH=0\n";
	    }
	    catch(std::exception &e)
	    {
	        log_file<<", exception: " << e.what() <<std::endl;
	    }
	}
	return status;
}

//линковка шейдерной программы и проверка статуса линковки
bool link_shader_program(GLuint program, std::ofstream& log_file) {
	glLinkProgram(program);
	return (shader_program_status(program, GL_LINK_STATUS, log_file) == GL_TRUE);
}
//проверка на корректность шейдерной программы
bool validate_shader_program(GLuint program, std::ofstream& log_file) {
	glValidateProgram(program);
	return (shader_program_status(program, GL_VALIDATE_STATUS, log_file) == GL_TRUE);
}

//делает указанную шейдерную программу неактивной и удаляет ее
void delete_shader_program(GLuint program) {
	glUseProgram(0);
	glDeleteProgram(program);
}

GLuint create_shader(const char* file_name, GLenum shader_type, std::ofstream& log_file) {
    GLuint shader = 0;
    GLchar *source = nullptr;

    //создадим шейдер
    if((shader = glCreateShader(shader_type)) == 0)
        log_file<<"Creating shader " << file_name <<" fail, error=  "<< glGetError() <<std::endl;
    else {
        try {
            GLuint size = 0;
            std::ifstream file(file_name);
            if( file.is_open() ) {
                file.seekg(0, std::ios::end);
                size = file.tellg();
                file.seekg (0, std::ios::beg);
                source = new GLchar[size+1];

                file.read(source, size);
                size = file.gcount();
                file.close();

                source[size] = 0x0;
            }
            else log_file<<"Error open file = "<< file_name << std::endl;

            //зададим шейдеру исходный код и скомпилируем его
            glShaderSource(shader, 1, &source, nullptr);
            glCompileShader(shader);
            delete[] source;

            //проверим статус компиляции шейдера
            if (shader_status(shader, file_name, log_file) != GL_TRUE) {
                glDeleteShader(shader); shader = 0;
            }
        }
        catch(std::exception& e) {
            log_file<<"Creating shader " << file_name <<" fail, what=  "<< e.what() <<std::endl;
            if(shader) glDeleteShader(shader); shader = 0;
        }
    }
	return shader;
}

GLuint create_shader_program(const char *vertex_fn, const char* fragment_fn, std::ofstream& log_file) {
	GLuint program = 0;
    //создадим шейдерную программу
	if((program = glCreateProgram()) == 0) {
		log_file<<"Creating shader program fail, error= "<< glGetError() <<std::endl;
		return 0;
	}

	//создадим шейдеры
	GLuint vertex_shader   = create_shader(vertex_fn,	GL_VERTEX_SHADER,   log_file);
	GLuint fragment_shader = create_shader(fragment_fn, GL_FRAGMENT_SHADER, log_file);

	//присоеденим шейдеры к шейдерной программе
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	//шейдеры больше не нужены, пометим их на удаление
	//они будет удалены вместе с шейдерной программой
	glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    //линковка шейдерной программы и проверка статуса линковки
    if(link_shader_program(program, log_file) == false)     {
        //делает указанную шейдерную программу неактивной и удаляет ее
        delete_shader_program(program); program=0;
    }

    return program;
}

GLuint create_shader_program(const char *compute_fn, std::ofstream& log_file) {
	GLuint program=0;

	//создадим шейдерную программу
	if((program = glCreateProgram()) == 0)
	{
		log_file<<"Creating compute shader program fail, error= "<< glGetError() <<std::endl;
		return 0;
	}

    //создадим шейдер
    GLuint compute_shader   = create_shader(compute_fn, GL_COMPUTE_SHADER, log_file);

	//присоеденим шейдер к шейдерной программе
	glAttachShader(program, compute_shader);
	//шейдер больше не нужен, пометим на удаление
	//будет удален вместе с шейдерной программой
	glDeleteShader(compute_shader);

    //линковка шейдерной программы и проверка статуса линковки
    if(link_shader_program(program, log_file) == false)
    {
        //делает указанную шейдерную программу неактивной и удаляет ее
        delete_shader_program(program); program=0;
    }

    return program;
}

void ShaderProgram::Init(const std::string& path, const std::string& name, std::ofstream& log_file) {
    //проверка на наличии папки в текущей директории
    std::string path_shaders = path;
    if (!std::filesystem::exists(path_shaders)) {
        path_shaders = "../" + path_shaders;
    }

    std::string name_vx = path_shaders + "/" + name + ".vsh";
    std::string name_fr = path_shaders + "/" + name + ".fsh";

    program = create_shader_program(name_vx.c_str(), name_fr.c_str(), log_file);    
}

// void ShaderProgram::Init(const char *compute_fn, std::ofstream& log_file) {
//     program = create_shader_program(compute_fn, log_file);
// }

void ShaderProgram::ShaderCompute() {
    glDispatchCompute(100*100*100 / 1000, 1, 1);
    glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT );
}
