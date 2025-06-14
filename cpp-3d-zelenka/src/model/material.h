#ifndef MATERIAL_H
#define MATERIAL_H

#include "gl_extentions.h"
#include "math/other_vectors.h"

namespace ag {

class Material {
public:
    Material();
    ~Material() {}

    static void Location(const GLuint program_id);
    void BeginApply();
    void EndApply();

    inline bool TexturePresent() const { return _texture_ref; }
    inline const ag::color& GetDiffuse() const { return _diffuse; }

    inline void SetTextureRef(const GLuint texture_ref) { _texture_ref = texture_ref;}
    inline void SetTwoSided(const bool two_sided)       { _two_sided = two_sided; }

    inline void SetAmbient(const ag::color& color)   { _ambient.set(color); }
    inline void SetDiffuse(const ag::color& color)   { _diffuse.set(color); }
    inline void SetEmission(const ag::color& color)  { _emission.set(color); }
    inline void SetSpecular(const ag::color& color)  { _specular.set(color); }

    inline void SetShininess(const float value)      { _shininess = value; }

private:
    static GLint _ambient_loc;
    static GLint _diffuse_loc;
    static GLint _emission_loc;
    static GLint _specular_loc;
    static GLint _shininess_loc;
    static GLint _opacity_loc;
    static GLint _texture_present_loc;
    static GLint _two_sided_loc;
    static GLint _texture_loc;

    ag::color   _ambient;      //рассеянный цвет материала (цвет материала в тени)
    ag::color   _diffuse;      //цвет диффузного отражения материала
    ag::color   _emission;     //интенсивность излучаемого света материала
    ag::color   _specular;     //цвет зеркального отражения материала
    GLfloat _shininess;        //степень зеркального отражения материала
    GLfloat _opacity;          //непрозрачность

    GLuint _texture_ref;
    bool _two_sided;
};

} //end namespace ag

#endif // MATERIAL_H
