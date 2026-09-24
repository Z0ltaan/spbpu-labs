#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <stdexcept>
#include <string_view>
#include <utility>

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

template< class EventType >
struct event_line
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

  void add_event(const EventType& e) { m_events.push_back(e); }

  bool execute_current()
  {
    if (m_events.empty() || m_pos >= m_events.size())
    {
      return false;
    }

    auto& e = m_events[m_pos];
    if (e.completed())
    {
      return false;
    }
    e.execute();
    return true;
  }

  void reset()
  {
    m_pos = 0;
    m_events = decltype(m_events){};
  }

private:
  size_t m_pos;
  std::vector< EventType > m_events;
};

class scene_event
{
public:
  template< class Callable >
  explicit scene_event(Callable f) : m_completed(false), m_body(std::move(f))
  {}

  void execute_idempotent()
  {
    if (completed())
    {
      return;
    }
    execute();
  }

  void execute() { m_body(); }

  bool completed() const noexcept { return m_completed; }

private:
  bool m_completed;
  std::function< void() > m_body;
};

#include "raylib-cpp.hpp"

float cubeSize = 2.0f;
float sphereRadius = cubeSize / 2.0f;
raylib::Vector3 smallCylinderPos(3.0f, 2.6f, 0.0f);
float angle = 0.0f;

int
main()
{
  // Инициализация окна
  const int screenWidth = 1000;
  const int screenHeight = 600;
  raylib::Window window(screenWidth, screenHeight, "Modeling 1");

  // Настройка 3D-камеры
  raylib::Camera3D camera(
    raylib::Vector3(0.0f, 6.0f, 12.0f), // Позиция камеры
    raylib::Vector3(0.0f, 1.0f, 0.0f),  // Точка, на которую смотрит камера
    raylib::Vector3(0.0f, 1.0f, 0.0f),  // Направление "верх" для камеры
    45.0f,                              // Угол обзора (FOV)
    CAMERA_PERSPECTIVE                  // Тип проекции
  );

  window.SetTargetFPS(60);

  // camera.Update(CAMERA_ORBITAL);
  while (!window.ShouldClose())
  {
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
    raylib::Vector3 cubePos(-4.0f, 1.0f, 0.0f);

    // 1. Рисуем каркасный куб
    cubePos.DrawCubeWires(cubeSize, cubeSize, cubeSize, BLUE);

    // 2. Изначально вписанная сфера имела бы радиус = cubeSize / 2 = 1.0f
    // Масштабируем её с коэффициентом 2.5 -> новый радиус = 1.0f * 2.5f = 2.5f
    cubePos.DrawSphereWires(sphereRadius, 16, 16, RED);

    // =================================================================
    // ЗАДАНИЕ 3 и 4: Большой и малый цилиндры
    // Сместим эту группу вправо (X = 3.0f)
    // =================================================================

    // Параметры большого цилиндра
    raylib::Vector3 largeCylinderPos(
      3.0f, 1.0f, 0.0f); // Центр по Y = 1.0f (при высоте 2.0f он стоит на полу)
    float largeRadius = 1.5f;
    float largeHeight = 2.0f;

    // Параметры малого цилиндра
    float smallRadius = 0.6f;
    float smallHeight = 1.2f;
    // Начальная позиция малого цилиндра (прямо поверх большого):
    // Y = Y_большого + (Высота_большого / 2) + (Высота_малого / 2) = 1.0 + 1.0
    // + 0.6 = 2.6f

    // 4. Модификации цилиндров:
    // А) Сдвигаем малый цилиндр по оси X (например, на +1.5 единицы вправо
    // относительно общей позиции)
    smallCylinderPos.x += 1.5f;

    rlPushMatrix();
    rlTranslatef(largeCylinderPos.x, largeCylinderPos.y, largeCylinderPos.z);

    // Наклоняем цилиндр по оси X, чтобы он лёг набок,
    // тогда поворот вокруг Y (или Z) станет визуально заметен!
    rlRotatef(angle, 1.0f, 0.0f, 0.0f);
    // rlRotatef(45.0f, 0.0f, 1.0f, 0.0f);

    DrawCylinderWires(
      ::Vector3{ 0, 0, 0 }, largeRadius, largeRadius, largeHeight, 12, LIME);
    rlPopMatrix();
    // Рисуем сдвинутый малый цилиндр
    DrawCylinderWires(
      smallCylinderPos, smallRadius, smallRadius, smallHeight, 12, ORANGE);

    camera.EndMode();

    // // Вывод подсказок на экран
    // DrawText("Управление: Зажмите ЛКМ и двигайте мышь для вращения камеры",
    //          10,
    //          10,
    //          20,
    //          DARKGRAY);
    // DrawText("Слева: Куб (синий) и масштабированная сфера (красная, k=2.5)",
    //          10,
    //          40,
    //          18,
    //          BLUE);
    // DrawText("Справа: Большой цилиндр (зеленый, повернут на 45 град) и малый
    // "
    //          "(оранжевый, сдвинут по X)",
    //          10,
    //          65,
    //          18,
    //          DARKGREEN);

    window.EndDrawing();
  }

  return 0;
}

// int
// main(int argc, char** argv)
// {
// }
