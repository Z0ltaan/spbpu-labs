#include "utils/query.hpp"

pqxx::result course::query(pqxx::connection &c, const std::string &query,
                           const pqxx::params &p) {
  pqxx::work transaction(c);
  pqxx::result r = transaction.exec(query, p);
  transaction.commit();
  return r;
}
