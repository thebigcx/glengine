#include "engine/renderer/mesh.h"

#include <iostream>

#include "engine/assimp/Importer.hpp"
#include "engine/assimp/scene.h"
#include "engine/assimp/postprocess.h"

static MeshData process_mesh(aiMesh* mesh, const aiScene* scene)
{
    MeshData mesh_data;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex3D vertex;

        vertex.pos.x = mesh->mVertices[i].x;
        vertex.pos.y = mesh->mVertices[i].y;
        vertex.pos.z = mesh->mVertices[i].z;

        if (mesh->mNormals)
        {
            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;
        }
        else
        {
            vertex.normal = Vector3f(0.0f);
        }

        if (mesh->mTextureCoords)
        {
            vertex.uv.x = mesh->mTextureCoords[0][i].x;
            vertex.uv.y = mesh->mTextureCoords[0][i].y;
        }
        else
        {
            vertex.uv = Vector2f(0.0f);
        }

        if (mesh->mTangents)
        {
            vertex.tangent.x = mesh->mTangents[i].x;
            vertex.tangent.y = mesh->mTangents[i].y;
            vertex.tangent.z = mesh->mTangents[i].z;
        }
        else
        {
            vertex.tangent = Vector3f(0.0f);
        }

        mesh_data.vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            mesh_data.indices.push_back(face.mIndices[j]);
    }

    return mesh_data;
}


MeshData AssimpLoader::load_mesh_from_file(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        std::cout << "[ASSIMP] " << importer.GetErrorString() << "\n";

    aiMesh* ai_mesh = scene->mMeshes[0];

    MeshData mesh_data = process_mesh(ai_mesh, scene);
    return mesh_data;
}