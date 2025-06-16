#pragma once

#include "base_model.h"
#include "buffers.h"
#include "mesh.h"

#include <memory>

namespace ag {

class ModelSimple : public BaseModel {
public:
    ModelSimple()
    : _textureId(0) { _type = AG_TYPE_SIMPLE_MODEL; }

    virtual ~ModelSimple() { Clear(); }

    bool Init() override;
    void Draw(const unsigned int program_id) const override;

private:
    std::unique_ptr<ag::Mesh> _u_mesh;    
    GLuint _textureId;

    void Clear(void);
    void BoundingBox(std::vector<ag::vec3> &vertex);
};

} //namespace ag

