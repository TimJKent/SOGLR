#include "SOGLR/SOGLR.hpp"
#include "SOGLR/etc/PerspectiveCameraController.hpp"

#include <filesystem>
#include <iostream>

int main()
{
    SOGLR::Window window(800, 600, "7_RenderText");
    std::string source_dir = std::filesystem::path(__FILE__).parent_path().string();
    std::string font_file = source_dir + "/assets/fonts/NotoSansDisplay-Regular.ttf";

    
    std::string vert_file = source_dir + "/shaders/text.vert";
    std::string frag_file = source_dir + "/shaders/text.frag";
    std::shared_ptr<SOGLR::Shader> text_shader = std::make_shared<SOGLR::Shader>(vert_file, frag_file);
    
    SOGLR::Text::FontLoader font_loader;
    auto font = font_loader.LoadFont(font_file);
    if (!font)
    {
        std::cerr << "Failed to load font: " << font.error() << std::endl;
        return -1;
    }
    
    SOGLR::Scene scene;
    SOGLR::PerspectiveCameraController camera_controller;
    camera_controller.GetCamera()->GetTransform().position = glm::vec3(0.0f, 0.0f, 3.0f);
    scene.SetSceneCamera(camera_controller.GetCamera());

    // Setup scene lighting
    std::shared_ptr<SOGLR::Lighting::DirectionalLight> light1 = std::make_shared<SOGLR::Lighting::DirectionalLight>();
    light1->color = glm::vec3(1.0f, 1.0, 1.0);
    light1->intensity = 1.0f;
    light1->direction = glm::vec3(0.0, -0.95f, -0.05f);
    scene.SetDirectionalLight(light1);

    SOGLR::Text::TextRenderer text_renderer(text_shader, font.value(), scene);

    text_renderer.SetText("Hello, SOGLR!");

    while (!window.ShouldClose())
    {
        window.BeginFrame();
        scene.DrawScene();
        window.EndFrame();
    }

    return 0;
}