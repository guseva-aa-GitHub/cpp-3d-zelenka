#include "material.h"

#include <fstream>
extern std::ofstream log_file;

using namespace ag;

GLint Material::_ambient_loc;
GLint Material::_diffuse_loc;
GLint Material::_emission_loc;
GLint Material::_specular_loc;
GLint Material::_shininess_loc;
GLint Material::_opacity_loc;
GLint Material::_texture_present_loc;
GLint Material::_two_sided_loc;
GLint Material::_texture_loc;

Material::Material()
:_shininess(50.0f), _opacity(1.0f), 
_texture_ref(0), _two_sided(false) {
    _ambient.set(0.588f, 0.588f, 0.588f, 1.0f);
    _diffuse.set(0.588f, 0.588f, 0.588f, 1.0f);
    _specular.set(0.150f, 0.150f, 0.150f, 1.0f);
}

void Material::Location(const GLuint program_id) {
    _ambient_loc = glGetUniformLocation(program_id,      "material.ambient");
    _diffuse_loc = glGetUniformLocation(program_id,      "material.diffuse");
    _emission_loc = glGetUniformLocation(program_id,     "material.emission");
    _specular_loc = glGetUniformLocation(program_id,     "material.specular");
    _shininess_loc = glGetUniformLocation(program_id,    "material.shininess");
    _opacity_loc = glGetUniformLocation(program_id,      "material.opacity");
    _texture_present_loc = glGetUniformLocation(program_id,  "material.texture_present");
    _two_sided_loc = glGetUniformLocation(program_id,    "material.two_sided");
    _texture_loc = glGetUniformLocation(program_id,      "material.texture");
}

void Material::BeginApply() {
   // if(two_sided) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
    glUniform4fv(_ambient_loc, 1,   _ambient.ptr());
    glUniform4fv(_diffuse_loc, 1,   _diffuse.ptr());
    glUniform4fv(_emission_loc, 1,  _emission.ptr());
    glUniform4fv(_specular_loc, 1,  _specular.ptr());

    glUniform1f(_shininess_loc,     _shininess);
    glUniform1f(_opacity_loc,       _opacity);
    glUniform1f(_texture_present_loc, TexturePresent());
    glUniform1f(_two_sided_loc,     _two_sided);

    glActiveTexture(GL_TEXTURE0 + 0);
    if(_texture_ref) {
        glBindTexture(GL_TEXTURE_2D, _texture_ref);
        glUniform1i(_texture_loc, 0);
    }
}

void Material::EndApply(void) {
    //if(two_sided) glDisable(GL_CULL_FACE);
    glBindTexture(GL_TEXTURE_2D, 0);
}
