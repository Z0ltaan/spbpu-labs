#include <algorithm>
#include <cstddef>
#include <imgui.h>
#include <vector>
#include "get_column_values.hpp"
#include "gui/statistics_tab.hpp"
#include "utils/query.hpp"

void
course::place_scores_table_tab(pqxx::connection& c)
{
  if (ImGui::BeginTabItem("Scores table"))
  {
    // bool empty ;
    static int month;
    static int year;
    ImGui::InputInt("Input month", &month);
    ImGui::InputInt("Input year", &year);

    static std::vector< std::string > data;
    if (ImGui::Button("Request"))
    {
      pqxx::array< std::string, 1 > res =
        query(c, "call get_scores_table($1, $2)", { month, year })
          .front()
          .front()
          .as_sql_array< std::string >();
      data.resize(res.size());
      std::copy(res.cbegin(), res.cend(), data.begin());
    }

    if (ImGui::BeginTable("Scores", 2))
    {
      // set column names of the table
      ImGui::TableSetupColumn("Place");
      ImGui::TableSetupColumn("Master info (master id, name, car count)");
      ImGui::TableHeadersRow();

      for (size_t i = 0; i < data.size(); ++i)
      {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("%ld", i + 1);
        ImGui::TableNextColumn();
        ImGui::Text("%s", data[i].c_str());
      }
      ImGui::EndTable();
    }
    ImGui::EndTabItem();
  }
}

void
course::place_costs_tab(pqxx::connection& c)
{
  if (ImGui::BeginTabItem("Costs"))
  {
    static pqxx::result res;

    auto arr_entry = !res.size() ? 0 : res.front().front().as< int >();
    static char interval[20] = { '0' };
    ImGui::InputText(
      "Interval back from current year", interval, IM_ARRAYSIZE(interval));

    if (ImGui::Button("Check"))
    {
      if (interval[0] == '\0')
      {
        interval[1] = interval[0];
        interval[0] = '0';
      }
      res = query(c, "call sum_up_costs($1)", { interval });
    }

    ImGui::Text("Cost sum: ");
    ImGui::SameLine();
    ImGui::Text("%d", arr_entry);
    ImGui::EndTabItem();
  }
}
