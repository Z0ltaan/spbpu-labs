#include "main_logic.hpp"
#include <array>
#include <buddy/bdd.h>
#include <fstream>

#define N_VAR 18 // число булевых переменных
#define N 3      // число объектов
#define M 3      // число свойств
#define LOG_N 2

void fun(char *varset, int size);
std::ofstream out;

int course::main_logic(int argc, char **argv)
{

  std::array< std::array< std::array< bdd, N >, N >, M > properties;
  // инициализация
  bdd_init(10000, 1000);
  bdd_setvarnum(N_VAR);

  // ->–- вводим функцию p(k, i, j) следующим образом ( кодируем p[k][i][j] ):
  unsigned I = 0;
  for (unsigned i = 0; i < N; i++)
  {
    for (unsigned j = 0; j < N; j++)
    {
      for (unsigned k = 0; k < M; k++)
      {
        auto &property = properties[k][i][j];
        property = bddtrue;
        for (unsigned k = 0; k < LOG_N; k++)
          property &= ((j >> k) & 1) ? bdd_ithvar(I + LOG_N * k + k) : bdd_nithvar(I + LOG_N * k + k);
      }
    }
    I += LOG_N * M;
  }

  // здесь должны быть ограничения
  // они описаны формально при рассмотрении задачи Эйнштейна
  // вывод результатов
  auto &task = properties[0][0][0];
  out.open("out.txt");
  unsigned satcount = (unsigned) bdd_satcount(task);
  out << satcount << " solutions:\n";
  if (satcount)
    bdd_allsat(task, fun);
  bdd_done();
  return 0;
}

char var[N_VAR];
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
