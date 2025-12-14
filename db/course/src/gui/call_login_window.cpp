#include "gui/call_login_window.hpp"
#include <exception>
#include "imgui.h"
#include "login.hpp"

extern bool show_error_window;
extern std::string last_error_message;

void
course::call_login_window(pqxx::connection& c,
                          bool& open_indicator,
                          UserToken& token)
{
  if (ImGui::Begin("LoginWindow"))
  {
    static char login[65];
    static char password[65];
    ImGui::InputText("login", login, IM_ARRAYSIZE(login));
    ImGui::InputText("password",
                     password,
                     IM_ARRAYSIZE(password),
                     ImGuiInputTextFlags_Password);

    if (ImGui::Button("Login"))
    {
      try
      {
        token = course::login(c,
                              std::string(login, IM_ARRAYSIZE(login)),
                              std::string(password, IM_ARRAYSIZE(password)));
        open_indicator = true;
      }
      catch (const std::exception& e)
      {
        show_error_window = true;
        last_error_message = e.what();
      }
    }
    ImGui::End();
  }
}
