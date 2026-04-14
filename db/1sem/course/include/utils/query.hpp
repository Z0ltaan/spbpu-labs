#ifndef QUERY_HPP
#define QUERY_HPP

#include <pqxx/pqxx>

namespace course {
pqxx::result query(pqxx::connection &c, const std::string &query,
                   const pqxx::params &p);
} // namespace course

#endif
