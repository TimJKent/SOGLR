#include "SOGLR/SOGLR.hpp"
#include <filesystem>

int main()
{
    // Render maintains its own window
    SOGLR::Renderer renderer;
    renderer.GetWindow()->SetTitle("2_DrawingATriangle");
    renderer.GetWindow()->Resize(600, 600);

    // Initialize the geometry in the GPU
    std::vector<SOGLR::Vertex> triangle{
        {-0.5f, -0.5, 0.0f},
        {0.5f, -0.5, 0.0f},
        {0.0f, 0.5f, 0.0f}};

    SOGLR::VertexArrayObject vao;
    SOGLR::VertexBuffer vb(triangle);
    SOGLR::IndexBuffer ib({0, 2, 1});
    vao.AddVertexBuffer(vb);
    vao.SetIndexBuffer(ib);

    // Create shader
    std::string source_dir = std::filesystem::path(__FILE__).parent_path().string();
    std::string vert_file = source_dir + "/shaders/simple_triangle.vert";
    std::string frag_file = source_dir + "/shaders/simple_triangle.frag";
    SOGLR::Shader simple_triangle_shader(vert_file, frag_file);

    while (renderer.IsRunning())
    {
        renderer.BeginFrame();
        simple_triangle_shader.Bind();
        renderer.DrawIndexed(vao);
        renderer.EndFrame();
    }

    return 0;
}