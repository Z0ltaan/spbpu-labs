#ifndef USER_TOKEN_HPP
#define USER_TOKEN_HPP

namespace course
{
  struct UserToken
  {
    enum Role
    {
      REGULAR,
      ADMIN,
      NONE
    } role;

    UserToken() : role(Role::NONE) {}
    UserToken(UserToken::Role rhs) : role(rhs) {}
    UserToken& operator=(const UserToken::Role& rhs)
    {
      this->role = rhs;
      return *this;
    }
    bool is_admin() { return role == ADMIN; }
  };
}

#endif
