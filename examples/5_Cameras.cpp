#include "SOGLR/SOGLR.hpp"

#include <filesystem>
#include <memory>

#include "SOGLR/etc/PerspectiveCameraController.hpp"

/* Perspective Camera Controls:
    W - forward
    S - backward
    A - left
    D - right
    R - up
    F - down
    E - rotate right
    Q - rotate left
*/

int main()
{
    SOGLR::Renderer renderer;
    renderer.GetWindow()->SetTitle("5_Cameras");
    renderer.GetWindow()->Resize(600, 600);

    SOGLR::VertexArrayObject vao;
    SOGLR::VertexBuffer vb(SOGLR::Primitives::Quad::vertices);
    SOGLR::IndexBuffer ib(SOGLR::Primitives::Quad::indicies);
    vao.AddVertexBuffer(vb);
    vao.SetIndexBuffer(ib);

    // Create shader
    std::string source_dir = std::filesystem::path(__FILE__).parent_path().string();
    std::string vert_file = source_dir + "/shaders/unlit.vert";
    std::string frag_file = source_dir + "/shaders/unlit.frag";
    SOGLR::Shader lit_shader(vert_file, frag_file);

    std::shared_ptr<SOGLR::Texture> texture = std::make_shared<SOGLR::Texture>(source_dir + "/assets/wall.jpg", SOGLR::Texture::TEXTURE_TYPE::DIFFUSE_RGBA);

    SOGLR::PerspectiveCameraController perspective_camera;

    perspective_camera.SetPosition({0, 0, 2.0});

    while (renderer.IsRunning())
    {
        renderer.BeginFrame();

        perspective_camera.Update(*renderer.GetWindow(), renderer.GetDeltaTime());
        lit_shader.Bind();
        lit_shader.SetInt("ourTexture", 0);
        lit_shader.SetUBOMatrices(perspective_camera.GetCamera()->GetProjectionMatrix(), perspective_camera.GetCamera()->GetViewMatrix(), perspective_camera.GetCamera()->GetTransform().position, glm::vec3(1), glm::vec3(1), 1.0f);
        lit_shader.SetUniformMat4f("uModel", glm::mat4(1.0f));
        texture->Bind();
        renderer.DrawIndexed(vao);
        renderer.EndFrame();
    }

    return 0;
}