#include "mainLogic.hpp"
#include <algorithm>
#include <cmath>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iterator>
#include <stdio.h>
#include <vector>
#include "buffer.hpp"
#include "configuration.hpp"
#include "device.hpp"
#include "producer.hpp"
#include "request.hpp"
#include "utils/circled_operations.hpp"
#include "utils/idSetter.hpp"

#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

static void glfw_error_callback(int error, const char *description)
{
  fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void call_automatic_mode_layout(const course::ProgramState &state);
void call_interactive_mode_layout(course::ProgramState &state);

// NOTE: ONLY FOR NOW interactive mode option
course::ProgramConfiguration config{11, 10, 5, 100.0};
bool interactive_mode = true;
size_t programFlowPos = 0;
std::vector< course::ProgramState > programFlow;

int course::mainLogic(int argc, char **argv)
{
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
    return 1;

  const char *glsl_version = "#version 330";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  // initial programState
  ProgramState current_state{std::vector< Producer >(), std::vector< Device >(), Buffer(config.bufferSize)};

  {
    UniformRandomGenerator prodgen(UniformRandomGenerator::Bounds{2.0, 5.0});
    std::generate_n(std::back_insert_iterator(current_state.producers), config.producerCount,
                    [&] { return Producer(prodgen); });

    ExponentialRandomGenerator devgen(7.0);
    std::generate_n(std::back_insert_iterator(current_state.devices), config.deviceCount,
                    [&] { return Device(devgen); });
  }

  std::for_each(current_state.producers.begin(), current_state.producers.end(), course::IdSetter< course::Producer >());
  std::for_each(current_state.devices.begin(), current_state.devices.end(), course::IdSetter< course::Device >());

  while (current_state.currentTime < config.simulationEnd)
  {
    programFlow.push_back(current_state);
    current_state = predictNextProgramState(current_state);
  }
  programFlow.push_back(current_state);
  current_state = programFlow.front();
  // here goes gui or some other output
  float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()); // Valid on GLFW 3.3+ only
  GLFWwindow *window =
      glfwCreateWindow((int) (1280 * main_scale), (int) (800 * main_scale), "Queueing system model", nullptr, nullptr);
  if (window == nullptr)
    return 1;
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void) io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsLight();

  // Setup scaling
  ImGuiStyle &style = ImGui::GetStyle();
  style.ScaleAllSizes(main_scale); // Bake a fixed style scale. (until we have a solution for
                                   // dynamic style scaling, changing this requires resetting
                                   // Style + calling this again)
  style.FontScaleDpi = main_scale; // Set initial font scale. (using io.ConfigDpiScaleFonts=true
                                   // makes this unnecessary. We leave both here for
                                   // documentation purpose)

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // Our state
  bool show_demo_window = true;
  bool show_another_window = false;
  bool show_main_window = true;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

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

    if (interactive_mode)
    {
      call_interactive_mode_layout(current_state);
    }
    else
    {
      call_automatic_mode_layout(current_state);
    }

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w,
                 clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}

void call_interactive_mode_layout(course::ProgramState &state)
{
  ImGui::Begin("Interactive mode");

  static ImGuiTableFlags flags =
      ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Resizable;
  ImGui::Text("Producers info"); // Display some text (you can
  if (ImGui::BeginTable("producer", 3, flags))
  {
    ImGui::TableSetupColumn("Name");
    ImGui::TableSetupColumn("Timestamp");
    ImGui::TableSetupColumn("Produced");
    ImGui::TableHeadersRow();
    for (int row = 0; row < state.producers.size(); row++)
    {
      const auto &current = state.producers[row];
      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      ImGui::Text("P%d", current.id());
      ImGui::TableNextColumn();
      ImGui::Text("%f", current.time());
      ImGui::TableNextColumn();
      ImGui::Text("%ld", current.requestCount());
    }
    ImGui::EndTable();
  }

  ImGui::Text("Devices info"); // Display some text (you can
  if (ImGui::BeginTable("devices", 4, flags))
  {
    ImGui::TableNextColumn();
    ImGui::TableHeader("Name");
    ImGui::TableNextColumn();
    ImGui::TableHeader("Timestamp");
    ImGui::TableNextColumn();
    ImGui::TableHeader("State");
    ImGui::TableNextColumn();
    ImGui::TableHeader("Processed");
    for (int row = 0; row < state.devices.size(); row++)
    {
      const auto &current = state.devices[row];
      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      ImGui::Text("D%d", current.id()); // name
      ImGui::TableNextColumn();
      ImGui::Text("%f", current.time());
      ImGui::TableNextColumn();
      ImGui::Text("%s", current.empty() ? "Free" : "Occupied");
      ImGui::TableNextColumn();
      ImGui::Text("%ld", current.processedRequests());
    }
    ImGui::EndTable();
  }
  ImGui::Text("Buffer"); // Display some text (you can
  if (ImGui::BeginTable("buffer", 3, flags))
  {
    ImGui::TableNextColumn();
    ImGui::TableHeader("Slot id");
    ImGui::TableNextColumn();
    ImGui::TableHeader("Timestamp");
    ImGui::TableNextColumn();
    ImGui::TableHeader("Producer id");
    const auto &buff = state.buffer;
    for (size_t row = 0; row < buff.size(); row++)
    {
      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      ImGui::Text("%ld", row);
      ImGui::TableNextColumn();
      ImGui::Text("%f", buff[row].time());
      ImGui::TableNextColumn();
      ImGui::Text("%d", buff[row].producerId());
    }
    ImGui::EndTable();
  }

  if (ImGui::Button("Previous state"))
  {
    course::circle_decrement(programFlowPos, 0, programFlow.size() - 1);
    state = programFlow[programFlowPos];
  }
  ImGui::SameLine();
  ImGui::Text(" %ld / %ld ", programFlowPos + 1, programFlow.size());
  ImGui::SameLine();
  if (ImGui::Button("Next state"))
  {
    course::circle_increment(programFlowPos, 0, programFlow.size() - 1);
    state = programFlow[programFlowPos];
  }
  if (ImGui::Button("Finish simulation"))
  {
    interactive_mode = false;
  }
  ImGui::End();
}

void call_automatic_mode_layout(const course::ProgramState &state)
{
  ImGui::Begin("Automatic mode");

  if (ImGui::BeginTable("graphs", 8, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Resizable))
  {
    ImGui::TableSetupColumn("Producer count");
    ImGui::TableSetupColumn("Device count");
    ImGui::TableSetupColumn("Prod bounds");
    ImGui::TableSetupColumn("Exp dist lamdba");
    ImGui::TableSetupColumn("Buffer size");
    ImGui::TableSetupColumn("P canc");
    ImGui::TableSetupColumn("T req");
    ImGui::TableSetupColumn("Workload");
    ImGui::TableHeadersRow();

    ImGui::TableNextColumn();
    ImGui::Text("%ld", state.producers.size());
    ImGui::TableNextColumn();
    ImGui::Text("%ld", state.devices.size());
    ImGui::TableNextColumn();
    ImGui::Text("{ %f , %f }", 2.0, 5.0);
    ImGui::TableNextColumn();
    ImGui::Text("%f", 7.0);
    ImGui::TableNextColumn();
    ImGui::Text("%ld", state.buffer.size());
    ImGui::TableNextColumn();
    ImGui::Text("%f", double(state.canceled_requests) / double(state.produced_requests));
    ImGui::TableNextColumn();
    ImGui::Text("%f", double(state.accumulatedRequestTime) / double(state.processed_requests));
    ImGui::TableNextColumn();
    ImGui::Text("%f", 3.5 / 7.0);
    ImGui::EndTable();
  }

  ImGui::End();
}
