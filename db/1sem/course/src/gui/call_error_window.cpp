#include "gui/call_error_window.hpp"
#include <imgui.h>

void
course::call_error_window(bool* open_indicator, const std::string& message)
{
  if (ImGui::Begin("ErrorWindow", open_indicator))
  {
    ImGui::Text("Error: %s", message.c_str());
    ImGui::End();
  }
}
