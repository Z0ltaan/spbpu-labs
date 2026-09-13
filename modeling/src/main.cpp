#include <GL/glew.h>
// NOTE: i need glew.h at the top
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <fstream>
#include <functional>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <stb_image.h>
#include <stdexcept>
#include <string_view>
#include <utility>

namespace sandbox
{
  struct defaults
  {
    constexpr static inline size_t min_log_length = 1024;
    constexpr static inline int single_shader_source = 1;
    constexpr static inline GLuint empty_gl_object = 0;
    constexpr static inline std::nullptr_t no_length_array = nullptr;
  };
}
namespace sandbox::logging
{
  template< class char_t >
  void log(FILE* out, const char* format, const char_t* message)
  {
    std::fprintf(out, format, message);
  }

  void glfw_error_callback(int code, const char* description)
  {
    fprintf(
      stderr, "[GLFW ERROR]: Code = %uc\nDescription: %s", code, description);
  }
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

namespace sandbox::opengl
{
  class opengl_object
  {
  public:
    using id_type = GLuint;

    constexpr opengl_object() : id_(0) {}
    explicit opengl_object(id_type id) : opengl_object() { set_id(id); }

    auto id() const noexcept { return id_; }
    void set_id(id_type rhs) noexcept
    {
      sandbox::util::assert::throw_on_false(
        rhs != 0, "Not valid id for an opengl object");

      id_ = rhs;
    }

    bool valid_id() const noexcept { return id() != 0; }

    operator GLuint() const noexcept { return id(); }

  protected:
    ~opengl_object() = default;

  private:
    id_type id_;
  };

  class shader: public opengl_object
  {
  public:
    using shader_type_t = GLenum;

    constexpr shader() = default;
    explicit shader(opengl_object::id_type id) : opengl_object(id) {}

    shader(std::istream& in, shader_type_t shader_type, size_t shader_size) :
      shader(glCreateShader(shader_type))
    {
      read_shader_source(in, shader_size);
    }

    explicit shader(const std::filesystem::path& shader_file_path) :
      shader(glCreateShader(get_shader_type(shader_file_path.c_str())))
    {
      std::cout << id() << "\n";
      read_shader_source(shader_file_path);
    }

    void read_shader_source(std::istream& in, size_t shader_size)
    {
      std::string shader_body(shader_size + 1, '\0');

      in.read(shader_body.data(), shader_size);

      auto data = shader_body.data();

      glShaderSource(id(),
                     defaults::single_shader_source,
                     std::addressof(data),
                     defaults::no_length_array);
    }

    void read_shader_source(const std::filesystem::path& shader_file_path)
    {
      std::ifstream in(shader_file_path);
      if (!in.is_open())
      {
        std::string message = "Cant open shader file for read: ";
        message += shader_file_path;

        throw std::runtime_error(message);
      }

      auto shader_size = std::filesystem::file_size(shader_file_path);

      read_shader_source(in, shader_size);
    }

    void compile()
    {
      glCompileShader(id());

      GLint status = 0;
      glGetShaderiv(id(), GL_COMPILE_STATUS, &status);
      if (status != GL_TRUE)
      {
        static std::array< char, 1024 > info_log;
        glGetShaderInfoLog(id(), info_log.size(), nullptr, info_log.data());
        throw std::runtime_error(info_log.data());
      }
    }

  private:
    constexpr bool valid_shader_type(GLenum shader_type)
    {
      return shader_type == GL_INVALID_VALUE;
    }

    constexpr GLenum get_shader_type(std::string_view shader_file_path)
    {
      auto ext =
        shader_file_path.substr(shader_file_path.find_last_of('.') + 1);
      if (ext == "vert")
      {
        return GL_VERTEX_SHADER;
      }
      else if (ext == "frag")
      {
        return GL_FRAGMENT_SHADER;
      }
      else if (ext == "geom")
      {
        return GL_GEOMETRY_SHADER;
      }
      else if (ext == "comp")
      {
        return GL_COMPUTE_SHADER;
      }
      else if (ext == "tesc")
      {
        return GL_TESS_CONTROL_SHADER;
      }
      else if (ext == "tese")
      {
        return GL_TESS_EVALUATION_SHADER;
      }
      return GL_INVALID_VALUE;
    }
  };

  class shader_program: public opengl_object
  {
  public:
    using shaderid_t = opengl_object::id_type;

    constexpr shader_program() = default;

    explicit shader_program(opengl_object::id_type program) :
      opengl_object(program)
    {}

    template< class input_iterator >
    shader_program(input_iterator begin, input_iterator end) :
      shader_program(glCreateProgram())
    {
      attach_shader(begin, end);
    }

    shader_program(std::initializer_list< shader > shaders) :
      shader_program(shaders.begin(), shaders.end())
    {
      attach_shader(shaders.begin(), shaders.end());
    }

    void attach_shader(shaderid_t id) { glAttachShader(*this, id); }

    template< class input_iterator >
    void attach_shader(input_iterator begin, input_iterator end)
    {
      std::for_each(
        begin, end, [this](const shader& shader) { attach_shader(shader); });
    }

    void attach_shader(std::initializer_list< shader > shaders)
    {
      attach_shader(shaders.begin(), shaders.end());
    }

    void link()
    {
      glLinkProgram(id());
      GLint status = 0;
      glGetProgramiv(id(), GL_LINK_STATUS, &status);
      if (status != GL_TRUE)
      {
        static std::array< char, 1024 > info_log;
        glGetProgramInfoLog(id(), info_log.size(), nullptr, info_log.data());
        throw std::runtime_error(info_log.data());
      }
    }

    void use() const noexcept { glUseProgram(id()); }

    // void set_integer(int value) {}
    // void set_float(float value) {}
    // void set_(int value) {}

  private:
  };

  class buffer_object: public opengl_object
  {
  public:
    buffer_object() = default;

    explicit buffer_object(opengl_object::id_type id) : opengl_object(id) {}

    void bind(GLenum target) const noexcept { glBindBuffer(target, id()); }

    void unbind(GLenum target) const noexcept
    {
      glBindBuffer(target, defaults::empty_gl_object);
    }

    template< class container >
    void buffer_data(GLenum target,
                     const container& data,
                     GLenum usage) const noexcept
    {
      bind(target);
      glBufferData(target,
                   data.size() * sizeof(typename container::value_type),
                   data.data(),
                   usage);
    }

  private:
  };

  class vertex_array: public opengl_object
  {
  public:
    vertex_array() = default;
    vertex_array(opengl_object::id_type id) : opengl_object(id) {}

    void bind() const noexcept { glBindVertexArray(*this); }
    void unbind() const noexcept
    {
      glBindVertexArray(defaults::empty_gl_object);
    }

  private:
  };
}

// float deltaTime = 0.f;
// float lastFrame = 0.f;

std::vector< glm::vec3 >
generate_wire_cube()
{
  return { // Нижнее основание
           { -0.5f, -0.5f, -0.5f },
           { 0.5f, -0.5f, -0.5f },
           { 0.5f, -0.5f, -0.5f },
           { 0.5f, -0.5f, 0.5f },
           { 0.5f, -0.5f, 0.5f },
           { -0.5f, -0.5f, 0.5f },
           { -0.5f, -0.5f, 0.5f },
           { -0.5f, -0.5f, -0.5f },
           // Верхнее основание
           { -0.5f, 0.5f, -0.5f },
           { 0.5f, 0.5f, -0.5f },
           { 0.5f, 0.5f, -0.5f },
           { 0.5f, 0.5f, 0.5f },
           { 0.5f, 0.5f, 0.5f },
           { -0.5f, 0.5f, 0.5f },
           { -0.5f, 0.5f, 0.5f },
           { -0.5f, 0.5f, -0.5f },
           // Боковые стойки
           { -0.5f, -0.5f, -0.5f },
           { -0.5f, 0.5f, -0.5f },
           { 0.5f, -0.5f, -0.5f },
           { 0.5f, 0.5f, -0.5f },
           { 0.5f, -0.5f, 0.5f },
           { 0.5f, 0.5f, 0.5f },
           { -0.5f, -0.5f, 0.5f },
           { -0.5f, 0.5f, 0.5f }
  };
}

std::vector< glm::vec3 >
generate_wire_sphere(float radius = 0.5f, int rings = 20, int sectors = 20)
{
  std::vector< glm::vec3 > vertices;
  std::vector< glm::vec3 > temp_grid;

  // Генерируем точки сферы
  for (int r = 0; r <= rings; ++r)
  {
    float phi = glm::pi< float >() * (float) r / (float) rings; // от 0 до pi
    for (int s = 0; s <= sectors; ++s)
    {
      float theta =
        2.0f * glm::pi< float >() * (float) s / (float) sectors; // от 0 до 2pi
      float x = radius * std::sin(phi) * std::cos(theta);
      float y = radius * std::cos(phi);
      float z = radius * std::sin(phi) * std::sin(theta);
      temp_grid.push_back({ x, y, z });
    }
  }

  // Соединяем линиями для каркасной сетки
  for (int r = 0; r < rings; ++r)
  {
    for (int s = 0; s < sectors; ++s)
    {
      int p1 = r * (sectors + 1) + s;
      int p2 = p1 + 1;
      int p3 = (r + 1) * (sectors + 1) + s;

      // Параллели
      vertices.push_back(temp_grid[p1]);
      vertices.push_back(temp_grid[p2]);
      // Меридианы
      vertices.push_back(p1 < temp_grid.size() ? temp_grid[p1] : glm::vec3(0));
      vertices.push_back(p3 < temp_grid.size() ? temp_grid[p3] : glm::vec3(0));
    }
  }
  return vertices;
}

std::vector< glm::vec3 >
generate_wire_cylinder(float radius, float height, int segments = 24)
{
  std::vector< glm::vec3 > vertices;
  float half_h = height / 2.0f;

  for (int i = 0; i < segments; ++i)
  {
    float angle1 = 2.0f * glm::pi< float >() * (float) i / (float) segments;
    float angle2 =
      2.0f * glm::pi< float >() * (float) (i + 1) / (float) segments;

    glm::vec3 b1{ radius * std::cos(angle1),
                  -half_h,
                  radius * std::sin(angle1) };
    glm::vec3 b2{ radius * std::cos(angle2),
                  -half_h,
                  radius * std::sin(angle2) };
    glm::vec3 t1{ radius * std::cos(angle1),
                  half_h,
                  radius * std::sin(angle1) };
    glm::vec3 t2{ radius * std::cos(angle2),
                  half_h,
                  radius * std::sin(angle2) };

    // Нижнее кольцо
    vertices.push_back(b1);
    vertices.push_back(b2);
    // Верхнее кольцо
    vertices.push_back(t1);
    vertices.push_back(t2);
    // Вертикальные ребра стенки
    vertices.push_back(b1);
    vertices.push_back(t1);
  }
  return vertices;
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

void
setup_vao(const sandbox::opengl::vertex_array& vao,
          const sandbox::opengl::buffer_object& vbo,
          const std::vector< glm::vec3 >& data)
{
  vao.bind();
  vbo.buffer_data(GL_ARRAY_BUFFER, data, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
};

std::vector< glm::vec3 > cube_verts;
std::vector< glm::vec3 > sphere_verts;
std::vector< glm::vec3 > big_cyl_verts;
std::vector< glm::vec3 > small_cyl_verts;

void
process_input(GLFWwindow* window, event_line< scene_event >& lines)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }
  // Статическая переменная для отслеживания предыдущего состояния клавиши Enter

  static int last_enter_state = GLFW_RELEASE;
  int current_enter_state = glfwGetKey(window, GLFW_KEY_ENTER);

  // Если клавиша была нажата и сейчас отпущена (событие клика)
  if (current_enter_state == GLFW_RELEASE && last_enter_state == GLFW_PRESS)
  {
    // Переходим к следующему шагу, если он есть
    if (lines.next())
    {
      lines.execute_current();
    }
  }
  last_enter_state = current_enter_state;
}

int
main(int argc, char** argv)
{
  using namespace sandbox;

  try
  {
    glfwSetErrorCallback(sandbox::logging::glfw_error_callback);
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
    if (!glfwInit())
    {
      return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    sandbox::util::call_on_destruction destroy_glfw(glfwTerminate);
    GLFWwindow* window = glfwCreateWindow(640, 480, "Sandbox", NULL, NULL);
    if (!window)
    {
      return -1;
    };

    glfwMakeContextCurrent(window);
    {
      GLenum err = glewInit();
      if (GLEW_OK != err)
      {
        sandbox::logging::log(
          stderr, "Error initializing GLEW: %s\n", glewGetErrorString(err));
        return -1;
      }
    }

    glEnable(GL_DEPTH_TEST);
    // Переменные состояния нашей сцены
    struct SceneState
    {
      bool show_cube = false;
      bool show_sphere = false;
      float sphere_scale = 1.0f;

      bool show_big_cylinder = false;
      bool show_small_cylinder = false;
      float big_cylinder_rotation_y = 0.0f;
      float small_cylinder_shift_x = 0.0f;
    } state;

    event_line< scene_event > lines;
    lines.add_event(
      scene_event([]() { std::cout << "Step 0. Empty scene\n"; }));

    lines.add_event(scene_event(
      [&state]()
      {
        std::cout << "Step 1: Cube and sphere.\n";
        state.show_cube = true;
        state.show_sphere = true;
        state.sphere_scale = 1.0f;
      }));

    lines.add_event(scene_event(
      [&state]()
      {
        std::cout << "Step 2: Sphere is scaled up 2.5 times.\n";
        state.sphere_scale = 2.5f;
      }));

    lines.add_event(scene_event(
      [&state]()
      {
        std::cout << "Step 3: Cylinders.\n";
        state.show_big_cylinder = true;
        state.show_small_cylinder = true;
        state.small_cylinder_shift_x = 0.0f;
      }));

    lines.add_event(scene_event(
      [&state]()
      {
        std::cout << "Step 4: Small cyl moved along X axis, bigger one was "
                     "rotated 45 degrees\n";
        state.big_cylinder_rotation_y = 45.0f;
        state.small_cylinder_shift_x = 0.5f;
      }));

    // NOTE: start of rectangle
    sandbox::opengl::shader vertex_shader(
      "/home/georv/work/spbpu-labs/modeling/res/shaders/simple_pos_color.vert");
    sandbox::opengl::shader fragment_shader(
      "/home/georv/work/spbpu-labs/modeling/res/shaders/simple_color.frag");

    vertex_shader.compile();
    fragment_shader.compile();

    sandbox::opengl::shader_program shader_program(glCreateProgram());

    shader_program.attach_shader(vertex_shader);
    shader_program.attach_shader(fragment_shader);

    shader_program.link();

    cube_verts = generate_wire_cube();
    sphere_verts = generate_wire_sphere(0.5f, 20, 20);
    big_cyl_verts = generate_wire_cylinder(0.4f, 0.8f, 24);
    small_cyl_verts = generate_wire_cylinder(0.2f, 0.4f, 24);

    std::array< GLuint, 4 > vbo_handles;
    glGenBuffers(vbo_handles.size(), vbo_handles.data());

    std::array< GLuint, 4 > vao_handles;
    glGenVertexArrays(vao_handles.size(), vao_handles.data());

    sandbox::opengl::vertex_array cube_vao(vao_handles[0]),
      sphere_vao(vao_handles[1]), big_cyl_vao(vao_handles[2]),
      small_cyl_vao(vao_handles[3]);

    sandbox::opengl::buffer_object cube_vbo(vbo_handles[0]),
      sphere_vbo(vbo_handles[1]), big_cyl_vbo(vbo_handles[2]),
      small_cyl_vbo(vbo_handles[3]);

    setup_vao(cube_vao, cube_vbo, cube_verts);
    setup_vao(sphere_vao, sphere_vbo, sphere_verts);
    setup_vao(big_cyl_vao, big_cyl_vbo, big_cyl_verts);
    setup_vao(small_cyl_vao, small_cyl_vbo, small_cyl_verts);

    // Включаем каркасный режим отображения полигонов и линий
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.15f, 0.15f, 1.0f);

    lines.execute_current();

    // Настройка матриц камеры
    glm::vec3 cameraPos = glm::vec3(0.0f, 3.0f, 6.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
    glm::mat4 projection =
      glm::perspective(glm::radians(45.0f), 640.f / 480.f, 0.1f, 100.0f);

    unsigned int modelLoc = glGetUniformLocation(shader_program, "model");
    unsigned int viewLoc = glGetUniformLocation(shader_program, "view");
    unsigned int pLoc = glGetUniformLocation(shader_program, "projection");

    while (!glfwWindowShouldClose(window))
    {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      process_input(window, lines);

      shader_program.use();
      glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
      glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(projection));

      glm::mat4 group1_base =
        glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, 0.0f, 0.0f));

      if (state.show_cube)
      {
        glm::mat4 cube_model = group1_base;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(cube_model));
        cube_vao.bind();
        glDrawArrays(GL_LINES, 0, cube_verts.size());
      }

      if (state.show_sphere)
      {
        glm::mat4 sphere_model =
          glm::scale(group1_base, glm::vec3(state.sphere_scale));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(sphere_model));
        sphere_vao.bind();
        glDrawArrays(GL_LINES, 0, sphere_verts.size());
      }

      glm::mat4 group2_base =
        glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.0f, 0.0f));

      if (state.show_big_cylinder)
      {
        glm::mat4 big_cyl_model = group2_base;
        big_cyl_model = glm::rotate(big_cyl_model,
                                    glm::radians(state.big_cylinder_rotation_y),
                                    glm::vec3(1.0f, 0.0f, 0.0f));

        glUniformMatrix4fv(
          modelLoc, 1, GL_FALSE, glm::value_ptr(big_cyl_model));
        big_cyl_vao.bind();
        glDrawArrays(GL_LINES, 0, big_cyl_verts.size());
      }

      if (state.show_small_cylinder)
      {
        glm::mat4 small_cyl_model = group2_base;
        small_cyl_model = glm::translate(
          small_cyl_model, glm::vec3(state.small_cylinder_shift_x, 0.6f, 0.0f));

        glUniformMatrix4fv(
          modelLoc, 1, GL_FALSE, glm::value_ptr(small_cyl_model));
        small_cyl_vao.bind();
        glDrawArrays(GL_LINES, 0, small_cyl_verts.size());
      }

      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  }
  catch (const std::exception& e)
  {
    sandbox::logging::log(stderr, "[Caught]: %s\n", e.what());
  }
}
