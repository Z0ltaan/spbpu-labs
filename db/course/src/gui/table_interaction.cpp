#include "gui/table_interaction.hpp"
#include <cstdarg>
#include <imgui.h>
#include "table_operations.hpp"

void
course::insert_popup(pqxx::connection& c,
                     const std::string& table,
                     const std::vector< std::string >& labels)
{
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

  if (ImGui::BeginPopupModal("Insert", NULL, ImGuiWindowFlags_AlwaysAutoResize))
  {
    // static std::string columns(100, '\n');
    static char columns[100];
    ImGui::Text("%s", labels[0].c_str());
    ImGui::InputText("columns", columns, IM_ARRAYSIZE(columns));
    static char values[100];
    ImGui::Text("%s", labels[1].c_str());
    ImGui::InputText("values", values, IM_ARRAYSIZE(values));

    if (ImGui::Button("Insert", ImVec2(120, 0)))
    {
      insert_into(c, table, columns, values);
      ImGui::CloseCurrentPopup();
    }
    ImGui::SetItemDefaultFocus();
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(120, 0)))
    {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}

void
course::delete_popup(pqxx::connection& c,
                     const std::string& table,
                     const std::vector< std::string >& labels)
{
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

  if (ImGui::BeginPopupModal("Delete", NULL, ImGuiWindowFlags_AlwaysAutoResize))
  {
    static char where_cond[100];
    ImGui::Text("%s", labels[0].c_str());
    ImGui::InputText("where", where_cond, IM_ARRAYSIZE(where_cond));

    if (ImGui::Button("Delete", ImVec2(120, 0)))
    {
      delete_from(c, table, where_cond);
      ImGui::CloseCurrentPopup();
    }
    ImGui::SetItemDefaultFocus();
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(120, 0)))
    {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}
void
course::update_popup(pqxx::connection& c,
                     const std::string& table,
                     const std::vector< std::string >& labels)
{
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

  if (ImGui::BeginPopupModal("Update", NULL, ImGuiWindowFlags_AlwaysAutoResize))
  {
    static char column[100];
    ImGui::Text("%s", labels[0].c_str());
    ImGui::InputText("column", column, IM_ARRAYSIZE(column));
    static char new_value[100];
    ImGui::Text("%s", labels[1].c_str());
    ImGui::InputText("new_value", new_value, IM_ARRAYSIZE(new_value));
    static char where_cond[100];
    ImGui::Text("%s", labels[2].c_str());
    ImGui::InputText("where", where_cond, IM_ARRAYSIZE(where_cond));

    if (ImGui::Button("Update", ImVec2(120, 0)))
    {
      update_table(c, table, column, new_value, where_cond);
      ImGui::CloseCurrentPopup();
    }
    ImGui::SetItemDefaultFocus();
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(120, 0)))
    {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}
