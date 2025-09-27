#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "glm/glm.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Rendering/Mesh.hpp"
#include "Rendering/Shader.hpp"

namespace Rendering
{
    class Model
    {
    public:
        Model(const std::string &path)
        {
            Assimp::Importer importer;
            const aiScene *scene = importer.ReadFile(path,
                                                     aiProcess_Triangulate |
                                                         aiProcess_FlipUVs |
                                                         aiProcess_CalcTangentSpace |
                                                         aiProcess_GenSmoothNormals |
                                                         aiProcess_JoinIdenticalVertices |
                                                         aiProcess_ImproveCacheLocality);

            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                std::cerr << "Failed to load model: " << path << std::endl;
                return;
            }

            ProcessNode(scene->mRootNode, scene);
        }

        void AddMesh(const Mesh &mesh)
        {
            meshes.push_back(mesh);
        }

        Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene)
        {
            std::vector<Vertex> vertices;
            vertices.reserve(mesh->mNumVertices);
            std::vector<unsigned int> indices;
            indices.reserve(mesh->mNumFaces * 3);
            std::vector<Texture> textures;

            for (unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                float normal_x = 0.0f;
                float normal_y = 0.0f;
                float normal_z = 0.0f;

                if (mesh->HasNormals())
                {
                    normal_x = mesh->mNormals[i].x;
                    normal_y = mesh->mNormals[i].y;
                    normal_z = mesh->mNormals[i].z;
                }

                // Texture coordinates
                // if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
                //{
                //    glm::vec2 vec;
                //    vec.x = mesh->mTextureCoords[0][i].x;
                //    vec.y = mesh->mTextureCoords[0][i].y;
                //    vertex.texCoords = vec;
                //}
                // else
                //{
                //    vertex.texCoords = glm::vec2(0.0f, 0.0f);
                //}

                vertices.emplace_back(
                    Vertex{
                        mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z,
                        normal_x, normal_y, normal_z,
                        0.0f, 0.0f});
            }
            for (uint32_t i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                for (uint32_t j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }

            return Mesh(vertices, indices, textures);
        }

        void Draw(std::shared_ptr<Rendering::Shader> &shader)
        {
            for (uint32_t i = 0; i < meshes.size(); i++)
                meshes[i].Draw(*shader.get());
        }

        void ProcessNode(aiNode *node, const aiScene *scene)
        {

            // then do the same for each of its children
            for (unsigned int i = 0; i < node->mNumChildren; i++)
            {
                ProcessNode(node->mChildren[i], scene);
            }
            // process all the node's meshes (if any)
            for (unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
                meshes.push_back(ProcessMesh(mesh, scene));
            }
        }

    private:
        // model data
        std::vector<Mesh> meshes;
        std::string directory;
    };
};