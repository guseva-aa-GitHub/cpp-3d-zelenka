#pragma once

#include "base_model.h"
#include "buffers.h"
#include "mesh.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <unordered_map>
#include <memory>
#include <vector>

namespace ag {

class ModelAssimp : public BaseModel {
public:
	ModelAssimp() : _scene(nullptr) { _type = AG_TYPE_ASSIMP_MODEL; }
	virtual ~ModelAssimp() { Clear(); }

	bool Init() override;
	void Draw(const unsigned int program_id) const override;

private:
	const aiScene* _scene;
	std::vector<std::unique_ptr<ag::Mesh>> _meshs;
	std::unordered_map<std::string, GLuint> _textureIdMap;

	void Clear();
	void BoundingBox();
	void NodeBoundingBox(const aiNode* nd, ag::vec3 &min, ag::vec3 &max, aiMatrix4x4 *trafo);
	void RecursiveLoad(const aiNode* nd, aiMatrix4x4 *trafo);
	void CopyMaterial(ag::Material &material, const aiMaterial *mtl);
	void LoadTextures();	
};

} 
