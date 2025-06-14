#include <memory>
#include <float.h>
#include <fstream>

#include "base.h"
#include "model_simple.h"
#include "other_vectors.h"
#include "texture.h"

extern std::ofstream log_file;

using namespace ag;

bool ModelSimple::Init() {
    Clear();
    
    std::string file_name = _path + "/" + _file;
    std::string node_name;
    GLuint line_count;
    try {
        log_file<<"Init Simple Model, Model name = "<<_name<<", Model path = " <<_path<<", Model file = " <<_file<<std::endl;
        std::ifstream file(file_name.c_str());
        if (!file.is_open())
            return false;

        std::unique_ptr unique_mesh = std::make_unique<ag::Mesh>();
        unique_mesh->BeginApply();

        float x, y, z;
        file >> node_name >> line_count;
        std::vector<ag::vec3> vertex(line_count);
        for(GLuint i=0; i<line_count; ++i) { 
            file >> x >> y >> z; 
            vertex[i].set(x,y,z); 
        }
        unique_mesh->CreateVertexVBO(vertex[0].ptr(), 3*vertex.size());

        BoundingBox(vertex);
        vertex.clear();

        file >> node_name >> line_count;
        std::vector<ag::vec3> normal(line_count);
        for(GLuint i=0; i<line_count; ++i) { 
            file >> x >> y >> z; 
            normal[i].set(x,y,z); 
        }
        unique_mesh->CreateNormalVBO(normal[0].ptr(), 3*normal.size());
        normal.clear();

        file>>node_name>>line_count;
        std::vector<ag::vec2> texel(line_count);
        for(GLuint i=0; i < line_count; ++i) { 
            file >> x >> y; 
            texel[i].set(x,y); 
        }
        unique_mesh->CreateTexelVBO(texel[0].ptr(), 2*texel.size());
        texel.clear();

        unsigned int ux, uy, uz;
        file >> node_name >> line_count;

        std::vector<ag::uvec3> index(line_count);
		for (GLuint i=0; i < line_count; ++i) { 
            file >> ux >> uy >> uz; 
            index[i].set(ux,uy,uz); 
        }
        unique_mesh->CreateIndexVBO(index[0].ptr(), 3*index.size());
		index.clear();

        unique_mesh->_numFaces = 3*line_count;

        std::string tn, fn = _path + "/";
        file >> node_name >> tn;    
        fn += tn;

        _textureId = get_textureID(fn.c_str());
        unique_mesh->_material.SetTextureRef(_textureId);

        unique_mesh->EndApply();

        _u_mesh = std::move(unique_mesh);

        file.close();
        log_file<<"Init success"<<std::endl;
        _ready = true;
    }
    catch(std::exception &e) {
        log_file<<e.what()<<", Init failed"<<std::endl;
        return false;
    }

    return _ready;
}

void ModelSimple::Clear() {
    if (_textureId) 
        glDeleteTextures(1, &_textureId);
    _textureId = 0;

    _ready = false;
    // log_file<<"Clear Simple Model, Model name = "<<_name<<", Model file = "<<_file<<std::endl;
}

void ModelSimple::BoundingBox(std::vector<ag::vec3> &vertex) {
    _min.set(std::numeric_limits<float>::max());
    _max.set(std::numeric_limits<float>::min());

    for (const auto& vec : vertex) {
        _min.setX(ag::math::minimum(_min.x(), vec.x()));
		_min.setY(ag::math::minimum(_min.y(), vec.y()));
		_min.setZ(ag::math::minimum(_min.z(), vec.z()));

		_max.setX(ag::math::maximum(_max.x(), vec.x()));
		_max.setY(ag::math::maximum(_max.y(), vec.y()));
		_max.setZ(ag::math::maximum(_max.z(), vec.z()));
    }

    SetCenterAndScope();
}

void ModelSimple::Draw(const unsigned int program_id) const {
    if(!_ready) return;

    set_uniform_location(program_id);
    _u_mesh->Draw();
}
