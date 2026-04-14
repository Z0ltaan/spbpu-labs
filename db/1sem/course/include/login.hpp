#ifndef LOGIN_HPP
#define LOGIN_HPP

#include "gui/call_login_window.hpp"
#include "user_token.hpp"
namespace course
{
  UserToken login(pqxx::connection& e,
                  const std::string& login,
                  const std::string& password);
}

#endif
