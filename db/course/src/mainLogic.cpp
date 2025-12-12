#include "mainLogic.hpp"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <pqxx/pqxx>

#include <stdexcept>
#include <string>
#include <vector>
#include "assign_work.hpp"
#include "gui/clickable_table.hpp"
#include "gui/table_interaction.hpp"
#include "read_config.hpp"
#include "table_operations.hpp"
#include "utils/query.hpp"

#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

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
  try
  {
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
      "masters", "services", "cars", "works"
    };

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

      // Create new window
      ImGui::Begin("MainWindow", open_indicator, main_window_flags);

      // ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
      //   if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
      //   {
      //     if (ImGui::BeginTabItem("Avocado"))
      //     {
      //       ImGui::Text("This is the Avocado tab!\nblah blah blah blah
      //       blah"); ImGui::EndTabItem();
      //     }
      //     if (ImGui::BeginTabItem("Broccoli"))
      //     {
      //       ImGui::Text("This is the Broccoli tab!\nblah blah blah blah
      //       blah"); ImGui::EndTabItem();
      //     }
      //     if (ImGui::BeginTabItem("Cucumber"))
      //     {
      //       ImGui::Text("This is the Cucumber tab!\nblah blah blah blah
      //       blah"); ImGui::EndTabItem();
      //     }
      //     ImGui::EndTabBar();
      //   }

      for (const auto& table_name: table_names)
      {
        if (ImGui::TreeNode(table_name.c_str()))
        {
          course::place_clickable_table(connection, "public", table_name);
          if (ImGui::Button("Insert"))
          {
            ImGui::OpenPopup("Insert");
          }
          insert_popup(
            connection, table_name, { "(column,..)", "('str', int,..)" });
          if (ImGui::Button("Update"))
          {
            ImGui::OpenPopup("Update");
          }
          update_popup(connection, table_name, { "column", "value", "where" });

          if (ImGui::Button("Delete"))
          {
            ImGui::OpenPopup("Delete");
          }
          delete_popup(connection, table_name, { "where" });
          ImGui::TreePop();
        }
      }

      ImGui::End();

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

    // pqxx::result res = query(connection, "call get_scores_table($1)", {2});
    // auto arr_entry = res.front().front().as_sql_array<std::string>();

    // pqxx::result res = query(connection, "call sum_up_costs($1)",
    // {"10months"}); auto arr_entry = res.front().front().as<int>();
    // std::cout << arr_entry << '\n';
    // delete_from(connection, "masters", "name='van'");
    // insert_into(connection, "masters", "(name)", "('van')");
    // query_res_t res =
    //   query(connection, "SELECT * from masters where name = 'van'", {});
    //
    // assign_work(connection, "11", "1", "1", "2020-04-12");
    //
    update_table(connection, "masters", "name", "'vanya'", "id=27");
    // for (const auto& row: res)
    // {
    //   auto [id, name] = row.as< int, std::string >();
    //   std::cout << id << ' ' << name << '\n';
    // }
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
