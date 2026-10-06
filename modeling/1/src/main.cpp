#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <memory>
#include <raylib-cpp/Keyboard.hpp>
#include <raylib-cpp/Vector3.hpp>
#include <raylib.h>
#include <raymath.h>
#include <utility>
#include "raylib-cpp.hpp"

class scene_event
{
public:
  template< class Callable >
  explicit scene_event(Callable f) : m_completed(false), m_body(std::move(f))
  {}

  ~scene_event() = default;

  void execute_idempotent()
  {
    if (completed())
    {
      return;
    }
    execute();
    m_completed = true;
  }

  void execute() { m_body(); }

  bool completed() const noexcept { return m_completed; }

private:
  bool m_completed;
  std::function< void() > m_body;
};


class event_line
{
public:
  event_line() : m_pos(0), m_events() {}
  bool next() noexcept
  {
    assert(m_pos < m_events.size() && "m_pos cant be greater than event count");
    if (m_events.empty() || m_pos + 1 == m_events.size())
    {
      return false;
    }
    ++m_pos;
    return true;
  }

  void add_event(std::unique_ptr< scene_event > e)
  {
    m_events.push_back(std::move(e));
  }

  void execute_current()
  {
    if (m_events.empty() || m_pos >= m_events.size())
    {
      throw std::runtime_error("Empty event line execution");
    }

    auto& e = m_events[m_pos];
    e->execute_idempotent();
  }

  bool completed_current() const { return m_events[m_pos]->completed(); }

  void reset()
  {
    m_pos = 0;
    m_events = decltype(m_events){};
  }

private:
  size_t m_pos;
  std::vector< std::unique_ptr< scene_event > > m_events;
};

class scene_config
{
public:
  using config_internal = struct
  {
    float cube_size;
    float sphere_radius;
    float small_cylinder_radius;
    float large_cylinder_radius;
    float small_cylinder_height;
    float large_cylinder_height;
    float angle;
    float camera_fov;
    float cylinder_shift;
    raylib::Vector3 sphere_pos;
    raylib::Vector3 small_cylinder_pos;
    raylib::Vector3 cube_pos;
    raylib::Vector3 large_cylinder_pos;
    raylib::Vector3 camera_pos;
    raylib::Vector3 camera_focus_point;
    raylib::Vector3 camera_up_direction;
    event_line events;
  };

  // NOTE: why protected
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
handle_input()
{
  auto config = scene_config::get_instance();
  if (raylib::Keyboard::IsKeyReleased(KEY_ENTER))
  {
    config->events.execute_current();
    if (config->events.completed_current())
    {
      config->events.next();
    }
  }
}

int
main()
{
  auto config = scene_config::get_instance();

  config->cube_size = 2.0f;
  config->sphere_radius = config->cube_size / 2.0f;
  config->small_cylinder_radius = 0.6f;
  config->large_cylinder_radius = 1.5f;
  config->small_cylinder_height = 1.2f;
  config->large_cylinder_height = 2.0f;
  config->cylinder_shift = 1.5f;
  config->angle = 0.0f;
  config->small_cylinder_pos = Vector3{ 3.0f, config->large_cylinder_height, 0.0f };
  config->large_cylinder_pos = Vector3{ 3.0f, 0.0f, 0.0f };
  config->cube_pos = Vector3{ -4.0f, 1.0f, 0.0f };
  config->sphere_pos = config->cube_pos;
  config->camera_pos = Vector3{ 0.0f, 6.0f, 12.0f };
  config->camera_focus_point = Vector3{ 0.0f, 1.0f, 0.0f };
  config->camera_up_direction = Vector3{ 0.0f, 1.0f, 0.0f };
  config->camera_fov = 45.0f;

  config->events.add_event(std::make_unique< scene_event >(
    []()
    {
      auto config = scene_config::get_instance();
      config->sphere_radius = config->sphere_radius * 2.5f;
    }));

  config->events.add_event(std::make_unique< scene_event >(
    []()
    {
      auto config = scene_config::get_instance();
      config->small_cylinder_pos.x += config->cylinder_shift;
    }));

  config->events.add_event(std::make_unique< scene_event >(
    []()
    {
      auto config = scene_config::get_instance();
      config->angle = 45.0f;
    }));

  const int screenWidth = 1000;
  const int screenHeight = 600;
  raylib::Window window(screenWidth, screenHeight, "Modeling 1");

  raylib::Camera3D camera(config->camera_pos,
                          config->camera_focus_point,
                          config->camera_up_direction,
                          config->camera_fov,
                          CAMERA_PERSPECTIVE);

  window.SetTargetFPS(60);

  camera.Update(CAMERA_ORBITAL);
  while (!window.ShouldClose())
  {
    handle_input();

    window.BeginDrawing();
    window.ClearBackground(DARKGRAY);

    camera.BeginMode();

    DrawGrid(20, 1.0f);

    config->cube_pos.DrawCubeWires(
      config->cube_size, config->cube_size, config->cube_size, BLUE);

    config->sphere_pos.DrawSphereWires(config->sphere_radius, 16, 16, RED);

    rlPushMatrix();
    rlTranslatef(config->large_cylinder_pos.x,
                 config->large_cylinder_pos.y,
                 config->large_cylinder_pos.z);

    rlRotatef(config->angle, 0.0f, 1.0f, 0.0f);

    DrawCylinderWires(::Vector3{ 0, 0, 0 },
                      config->large_cylinder_radius,
                      config->large_cylinder_radius,
                      config->large_cylinder_height,
                      12,
                      LIME);
    rlPopMatrix();

    DrawCylinderWires(config->small_cylinder_pos,
                      config->small_cylinder_radius,
                      config->small_cylinder_radius,
                      config->small_cylinder_height,
                      12,
                      ORANGE);

    camera.EndMode();

    window.EndDrawing();
  }

  return 0;
}
