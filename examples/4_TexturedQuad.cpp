#include "SOGLR/SOGLR.hpp"
#include <filesystem>
#include <memory>

int main()
{
    SOGLR::Renderer renderer;
    renderer.GetWindow()->SetTitle("4_TexturedQuad");
    renderer.GetWindow()->Resize(600, 600);

    SOGLR::VertexArrayObject vao;
    SOGLR::VertexBuffer vb(SOGLR::Primitives::Quad::vertices);
    SOGLR::IndexBuffer ib(SOGLR::Primitives::Quad::indicies);
    vao.AddVertexBuffer(vb);
    vao.SetIndexBuffer(ib);

    // Create shader
    std::string source_dir = std::filesystem::path(__FILE__).parent_path().string();
    std::string vert_file = source_dir + "/shaders/textured_quad.vert";
    std::string frag_file = source_dir + "/shaders/textured_quad.frag";
    SOGLR::Shader textured_quad_shader(vert_file, frag_file);

    std::shared_ptr<SOGLR::Texture> texture = std::make_shared<SOGLR::Texture>(source_dir + "/assets/wall.jpg", SOGLR::Texture::TEXTURE_TYPE::DIFFUSE_RGBA);

    while (renderer.IsRunning())
    {
        renderer.BeginFrame();
        textured_quad_shader.Bind();
        textured_quad_shader.SetInt("ourTexture", 0);
        texture->Bind();
        renderer.DrawIndexed(vao);
        renderer.EndFrame();
    }

    return 0;
}