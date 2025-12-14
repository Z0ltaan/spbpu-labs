#include "get_column_values.hpp"
#include <imgui.h>
#include <string>
#include "utils/query.hpp"

int
course::place_column_values_list(pqxx::connection& c,
                                 const std::string& table,
                                 const std::string& column,
                                 int& list_pos,
                                 int& new_id)
{
  auto res = course::query(c, "select id, " + column + " from " + table, {});

  if (ImGui::Button(table.c_str()))
    ImGui::OpenPopup((table + "select_popup").c_str());
  ImGui::SameLine();
  ImGui::TextUnformatted(list_pos == -1
                           ? "<None>"
                           : res[list_pos].front().as< std::string >().c_str());
  if (ImGui::BeginPopup((table + "select_popup").c_str()))
  {
    ImGui::SeparatorText((table + ' ' + column).c_str());
    for (int i = 0; i < res.size(); i++)
    {
      auto [id, value] = res[i].as< int, std::string >();
      if (ImGui::Selectable((value + "##" + std::to_string(id)).c_str()))
      {
        list_pos = i;
        new_id = id;
      }
    }

    ImGui::EndPopup();
  }
  return new_id;
}
