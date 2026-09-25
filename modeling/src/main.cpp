#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <raylib-cpp/Keyboard.hpp>
#include <raylib-cpp/Vector3.hpp>
#include <raylib.h>
#include <raymath.h>
#include <stdexcept>
#include <string_view>
#include <utility>
#include "raylib-cpp.hpp"

namespace sandbox
{
  struct defaults
  {
    constexpr static inline size_t min_log_length = 1024;
    constexpr static inline int single_shader_source = 1;
    constexpr static inline std::nullptr_t no_length_array = nullptr;
  };
}

namespace sandbox::util
{
  template< class char_t, size_t N >
  using on_stack_string_t = std::array< char_t, N >;
  using on_stack_string = on_stack_string_t< char, 1024 >;

  template< class string_t = on_stack_string >
  class string_format
  {
  public:
    string_format() = default;

    template< class... Args >
    string_format(std::string_view format, Args... args) : string_format()
    {
      update_format(format, args...);
    }

    const char* data() const noexcept { return string_.data(); }

    // TODO: make more reusable
    template< class... Args >
    void set_format(std::string_view format, Args... args)
    {
      update_format(format, args...);
    }

  private:
    string_t string_;

    template< class... Args >
    void update_format(std::string_view format, Args... args)
    {
      std::snprintf(string_.data(), string_.size(), format.data(), args...);
    }
  };

  template< class string_t = on_stack_string >
  std::ostream& operator<<(std::ostream& out,
                           const string_format< string_t >& format)
  {
    return out << format.data();
  }

  template< class Callable >
  struct call_on_destruction
  {
    call_on_destruction(Callable to_call) : to_call_(std::move(to_call)) {}

    ~call_on_destruction() { to_call_(); }

  private:
    Callable to_call_;
  };

  struct assert
  {
    static void throw_on_false(bool expr, const std::string& message)
    {
      if (!expr)
      {
        throw std::runtime_error(message);
      }
    }
  };
}

struct abstract_scene_event
{
  virtual void execute_idempotent() = 0;
  virtual void execute() = 0;
  virtual bool completed() const noexcept = 0;
  virtual ~abstract_scene_event() = default;
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

  void add_event(std::unique_ptr< abstract_scene_event > e)
  {
    m_events.push_back(std::move(e));
  }

  bool execute_current()
  {
    if (m_events.empty() || m_pos >= m_events.size())
    {
      return false;
    }

    auto& e = m_events[m_pos];
    if (e->completed())
    {
      return false;
    }
    e->execute();
    return true;
  }

  void reset()
  {
    m_pos = 0;
    m_events = decltype(m_events){};
  }

private:
  size_t m_pos;
  std::vector< std::unique_ptr< abstract_scene_event > > m_events;
};

class scene_event: public abstract_scene_event
{
public:
  template< class Callable >
  explicit scene_event(Callable f) : m_completed(false), m_body(std::move(f))
  {}

  ~scene_event() = default;

  void execute_idempotent() override
  {
    if (completed())
    {
      return;
    }
    execute();
  }

  void execute() override { m_body(); }

  bool completed() const noexcept override { return m_completed; }

private:
  bool m_completed;
  std::function< void() > m_body;
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
    config->events.next();
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
  config->small_cylinder_pos = Vector3{ 3.0f, 2.6f, 0.0f };
  config->large_cylinder_pos = Vector3{ 3.0f, 1.0f, 0.0f };
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

  // Инициализация окна
  const int screenWidth = 1000;
  const int screenHeight = 600;
  raylib::Window window(screenWidth, screenHeight, "Modeling 1");

  // Настройка 3D-камеры
  raylib::Camera3D camera(
    config->camera_pos,          // Позиция камеры
    config->camera_focus_point,  // Точка, на которую смотрит камера
    config->camera_up_direction, // Направление "верх" для камеры
    config->camera_fov,          // Угол обзора (FOV)
    CAMERA_PERSPECTIVE           // Тип проекции
  );

  window.SetTargetFPS(60);

  // camera.Update(CAMERA_ORBITAL);
  while (!window.ShouldClose())
  {
    handle_input();
    // Обновление камеры (управление мышью/клавиатурой для осмотра сцены)
    camera.Update(CAMERA_ORBITAL);

    window.BeginDrawing();
    window.ClearBackground(DARKGRAY);

    camera.BeginMode();

    // Рисуем сетку на полу для визуального ориентирования в пространстве
    DrawGrid(20, 1.0f);

    // =================================================================
    // ЗАДАНИЕ 1 и 2: Каркасный куб и масштабированная каркасная сфера
    // Сместим эту группу влево (X = -4.0f)
    // =================================================================

    // 1. Рисуем каркасный куб
    config->cube_pos.DrawCubeWires(
      config->cube_size, config->cube_size, config->cube_size, BLUE);

    // 2. Изначально вписанная сфера имела бы радиус = cubeSize / 2 = 1.0f
    // Масштабируем её с коэффициентом 2.5 -> новый радиус = 1.0f * 2.5f = 2.5f
    config->sphere_pos.DrawSphereWires(config->sphere_radius, 16, 16, RED);

    // =================================================================
    // ЗАДАНИЕ 3 и 4: Большой и малый цилиндры
    // Сместим эту группу вправо (X = 3.0f)
    // =================================================================

    // Начальная позиция малого цилиндра (прямо поверх большого):
    // Y = Y_большого + (Высота_большого / 2) + (Высота_малого / 2) = 1.0 + 1.0
    // + 0.6 = 2.6f

    // 4. Модификации цилиндров:
    // А) Сдвигаем малый цилиндр по оси X (например, на +1.5 единицы вправо
    // относительно общей позиции)

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
    // Рисуем сдвинутый малый цилиндр
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
