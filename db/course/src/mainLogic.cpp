#include "mainLogic.hpp"
#include <exception>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <pqxx/pqxx>

#include <stdexcept>
#include <string>
#include <vector>
#include "gui/call_error_window.hpp"
#include "gui/call_login_window.hpp"
#include "gui/clickable_table.hpp"
#include "gui/statistics_tab.hpp"
#include "gui/table_interaction.hpp"
#include "gui/work_assignment_tab.hpp"
#include "read_config.hpp"
#include "user_token.hpp"

#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

bool show_error_window = false;
std::string last_error_message;

static void
glfw_error_callback(int error, const char* description)
{
  fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int
course::mainLogic(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "Usage: <program name> <config path>\n";
    return -1;
  }

  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
    return 1;

  const char* glsl_version = "#version 330";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  float main_scale =
    ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()) * 1.2;

  using query_res_t = pqxx::result;
  using row_t = pqxx::row;

  GLFWwindow* window = nullptr;
  // NOTE: create connection to database
  std::string connection_options;
  read_config(connection_options, argv[1]);
  pqxx::connection connection(connection_options);
  std::cout << "Connected to " << connection.dbname() << '\n';

  window = glfwCreateWindow((int) (1280 * main_scale),
                            (int) (800 * main_scale),
                            "Car center helper",
                            nullptr,
                            nullptr);
  if (!window)
    throw std::runtime_error("Could not create a window");

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void) io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup scaling
  ImGuiStyle& style = ImGui::GetStyle();
  style.ScaleAllSizes(main_scale);
  style.FontScaleDpi = main_scale;

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  std::vector< std::string > table_names{
    "masters", "services", "cars", "works", "users"
  };

  try
  {

    while (!glfwWindowShouldClose(window))
    {
      glfwPollEvents();
      if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
      {
        ImGui_ImplGlfw_Sleep(10);
        continue;
      }

      // Start the Dear ImGui frame
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      ImGuiWindowFlags main_window_flags =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
      // ImGuiWindowFlags_NoBackground;
      bool* open_indicator = nullptr;

      ImGui::SetNextWindowSize(io.DisplaySize);
      ImGui::SetNextWindowPos(ImVec2{ 0, 0 });

      static bool logged_in = false;
      static UserToken current_user;
      if (logged_in)
      {
        // Create new window
        ImGui::Begin("MainWindow", open_indicator, main_window_flags);

        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
        if (ImGui::BeginTabBar("worktabs", tab_bar_flags))
        {
          if (current_user.is_admin() && ImGui::BeginTabItem("Tables"))
          {
            for (const auto& table_name: table_names)
            {
              if (ImGui::TreeNode(table_name.c_str()))
              {
                course::place_clickable_table(connection, "public", table_name);
                if (ImGui::Button("Insert"))
                {
                  ImGui::OpenPopup("Insert");
                }
                ImGui::SameLine();
                if (ImGui::Button("Update"))
                {
                  ImGui::OpenPopup("Update");
                }
                ImGui::SameLine();
                if (ImGui::Button("Delete"))
                {
                  ImGui::OpenPopup("Delete");
                }
                insert_popup(
                  connection, table_name, { "(column,..)", "('str', int,..)" });
                update_popup(
                  connection, table_name, { "column", "value", "where" });
                delete_popup(connection, table_name, { "where" });
                ImGui::TreePop();
              }
            }
            ImGui::EndTabItem();
          }
          place_work_assignment_tab(connection,
                                    { { "masters", "name" },
                                      { "services", "name" },
                                      { "cars", "num" } });
          place_scores_table_tab(connection);
          place_costs_tab(connection);
          ImGui::EndTabBar();
        }
        ImGui::End();
      }
      else
      {
        call_login_window(connection, logged_in, current_user);
      }

      if (show_error_window)
        call_error_window(&show_error_window, last_error_message);

      // Rendering
      ImGui::Render();
      int display_w, display_h;
      glfwGetFramebufferSize(window, &display_w, &display_h);
      glViewport(0, 0, display_w, display_h);
      glClearColor(clear_color.x * clear_color.w,
                   clear_color.y * clear_color.w,
                   clear_color.z * clear_color.w,
                   clear_color.w);
      glClear(GL_COLOR_BUFFER_BIT);
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

      glfwSwapBuffers(window);
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
