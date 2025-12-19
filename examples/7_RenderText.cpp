#include "SOGLR/SOGLR.hpp"
#include "SOGLR/etc/PerspectiveCameraController.hpp"

#include <filesystem>
#include <iostream>

int main()
{
    SOGLR::Renderer renderer;
    renderer.GetWindow()->SetTitle("7_RenderText");

    std::string source_dir = std::filesystem::path(__FILE__).parent_path().string();
    std::string font_file = source_dir + "/assets/fonts/NotoSansDisplay-Regular.ttf";
    
    std::string vert_file = source_dir + "/shaders/text.vert";
    std::string frag_file = source_dir + "/shaders/text.frag";
    std::shared_ptr<SOGLR::Shader> text_shader = std::make_shared<SOGLR::Shader>(vert_file, frag_file);
    
    SOGLR::Text::FontLoader font_loader;
    auto font = font_loader.LoadFont(font_file, 24);
    if (!font)
    {
        std::cerr << "Failed to load font: " << font.error() << std::endl;
        return -1;
    }
    
    SOGLR::Scene scene;
    SOGLR::PerspectiveCameraController camera_controller;
    camera_controller.GetCamera()->GetTransform().position = glm::vec3(0.0f, 0.0f, 3.0f);
    scene.SetSceneCamera(camera_controller.GetCamera());
    
    SOGLR::Text::TextRenderer text_renderer(text_shader, font.value(), scene);
    SOGLR::Text::TextRenderer text_renderer2(text_shader, font.value(), scene);
    text_renderer.GetTextObject()->GetTransform().position = glm::vec3(0.0f, 24.0f, 0.0f);
    text_renderer.SetText("Hello, SOGLR!");
    text_renderer2.SetText("Hello, SOGLR!");

    while (renderer.IsRunning())
    {
        renderer.BeginFrame();
        scene.DrawScene(nullptr, renderer.GetWindow()->GetSize());
        text_renderer.SetText("FPS: " + std::to_string(static_cast<int>(1.0f / renderer.GetDeltaTime())));
        text_renderer2.SetText("MS: " + std::to_string(renderer.GetDeltaTime() * 1000.0f));
        renderer.EndFrame();
    }

    return 0;
}