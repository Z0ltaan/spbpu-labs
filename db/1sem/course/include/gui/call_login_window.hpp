#ifndef CALL_LOGIN_WINDOW_HPP
#define CALL_LOGIN_WINDOW_HPP

#include <pqxx/pqxx>
#include "user_token.hpp"

namespace course
{

  void call_login_window(pqxx::connection& e,
                         bool& open_indicator,
                         UserToken& token);
}

#endif
