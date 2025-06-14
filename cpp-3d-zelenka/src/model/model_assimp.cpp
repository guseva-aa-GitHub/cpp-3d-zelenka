#include "model_assimp.h"

#include "texture.h"
#include "math/base.h"

#include <assert.h>
#include <float.h>
#include <fstream>

extern std::ofstream log_file;

using namespace ag;

class ASMexception : public std::exception {
    std::string msg;
public:
    ASMexception(const char* error_str) : msg(error_str) {}
    ~ASMexception() throw() {}
    const char* what() const throw() { return msg.c_str(); }
};

bool ModelAssimp::Init() {
    Clear();
    log_file<<"Init Assimp Model, Model name = "<<_name<<", Model path = " <<_path<<", Model file = " <<_file<<std::endl;
    std::string file = _path + "/" + _file;

    _scene = aiImportFile(file.c_str(),
                        aiProcess_OptimizeMeshes|
                        aiProcess_SplitLargeMeshes|
                        aiProcess_Triangulate|
                        aiProcess_PreTransformVertices|
                        aiProcess_GenSmoothNormals|
                        aiProcess_FindInvalidData|
                        aiProcess_FixInfacingNormals|
                        aiProcess_JoinIdenticalVertices|
                        aiProcess_SortByPType|
                        aiProcess_FlipUVs
                       );
    if (_scene == nullptr) {
        log_file<<aiGetErrorString()<<", Init failed."<<std::endl;
        return false;
    }

    try {
        BoundingBox();
        LoadTextures();

        aiMatrix4x4 trafo;
        aiIdentityMatrix4(&trafo);
        RecursiveLoad(_scene->mRootNode, &trafo);

//        log_file<<"mesh count "<<mesh_list.size()<<"\n";
    } catch(std::exception &e) {
        log_file<<e.what()<<", Init failed"<<std::endl;
        return false;
    }

    if (_scene) aiReleaseImport(_scene);
    _scene = nullptr;

//    log_file<<"Init success"<<std::endl;

    _ready = true;
    return _ready;
}

void ModelAssimp::RecursiveLoad(const aiNode* nd, aiMatrix4x4 *trafo) {
    assert(_scene);
    assert(nd);

    aiMatrix4x4 prev = *trafo;
    aiMultiplyMatrix4(trafo,&nd->mTransformation);

    // load all meshes assigned to this node
	for (unsigned int n=0; n < nd->mNumMeshes; ++n) {
		const aiMesh* ai_ptr_mesh = _scene->mMeshes[nd->mMeshes[n]];

        std::unique_ptr mesh = std::make_unique<ag::Mesh>();
        mesh->BeginApply();

        // buffer for vertex positions
		if (ai_ptr_mesh->HasPositions()) {
            std::vector<ag::vec3> vertex( ai_ptr_mesh->mNumVertices );
            for (unsigned int i = 0; i < ai_ptr_mesh->mNumVertices; ++i)  {
                //aiTransformVecByMatrix4(&mesh->mVertices[i],trafo);
                vertex[i].set(ai_ptr_mesh->mVertices[i].x, ai_ptr_mesh->mVertices[i].y, ai_ptr_mesh->mVertices[i].z);
            }
            mesh->CreateVertexVBO(vertex[0].ptr(), 3*vertex.size());
            vertex.clear();
        }

        // buffer for normals
		if (ai_ptr_mesh->HasNormals()) {
            std::vector<ag::vec3> normal(ai_ptr_mesh->mNumVertices);
            for (unsigned int i = 0; i < ai_ptr_mesh->mNumVertices; ++i) {
                //aiTransformVecByMatrix4(&mesh->mNormals[i],trafo);
                normal[i].set(ai_ptr_mesh->mNormals[i].x, ai_ptr_mesh->mNormals[i].y, ai_ptr_mesh->mNormals[i].z);
            }
            mesh->CreateNormalVBO(normal[0].ptr(), 3*normal.size());
            normal.clear();
         }

        // buffer for texture coordinates
		if (ai_ptr_mesh->HasTextureCoords(0)) {
            std::vector<ag::vec2> texel(ai_ptr_mesh->mNumVertices);
            for (GLuint i=0; i < ai_ptr_mesh->mNumVertices; ++i) {
                texel[i].set(ai_ptr_mesh->mTextureCoords[0][i].x, ai_ptr_mesh->mTextureCoords[0][i].y );
            }
            mesh->CreateTexelVBO(texel[0].ptr(), 2*texel.size());
            texel.clear();
        }

        // create array with faces have to convert from Assimp format to array
		std::vector<ag::uvec3> faceArray(ai_ptr_mesh->mNumFaces);
		for (unsigned int i=0; i<ai_ptr_mesh->mNumFaces; ++i) {
			const aiFace* face = &ai_ptr_mesh->mFaces[i];
			if (face->mNumIndices != 3) {
                log_file<<"face number indices = "<<face->mNumIndices<<"\n";
                throw ASMexception("model face not TRIANGLES");
            }
			faceArray[i].set(face->mIndices[0], face->mIndices[1], face->mIndices[2]);
        }

		mesh->CreateIndexVBO(faceArray[0].ptr(), 3*faceArray.size());
		faceArray.clear();

        mesh->_numFaces = 3*ai_ptr_mesh->mNumFaces;

	    CopyMaterial(mesh->_material, _scene->mMaterials[ai_ptr_mesh->mMaterialIndex]);

        mesh->EndApply();

        _meshs.push_back(std::move(mesh));
    }

	for (unsigned int n=0; n < nd->mNumChildren; ++n) {
		RecursiveLoad(nd->mChildren[n],trafo);
	}
	*trafo = prev;
}

void ModelAssimp::LoadTextures() {
	/* scan scene's materials for textures */
    for (unsigned int m=0; m<_scene->mNumMaterials; ++m)	{
        int texIndex = 0;
		aiString ai_name;	// filename
		while(_scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &ai_name) == AI_SUCCESS) {
			//fill map with textures, OpenGL image ids set to 0

			_textureIdMap.insert({ai_name.data, 0});

			texIndex++;
		}
	}

    for (auto& [name, idtex] : _textureIdMap) {
        std::string filename = _path + "/" + name;  // get filename
        idtex = get_textureID(filename);            // save texture id for filename in map
    }
}

void ModelAssimp::CopyMaterial(ag::Material &material, const aiMaterial *mtl) {
    assert(mtl);

	unsigned int max;	// changed: to unsigned
	int ret1, ret2;
	aiColor4D color;
	float shininess, strength;
	int two_sided;

	aiString texPath;	//contains filename of texture
    GLuint texIndex = 0;
	if (AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath)) {
        if (auto itr = _textureIdMap.find(texPath.data); itr != _textureIdMap.end()) {
            material.SetTextureRef(itr->second); 
        }
	}

	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &color))
        material.SetDiffuse(ag::color(color.r, color.g, color.b, color.a));

	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &color))
        material.SetSpecular(ag::color(color.r, color.g, color.b, color.a));

	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &color))
        material.SetAmbient(ag::color(color.r, color.g, color.b, color.a));

	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &color))
        material.SetEmission(ag::color(color.r, color.g, color.b, color.a));

	max = 1;
	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
	max = 1;
	ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
	if (ret1 == AI_SUCCESS && ret2 == AI_SUCCESS) 
        material.SetShininess(shininess * strength);
	else {
        material.SetShininess(0.0);
        material.SetSpecular(ag::color(0.0));
	}

	max = 1;
	if(AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)
                && two_sided )
        material.SetTwoSided(true);
	else
        material.SetTwoSided(false);

}

void ModelAssimp::Draw(const unsigned int program_id) const {
    if(!_ready) return;

    set_uniform_location(program_id);
    for (auto& mesh :_meshs) 
        mesh->Draw();
}

void ModelAssimp::Clear(void) {
    if (_scene) aiReleaseImport(_scene);
    _scene = nullptr;

    _meshs.clear();

    for(auto& [_, id] : _textureIdMap) {
        if(id) glDeleteTextures(1, &id);
    }
    _textureIdMap.clear();
    _ready = false;
}

void ModelAssimp::BoundingBox() {
    assert(_scene);

    aiMatrix4x4 trafo;
	aiIdentityMatrix4(&trafo);

    _min.set(std::numeric_limits<float>::max());
	_max.set(std::numeric_limits<float>::min());

	NodeBoundingBox(_scene->mRootNode, _min, _max, &trafo);

    SetCenterAndScope();
}

void ModelAssimp::NodeBoundingBox(const aiNode* nd, ag::vec3 &min, ag::vec3 &max,
            aiMatrix4x4 *trafo) {
    assert(_scene);
    assert(nd);

    aiMatrix4x4 prev = *trafo;
	aiMultiplyMatrix4(trafo, &nd->mTransformation);

	for (unsigned int n=0; n < nd->mNumMeshes; ++n) {
		const aiMesh* mesh = _scene->mMeshes[nd->mMeshes[n]];

		for (unsigned int t = 0; t < mesh->mNumVertices; ++t)		{
			aiVector3D tmp = mesh->mVertices[t];
			aiTransformVecByMatrix4(&tmp, trafo);

			min.setX(ag::math::minimum(min.x(), tmp.x));
			min.setY(ag::math::minimum(min.y(), tmp.y));
			min.setZ(ag::math::minimum(min.z(), tmp.z));

			max.setX(ag::math::maximum(max.x(), tmp.x));
			max.setY(ag::math::maximum(max.y(), tmp.y));
			max.setZ(ag::math::maximum(max.z(), tmp.z));
		}
	}

	for (unsigned int n = 0; n < nd->mNumChildren; ++n)	{
		NodeBoundingBox(nd->mChildren[n],min,max,trafo);
	}
	*trafo = prev;
}
