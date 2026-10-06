#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <raygui.h>
#include <raylib-cpp/Keyboard.hpp>
#include <raylib-cpp/Texture.hpp>
#include <raylib-cpp/Vector3.hpp>
#include <raylib.h>
#include <raymath.h>
#include <rlights.h>
#include "raylib-cpp.hpp"

class scene_config
{
public:
  using config_internal = struct
  {
    bool draw_sliders;
    float cube_size;
    float sphere_radius;
    float light_radius;
    float min_light_radius;
    float max_light_radius;
    float light_height;
    float min_light_height;
    float max_light_height;
    float shift_sector_angle;
    float min_shift_sector_angle;
    float max_shift_sector_angle;
    float large_cylinder_radius;
    float large_cylinder_height;
    float camera_fov;
    float light_intensity;
    float min_light_intensity;
    float max_light_intensity;
    raylib::Vector3 sphere_pos;
    raylib::Vector3 cube_pos;
    raylib::Vector3 large_cylinder_pos;
    raylib::Vector3 light_current_pos;
    raylib::Vector3 camera_pos;
    raylib::Vector3 camera_focus_point;
    raylib::Vector3 camera_up_direction;
    raylib::Color light_color;
  };

protected:
  scene_config() = default;

public:
  static std::shared_ptr< config_internal > get_instance()
  {
    if (!scene_config::instance_)
    {
      scene_config::instance_ = std::make_shared< config_internal >();
    }
    return scene_config::instance_;
  }

private:
  static inline std::shared_ptr< config_internal > instance_;
};

void
handle_inputs()
{
  if (raylib::Keyboard::IsKeyReleased(KEY_TAB))
  {
    auto config = scene_config::get_instance();
    config->draw_sliders = !config->draw_sliders;
  }
}

int
main()
{
  auto config = scene_config::get_instance();

  config->cube_size = 2.0f;
  config->sphere_radius = config->cube_size / 2.0f;
  config->large_cylinder_radius = 1.5f;
  config->large_cylinder_height = 2.0f;
  config->large_cylinder_pos = Vector3{ 2.0f, 0.0f, 0.0f };
  config->cube_pos = Vector3{ -2.0f, 0.0f, 0.0f };
  config->sphere_pos = Vector3{ config->cube_pos.x + config->cube_size / 2,
                                config->cube_pos.y + config->cube_size,
                                config->cube_pos.z + config->cube_size / 2 };
  config->camera_pos = Vector3{ 0.0f, 5.0f, 22.0f };
  config->camera_focus_point = Vector3{ 0.0f, 0.0f, 0.0f };
  config->camera_up_direction = Vector3{ 0.0f, 1.0f, 0.0f };
  config->camera_fov = 40.0f;
  config->light_radius = 10.0f;
  config->min_light_radius = 5.0f;
  config->max_light_radius = 30.0f;
  config->light_height = 0.0f;
  config->min_light_height = -20.0f;
  config->max_light_height = 20.0f;
  config->light_color = Color{ 255, 255, 255, 255 };
  config->min_shift_sector_angle = 0.0f;
  config->shift_sector_angle = config->min_shift_sector_angle;
  config->max_shift_sector_angle = 360.0f;
  config->light_intensity = 1.0f;
  config->min_light_intensity = 0.0f;
  config->max_light_intensity = 5.0f;

  constexpr int screenWidth = 1000;
  constexpr int screenHeight = 600;

  raylib::Window window(screenWidth, screenHeight, "Modeling 1");

  raylib::Camera3D camera(config->camera_pos,
                          config->camera_focus_point,
                          config->camera_up_direction,
                          config->camera_fov,
                          CAMERA_PERSPECTIVE);

  raylib::Shader light_shader = LoadShader("resources/shaders/lighting.vs",
                                           "resources/shaders/lighting.fs");
  light_shader.locs[SHADER_LOC_VECTOR_VIEW] =
    GetShaderLocation(light_shader, "viewPos");

  float ambient_value[4] = { 0.15f, 0.15f, 0.15f, 1.0f };
  int ambientLoc = GetShaderLocation(light_shader, "ambient");
  SetShaderValue(light_shader, ambientLoc, ambient_value, SHADER_UNIFORM_VEC4);

  float camera_view_pos[3] = { camera.position.x,
                               camera.position.y,
                               camera.position.z };
  SetShaderValue(light_shader,
                 light_shader.locs[SHADER_LOC_VECTOR_VIEW],
                 camera_view_pos,
                 SHADER_UNIFORM_VEC3);

  // Get Uniform locations for the custom material properties
  int shininessLoc = GetShaderLocation(light_shader, "materialShininess");
  int glossLoc = GetShaderLocation(light_shader, "materialGloss");
  int alphaLoc = GetShaderLocation(light_shader, "materialAlpha");
  int intensityLoc = GetShaderLocation(light_shader, "lightIntensity");

  raylib::Model cube_model = LoadModelFromMesh(
    GenMeshCube(config->cube_size, config->cube_size, config->cube_size));
  raylib::Model sphere_model =
    LoadModelFromMesh(GenMeshSphere(config->sphere_radius, 32, 32));
  raylib::Model cylinder_model = LoadModelFromMesh(GenMeshCylinder(
    config->large_cylinder_radius, config->large_cylinder_height, 32));

  cube_model.materials[0].shader = light_shader;
  sphere_model.materials[0].shader = light_shader;
  cylinder_model.materials[0].shader = light_shader;

  cube_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTexture(
    "resources/textures/paving-stone/PavingStones128_1K-PNG_Color.png");
  // cube_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = BLUE;
  sphere_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = RED;
  cylinder_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = LIME;

  Light lights[MAX_LIGHTS] = { 0 };
  lights[0] = CreateLight(LIGHT_POINT,
                          Vector3{ 0, 0, 0 },
                          Vector3Zero(),
                          config->light_color,
                          light_shader);

  window.SetTargetFPS(60);

  while (!window.ShouldClose())
  {
    handle_inputs();

    float radians = config->shift_sector_angle * DEG2RAD;
    config->light_current_pos.x = config->light_radius * cosf(radians);
    config->light_current_pos.z = config->light_radius * sinf(radians);
    config->light_current_pos.y = config->light_height;

    lights[0].position = config->light_current_pos;
    lights[0].color = config->light_color;
    UpdateLightValues(light_shader, lights[0]);

    SetShaderValue(light_shader,
                   intensityLoc,
                   &config->light_intensity,
                   SHADER_UNIFORM_FLOAT);

    window.BeginDrawing();
    window.ClearBackground(DARKGRAY);

    camera.BeginMode();

    float cubeShininess = 1.0f;
    float cubeGloss = 0.2f;
    float cubeAlpha = 1.0f;
    SetShaderValue(
      light_shader, shininessLoc, &cubeShininess, SHADER_UNIFORM_FLOAT);
    SetShaderValue(light_shader, glossLoc, &cubeGloss, SHADER_UNIFORM_FLOAT);
    SetShaderValue(light_shader, alphaLoc, &cubeAlpha, SHADER_UNIFORM_FLOAT);
    cube_model.Draw(config->cube_pos);

    float cylShininess = 64.0f;
    float cylGloss = 1.0f;
    float cylAlpha = 1.0f;
    SetShaderValue(
      light_shader, shininessLoc, &cylShininess, SHADER_UNIFORM_FLOAT);
    SetShaderValue(light_shader, glossLoc, &cylGloss, SHADER_UNIFORM_FLOAT);
    SetShaderValue(light_shader, alphaLoc, &cylAlpha, SHADER_UNIFORM_FLOAT);
    cylinder_model.Draw(config->large_cylinder_pos);

    BeginBlendMode(BLEND_ALPHA);
    float sphereShininess = 16.0f;
    float sphereGloss = 0.5f;
    float sphereAlpha = 0.6f;
    SetShaderValue(
      light_shader, shininessLoc, &sphereShininess, SHADER_UNIFORM_FLOAT);
    SetShaderValue(light_shader, glossLoc, &sphereGloss, SHADER_UNIFORM_FLOAT);
    SetShaderValue(light_shader, alphaLoc, &sphereAlpha, SHADER_UNIFORM_FLOAT);
    sphere_model.Draw(config->sphere_pos);
    EndBlendMode();

    // Lightbulb visualization node
    config->light_current_pos.DrawSphere(0.2f, 8, 8, config->light_color);

    camera.EndMode();

    if (config->draw_sliders)
    {
      GuiColorPicker(Rectangle{ 10, 10, 170, 80 },
                     "Pick light color",
                     std::addressof(config->light_color));
      GuiSlider(Rectangle{ 220, 10, 400, 20 },
                nullptr,
                "Light height",
                &config->light_height,
                config->min_light_height,
                config->max_light_height);
      GuiSlider(Rectangle{ 220, 40, 400, 20 },
                nullptr,
                "Light radius",
                &config->light_radius,
                config->min_light_radius,
                config->max_light_radius);
      GuiSlider(Rectangle{ 220, 70, 400, 20 },
                nullptr,
                "Light angle",
                &config->shift_sector_angle,
                config->min_shift_sector_angle,
                config->max_shift_sector_angle);

      GuiSlider(Rectangle{ 220, 100, 400, 20 },
                nullptr,
                "Intensity",
                &config->light_intensity,
                config->min_light_intensity,
                config->max_light_intensity);
    }
    window.EndDrawing();
  }

  return 0;
}
