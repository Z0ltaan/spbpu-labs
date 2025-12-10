#include "gui/clickable_table.hpp"
#include <algorithm>
#include <imgui.h>
#include <iterator>
#include <vector>
#include "utils/query.hpp"

void
course::place_clickable_table(pqxx::connection& c,
                              const std::string& schema,
                              const std::string& table_name)
{

  using column_names_t = std::vector< std::string >;
  column_names_t column_names;
  {
    auto columns =
      query(c,
            "SELECT column_name FROM information_schema.columns WHERE "
            "table_schema = $1 AND table_name = $2",
            { schema, table_name });
    std::transform(columns.begin(),
                   columns.end(),
                   std::back_insert_iterator(column_names),
                   [&](const pqxx::row& name)
                   { return std::get< 0 >(name.as< std::string >()); });
  }

  if (ImGui::BeginTable(table_name.c_str(), column_names.size()))
  {
    for (const auto& name: column_names)
    {
      ImGui::TableSetupColumn(name.c_str());
    }
    ImGui::TableHeadersRow();

    auto table_rows = query(c, "SELECT * FROM $1", { table_name });
    for (const auto& row: table_rows)
    {
      ImGui::TableNextRow();
      for (const auto& name: column_names)
      {
        ImGui::TableNextColumn();
        ImGui::Text("%s", row[name].c_str());
      }
    }
    ImGui::EndTable();
  }
}
