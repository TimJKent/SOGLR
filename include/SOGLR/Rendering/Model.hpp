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

namespace SOGLR
{
    class Model
    {
    public:
        Model(const std::string &file_path)
            : file_path_(file_path), file_directory_(file_path.substr(0, file_path.find_last_of('\\')))
        {

#ifdef PROFILE
            std::chrono::time_point<std::chrono::high_resolution_clock> model_load_start = std::chrono::high_resolution_clock::now();
#endif
            Assimp::Importer importer;
            const aiScene *scene = importer.ReadFile(file_path_,
                                                     aiProcess_Triangulate |
                                                         aiProcess_FlipUVs |
                                                         aiProcess_CalcTangentSpace |
                                                         aiProcess_GenSmoothNormals |
                                                         aiProcess_JoinIdenticalVertices |
                                                         aiProcess_ImproveCacheLocality);

            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                std::cerr << "Failed to load model: " << file_path_ << std::endl;
                return;
            }

            ProcessNode(scene->mRootNode, scene);
#ifdef PROFILE
            std::cout
                << "Total Loading Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - model_load_start).count() << " ms" << std::endl;
#endif
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
            std::vector<std::shared_ptr<Texture>> textures;

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

                float tex_x = 0.0f;
                float tex_y = 0.0f;

                if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
                {
                    tex_x = mesh->mTextureCoords[0][i].x;
                    tex_y = mesh->mTextureCoords[0][i].y;
                }

                float tan_x = 0.0f;
                float tan_y = 0.0f;
                float tan_z = 0.0f;
                float bittan_x = 0.0f;
                float bittan_y = 0.0f;
                float bittan_z = 0.0f;

                if (mesh->HasTangentsAndBitangents())
                {
                    tan_x = mesh->mTangents[i].x;
                    tan_y = mesh->mTangents[i].y;
                    tan_z = mesh->mTangents[i].z;
                    bittan_x = mesh->mBitangents[i].x;
                    bittan_y = mesh->mBitangents[i].y;
                    bittan_z = mesh->mBitangents[i].z;
                }

                vertices.emplace_back(
                    Vertex{
                        mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z,
                        normal_x, normal_y, normal_z,
                        tex_x, tex_y,
                        tan_x, tan_y, tan_z,
                        bittan_x, bittan_y, bittan_z});
            }

            for (uint32_t i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                for (uint32_t j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }

            if (mesh->mMaterialIndex >= 0)
            {
                aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
                std::vector<std::shared_ptr<Texture>> diff_textures = LoadMaterialTextures(material, aiTextureType_DIFFUSE, Texture::TEXTURE_TYPE::DIFFUSE_RGBA);
                textures.insert(textures.end(), diff_textures.begin(), diff_textures.end());
                std::vector<std::shared_ptr<Texture>> norm_textures = LoadMaterialTextures(material, aiTextureType_DISPLACEMENT, Texture::TEXTURE_TYPE::NORMAL);
                textures.insert(textures.end(), norm_textures.begin(), norm_textures.end());
            }
            return Mesh(vertices, indices, textures);
        }

        std::vector<std::shared_ptr<Texture>>
        LoadMaterialTextures(aiMaterial *mat, aiTextureType ai_type, Texture::TEXTURE_TYPE type)
        {
            std::vector<std::shared_ptr<Texture>> textures;
            for (unsigned int i = 0; i < mat->GetTextureCount(ai_type); i++)
            {
                aiString str;
                mat->GetTexture(ai_type, i, &str);
                std::string file_path = file_directory_;
                file_path += "\\" + std::string(str.C_Str());
                textures.emplace_back(std::make_shared<Texture>(file_path.c_str(), type));
            }
            return textures;
        }

        void Draw(std::shared_ptr<Shader> &shader)
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
        std::vector<Mesh> meshes;
        std::string file_path_;
        std::string file_directory_;
    };
};