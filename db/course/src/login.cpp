#include "login.hpp"
#include <map>
#include "user_token.hpp"
#include "utils/query.hpp"

extern bool show_error_window;
extern std::string last_error_message;

course::UserToken
course::login(pqxx::connection& e,
              const std::string& login,
              const std::string& password)
{
  std::string res =
    query(e,
          "SELECT role FROM users WHERE login = hash_string($1) AND "
          "password = hash_string($2)",
          { login, password })
      .front()
      .front()
      .as< std::string >();
  std::map< std::string, UserToken::Role > roles{
    { "admin", UserToken::ADMIN }, { "regular", UserToken::REGULAR }
  };
  UserToken tok = roles.at(res);

  return tok;
}
