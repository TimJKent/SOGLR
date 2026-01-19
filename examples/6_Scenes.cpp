#pragma once

#include <filesystem>

#include "SOGLR/SOGLR.hpp"
#include "SOGLR/etc/PerspectiveCameraController.hpp"

int main()
{
    SOGLR::Renderer renderer;
    renderer.GetWindow()->SetTitle("6_Scenes");
    renderer.GetWindow()->Resize(600, 600);

    SOGLR::Scene scene;

    SOGLR::PerspectiveCameraController camera_controller;
    scene.SetSceneCamera(camera_controller.GetCamera());

    std::string source_dir = std::filesystem::path(__FILE__).parent_path().string();
    std::shared_ptr<SOGLR::Shader> lit_shader = std::make_shared<SOGLR::Shader>(source_dir + "/shaders/lit.vert", source_dir + "/shaders/lit.frag");
    std::shared_ptr<SOGLR::Shader> framebuffer_shader = std::make_shared<SOGLR::Shader>(source_dir + "/shaders/framebuffer.vert", source_dir + "/shaders/framebuffer.frag");
    std::shared_ptr<SOGLR::Shader> depth_shader = std::make_shared<SOGLR::Shader>(source_dir + "/shaders/depth.vert", source_dir + "/shaders/depth.frag");

    // Add Render Object to Scene
    std::shared_ptr<SOGLR::Model> sponza_mesh = std::make_shared<SOGLR::Model>(source_dir + "/assets/models/sponza/sponza.obj");
    std::shared_ptr<SOGLR::RenderObject> obj1 = std::make_shared<SOGLR::RenderObject>();
    obj1->SetModel(sponza_mesh);
    obj1->SetShader(lit_shader);
    // Sponza is a huge model. Scaling it down helps it fit with other models / our perspective camera's default clipping planes
    obj1->GetTransform().scale = glm::vec3(0.1f);
    scene.AddRenderObject(obj1);

    // Setup scene lighting
    std::shared_ptr<SOGLR::Lighting::DirectionalLight> light1 = std::make_shared<SOGLR::Lighting::DirectionalLight>();
    light1->color = glm::vec3(1.0f, 1.0, 1.0);
    light1->intensity = 1.0f;
    light1->direction = glm::vec3(0.0, -0.95f, -0.05f);
    scene.SetDirectionalLight(light1);

    std::shared_ptr<SOGLR::Framebuffer> framebuffer = std::make_shared<SOGLR::Framebuffer>(600, 600);
    std::shared_ptr<SOGLR::Advanced::Shadowmap> shadow_map = std::make_shared<SOGLR::Advanced::Shadowmap>(4096, 4096);
    shadow_map->SetDirectionalLight(light1);
    float time = 0.0f;
    while (renderer.IsRunning())
    {
        time += renderer.GetDeltaTime();
        light1->direction = glm::vec3(0.0f, -0.95f, sin(time));
        renderer.BeginFrame();
        camera_controller.Update(*renderer.GetWindow(), renderer.GetDeltaTime());
        renderer.DrawSceneToFramebuffer(scene, framebuffer, shadow_map, depth_shader);
        renderer.DrawFramebufferToScreen(framebuffer, framebuffer_shader);
        renderer.EndFrame();
    }

    return 0;
}