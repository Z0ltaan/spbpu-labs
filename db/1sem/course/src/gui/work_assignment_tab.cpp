#include "gui/work_assignment_tab.hpp"
#include <exception>
#include <imgui.h>
#include <string>
#include <utility>
#include <vector>
#include "assign_work.hpp"
#include "get_column_values.hpp"

extern bool show_error_window;
extern std::string last_error_message;

void
course::place_work_assignment_tab(
  pqxx::connection& c,
  const std::vector< std::pair< std::string, std::string > >&
    table_column_pairs)
{
  static std::vector< int > impl_specific(table_column_pairs.size(), -1);
  static std::vector< int > selected_pos(table_column_pairs.size(), -1);
  if (ImGui::BeginTabItem("Assign Work"))
  {
    static char date[15];
    ImGui::Text("Date");
    ImGui::InputText("year-month-day-opional_time", date, IM_ARRAYSIZE(date));
    for (int i = 0; i < table_column_pairs.size(); ++i)
    {
      place_column_values_list(c,
                               table_column_pairs[i].first,
                               table_column_pairs[i].second,
                               impl_specific[i],
                               selected_pos[i]);
    }

    if (ImGui::Button("Assign"))
    {
      try
      {
        assign_work(c,
                    std::to_string(selected_pos[0]),
                    std::to_string(selected_pos[1]),
                    std::to_string(selected_pos[2]),
                    date[0] == '\0' ? "0" : date);
      }
      catch (const std::exception& e)
      {
        last_error_message = e.what();
        show_error_window = true;
      }
    }
    ImGui::EndTabItem();
  }
}
