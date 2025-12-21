#include "main_logic.hpp"
#include <array>
#include <buddy/bdd.h>
#include <cmath>
#include <fstream>

constexpr int N = 9; // число объектов;
constexpr int M = 4; // число свойств

constexpr int n1 = 7;
constexpr int n2 = 4;
constexpr int n3 = 2;
constexpr int n4 = 6;

const int LOG_N = std::ceil(std::log2(N));
const int N_VAR = N * M * LOG_N; // число булевых переменных

void fun(char *varset, int size);
std::ofstream out;

int var_index(int i, int k, int t) { return (i * M * LOG_N) + (k * LOG_N) + t; }

bdd bdd_eq(const bdd &a, const bdd &b) { return (a & b) | (!a & !b); }

int course::main_logic(int argc, char **argv)
{
  std::array< std::array< std::array< bdd, N >, N >, M > properties;
  std::cout << LOG_N << ' ' << N_VAR << '\n';

  // инициализация
  bdd_init(10000000, 100000);
  bdd_setvarnum(N_VAR);

  // ->–- вводим функцию p(k, i, j) следующим образом ( кодируем p[k][i][j] ):
  std::cout << "naming not done\n";

  for (int i = 0; i < N; ++i)
  {
    for (int j = 0; j < N; ++j)
    {
      for (int k = 0; k < M; ++k)
      {
        bdd &tmp = properties[k][i][j];
        tmp = bddtrue;
        for (int t = 0; t < LOG_N; ++t)
        {
          auto idx = var_index(i, k, t);
          tmp &= ((j >> t) & 1) ? bdd_ithvar(idx) : bdd_nithvar(idx);
        }
      }
    }
  }

  std::cout << "naming done\n";
  bdd task = bddtrue;

  // NOTE: 1st level constraints
  int cond = 0;
  for (int k = 0; k < M && cond < n1; ++k)
  {
    for (int i = 0; i < N && cond < n1; ++i)
    {
      task &= properties[k][i][0];
      cond++;
      std::cout << cond << '\n';
    }
  }

  cond = 0;
  for (int k = 1; k < M && cond < n2; ++k)
    for (int i = 0; i < N - 1 && cond < n2; ++i)
    {
      auto p1 = properties[0][i][0];
      auto p2 = properties[1][i][0];
      task &= bdd_eq(p1, p2);
      cond++;
    }

  cond = 0;
  // for (int i = 1; i < N - 2 && cond < n3; ++i)
  // {
  //   auto p1 = properties[0][i - 1][0];
  //   auto p2 = properties[1][i][1];
  //   task &= bdd_eq(p1, p2);
  //   cond++;
  // }
  // task &= !properties[0][0][0] & !properties[1][N - 1][1];

  for (int i = 0; i < N && i < n4; ++i)
  {
  }

  out.open("out.txt");
  double satcount = bdd_satcount(task);
  std::cout << satcount << '\n';
  out << satcount << " solutions:\n";
  // if (satcount)
  //   bdd_allsat(task, fun);
  bdd_done();
  return 0;
}

char var[N_VAR];
// TODO: change signature for it to accept ref to ostream and maybe get rid of global variable
void print()
{
  for (unsigned i = 0; i < N; i++)
  {
    out << i << ": ";
    for (unsigned j = 0; j < N; j++)
    {
      unsigned J = i * N * LOG_N + j * LOG_N;
      unsigned num = 0;
      for (unsigned k = 0; k < LOG_N; k++)
        num += (unsigned) (var[J + k] << k);
      out << num << ' ';
    }
    out << std::endl;
  }
  out << std::endl;
}

// TODO: Rewrite
void build(char *varset, unsigned n, unsigned I)
{
  if (I == n - 1)
  {
    if (varset[I] >= 0)
    {
      var[I] = varset[I];
      print();
      return;
    }
    var[I] = 0;
    print();
    var[I] = 1;
    print();
    return;
  }
  if (varset[I] >= 0)
  {
    var[I] = varset[I];
    build(varset, n, I + 1);
    return;
  }
  var[I] = 0;
  build(varset, n, I + 1);
  var[I] = 1;
  build(varset, n, I + 1);
}

void fun(char *varset, int size) { build(varset, size, 0); }
