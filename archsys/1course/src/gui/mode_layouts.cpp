#include "gui/mode_layouts.hpp"
#include <imgui.h>
#include "configuration.hpp"
#include "disciplines.hpp"
#include "producer.hpp"

void course::call_interactive_mode_layout(const course::ProgramState &state)
{

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
      ImGui::Text("%f", current.prevProductionTime());
      // ImGui::Text("%f", current.nextProductionTime());
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
      // ImGui::Text("%f", current.nextFinishTime());
      ImGui::Text("%f", current.currentRequest().time());
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
}

void course::call_automatic_mode_layout(const course::ProgramState &state, const ProgramConfiguration &config)
{
  if (ImGui::BeginTable("graphs", 8, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Resizable))
  {
    ImGui::TableSetupColumn("Producer count");
    ImGui::TableSetupColumn("Device count");
    ImGui::TableSetupColumn("Production intensity");
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
    ImGui::Text("%f", (config.bounds.max + config.bounds.min) / 2.0);
    ImGui::TableNextColumn();
    ImGui::Text("%f", config.lambda);
    ImGui::TableNextColumn();
    ImGui::Text("%ld", state.buffer.size());
    ImGui::TableNextColumn();
    ImGui::Text("%f", double(state.canceled_requests) / double(state.produced_requests));
    ImGui::TableNextColumn();
    ImGui::Text("%f", double(state.accumulatedRequestTime) / double(state.processed_requests));
    ImGui::TableNextColumn();
    ImGui::Text("%f", double(state.produced_requests) / double(state.processed_requests));
    ImGui::EndTable();
  }
}
