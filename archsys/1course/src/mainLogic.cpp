#include "mainLogic.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <exception>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <iterator>
#include <ostream>
#include <stdio.h>
#include <vector>
#include "buffer.hpp"
#include "device.hpp"
#include "disciplines.hpp"
#include "priority.hpp"
#include "producer.hpp"
#include "request.hpp"
#include "utils/idSetter.hpp"

#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

void circle_increment(size_t &value, size_t lower_bound, size_t upper_bound)
{
  if (value >= upper_bound)
  {
    value = lower_bound;
  }
  else
  {
    value++;
  }
}

void circle_decrement(size_t &value, size_t lower_bound, size_t upper_bound)
{
  if (value <= lower_bound)
  {
    value = upper_bound;
  }
  else
  {
    value--;
  }
}

namespace course
{
  struct ProgramState
  {
    std::vector< Producer > producers;
    std::vector< Device > devices;
    Buffer buffer;
    requestid_t processed_requests = 0;
    requestid_t produced_requests = 0;
    requestid_t canceled_requests = 0;
    double accumulatedRequestTime = 0.0;
    double currentTime = 0.0;
  };

  struct ProgramConfiguration
  {
    deviceid_t producerCount;
    deviceid_t deviceCount;
    size_t bufferSize;
    double simulationEnd;
  } config{11, 10, 5, 100.0};

  std::ostream &printState(std::ostream &out, const ProgramState &state);
  ProgramState predictNextProgramState(const ProgramState &state);
} // namespace course

static void glfw_error_callback(int error, const char *description)
{
  fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void call_automatic_mode_layout(const course::ProgramState &state);
void call_interactive_mode_layout(course::ProgramState &state);

// NOTE: ONLY FOR NOW interactive mode option
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

namespace course
{
  struct Event
  {
    double timestamp;
    size_t position;

    enum EventType
    {
      PRODUCTION,
      COMPLETION,
    } type;
  };

  ProgramState predictNextProgramState(const ProgramState &state)
  {
    auto newState{state};

    // Determine closest special event
    Event closestEvent = {state.producers[0].time(), 0, Event::PRODUCTION};
    for (size_t i = 1; i < state.producers.size(); ++i)
    {
      const Producer &current = state.producers[i];
      if (current.time() < closestEvent.timestamp)
      {
        closestEvent = {current.time(), i, Event::PRODUCTION};
      }
    }

    for (size_t i = 0; i < state.devices.size(); ++i)
    {
      const Device &current = state.devices[i];
      if (!current.empty() && current.time() < closestEvent.timestamp)
      {
        closestEvent = {current.time(), i, Event::COMPLETION};
      }
    }
    newState.currentTime = closestEvent.timestamp;

    if (closestEvent.type == Event::PRODUCTION)
    {
      auto &neededProducer = newState.producers[closestEvent.position];
      SimpleRequest req = neededProducer.produceRequest();
      newState.produced_requests += 1;

      try
      {
        if (state.buffer.empty())
        {
          try
          {
            auto deviceIndex = course::chooseDeviceById(newState.devices);
            Device &chosenDevice = newState.devices[deviceIndex];
            chosenDevice.setRequest(req, req.time());
          }
          catch (const std::exception &e)
          {
            std::cerr << e.what() << '\n';
            newState.buffer.tryAddRequest(req, chooseFirstEmptyBufferSpot);
          }
        }
        else
        {
          newState.buffer.tryAddRequest(req, chooseFirstEmptyBufferSpot);
        }
      }
      catch (const std::exception &e)
      {
        std::cerr << e.what() << '\n';
        newState.canceled_requests += 1;
      }
    }
    else if (closestEvent.type == Event::COMPLETION)
    {
      Device &chosenDevice = newState.devices[closestEvent.position];
      newState.accumulatedRequestTime += (chosenDevice.time() - chosenDevice.currentRequest().time());
      chosenDevice.finishProcessing();
      newState.processed_requests += 1;

      // NOTE: setRequest here works fine only if special events are prod, fin and simfin
      try
      {
        SimpleRequest req;
        newState.buffer.tryGetRequest(req, chooseRequestFromBufferByProducerNumber);
        chosenDevice.setRequest(req);
      }
      catch (const std::exception &e)
      {
        std::cerr << e.what() << '\n';
      }
    }
    return newState;
  }

  std::ostream &printState(std::ostream &out, const ProgramState &state)
  {
    // construct strings for output
    std::string producers("P: ");
    for (const Producer &p: state.producers)
    {
      try
      {
        producers += " " + std::to_string(p.requestCount());
      }
      catch (const std::bad_alloc &e)
      {
        std::cerr << e.what() << '\n';
      }
    }
    producers += '\n';

    std::string devices("D: ");
    for (const Device &d: state.devices)
    {
      try
      {
        devices += " " + std::to_string(d.processedRequests());
      }
      catch (const std::bad_alloc &e)
      {
        std::cerr << e.what() << '\n';
      }
    }
    devices += '\n';

    // output results
    out << producers << devices << "processed: " << state.processed_requests << '\n';
    out << "produced: " << state.produced_requests << '\n' << "canceled: " << state.canceled_requests << '\n';
    out << "current time: " << state.currentTime << '\n';
    out << "accumulated time: " << state.accumulatedRequestTime << '\n';
    return out;
  }
} // namespace course

void call_interactive_mode_layout(course::ProgramState &state)
{
  ImGui::Begin("Interactive mode"); // Create a window called "Hello,
                                    // world!" and append into it.

  static ImGuiTableFlags flags =
      ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Resizable;
  ImGui::Text("Producers info"); // Display some text (you can
  if (ImGui::BeginTable("producer", 4, flags))
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
  if (ImGui::Button("Previous state"))
  {
    circle_decrement(programFlowPos, 0, programFlow.size() - 1);
    state = programFlow[programFlowPos];
  }
  ImGui::SameLine();
  ImGui::Text(" %ld / %ld ", programFlowPos + 1, programFlow.size());
  ImGui::SameLine();
  if (ImGui::Button("Next state"))
  {
    circle_increment(programFlowPos, 0, programFlow.size() - 1);
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
