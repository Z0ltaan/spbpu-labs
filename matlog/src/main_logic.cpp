#include "main_logic.hpp"
#include <buddy/bdd.h>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <vector>
#include "constraints.hpp"

static constexpr int N = 9; // число объектов;
static constexpr int M = 4; // число свойств

// static constexpr int n1 = 7;
// static constexpr int n2 = 4;
// static constexpr int n3 = 2;
// static constexpr int n4 = 6;

static const int LOG_N = std::ceil(std::log2(N));
static const int N_VAR = N * M * LOG_N; // число булевых переменных
static const size_t N_SQRT = static_cast< size_t >(std::sqrt(N));
static std::vector< char > var(N_VAR);

using props_array = course::props_array_t< bdd, N, M >;

static void
fun(char* varset, int size);
static std::ofstream out;

static int
var_index(int i, int k, int t)
{
  return i * M * LOG_N + k * LOG_N + t;
}

static bdd
bdd_eql(const bdd& a, const bdd& b)
{
  return (a & b) | (!a & !b);
}

static void
evaluate_constraint(bdd& func,
                    const props_array& props,
                    const course::constraint1& c)
{
  func &= props.at(c.prop).at(c.obj).at(c.value);
}

// TODO: rewrite this shit so .at is used instead of []
static void
evaluate_constraint(bdd& func,
                    const props_array& props,
                    const course::constraint2& c)
{
  bdd to_return = bddtrue;
  auto upper_bound = props.front().size();
  for (size_t i = 0; i < upper_bound; ++i)
  {
    func &= bdd_eql(props[c.first.prop][i][c.first.value],
                    props[c.second.prop][i][c.second.value]);
  }
  // return to_return;
}

static bdd
evaluate_constraint(const props_array& props, const course::constraint3& c)
{
  bdd to_return = bddtrue;
  auto upper_bound = props.front().size();
  if (c.side == 'u')
  {
    for (size_t i = 0; i < upper_bound; ++i)
    {
      if (i / N_SQRT == 0)
      {
        to_return &= !props[c.lhs.prop][i][c.lhs.value];
      }
      else if (i % N_SQRT == 0)
      {
        to_return &= bdd_eql(props[c.lhs.prop][i][c.lhs.value],
                             props[c.rhs.prop][i - 1][c.rhs.value]);
      }
      else
      {
        to_return &= bdd_eql(props[c.lhs.prop][i][c.lhs.value],
                             props[c.rhs.prop][i - N_SQRT - 1][c.rhs.value]);
      }
    }
  }
  else if (c.side == 'd')
  {
    for (size_t i = 0; i < upper_bound; ++i)
    {
      if (i / N_SQRT >= N_SQRT - 1)
      {
        to_return &= !props[c.lhs.prop][i][c.lhs.value];
      }
      else if (i % N_SQRT == 0)
      {
        to_return &=
          bdd_eql(props[c.lhs.prop][i][c.lhs.value],
                  props[c.rhs.prop][i + 2 * N_SQRT - 1][c.rhs.value]);
      }
      else
      {
        to_return &= bdd_eql(props[c.lhs.prop][i][c.lhs.value],
                             props[c.rhs.prop][i + N_SQRT - 1][c.rhs.value]);
      }
    }
  }

  return to_return;
}

static void
evaluate_constraint4(bdd& func,
                     const props_array& props,
                     const course::constraint3& c)
{
  func &= evaluate_constraint(props, c) |
          evaluate_constraint(
            props,
            course::constraint3{
              .side = c.side == 'd' ? 'u' : 'd', .lhs = c.lhs, .rhs = c.rhs });
}

int
course::main_logic(int argc, char** argv)
{
  static_assert((N > 0 && M > 0) && (M < N), "check your N and M values");

  props_array properties;
  std::cout << LOG_N << ' ' << N_VAR << ' ' << '\n';

  size_t nodenum = 10000000;
  size_t cache_size = 100000;
  bdd_init(nodenum, cache_size);
  bdd_setvarnum(N_VAR);

  // NOTE: Set function p(k, i, j)
  for (int i = 0; i < N; ++i)
  {
    for (int j = 0; j < N; ++j)
    {
      for (int k = 0; k < M; ++k)
      {
        bdd& tmp = properties[k][i][j];
        tmp = bddtrue;
        for (int t = 0; t < LOG_N; ++t)
        {
          auto idx = var_index(i, k, t);
          tmp &= ((j >> t) & 1) ? bdd_ithvar(idx) : bdd_nithvar(idx);
        }
      }
    }
  }

  // NOTE: init F
  bdd task = bddtrue;

  // NOTE: 1st level constraints
  task &= properties[0][0][0];
  task &= properties[0][1][1];
  task &= properties[0][2][2];
  task &= properties[0][3][3];
  task &= properties[0][4][4];
  task &= properties[0][5][5];
  task &= properties[0][6][6];

  // NOTE: 2nd level constraint
  // task &=
  evaluate_constraint(
    task,
    properties,
    constraint2{ { .prop = 2, .value = 1 }, { .prop = 3, .value = 1 } });
  // // task &=
  evaluate_constraint(
    task,
    properties,
    constraint2{ { .prop = 3, .value = 0 }, { .prop = 1, .value = 0 } });
  // // task &=
  evaluate_constraint(
    task,
    properties,
    constraint2{ { .prop = 3, .value = 2 }, { .prop = 1, .value = 2 } });
  // // task &=
  evaluate_constraint(
    task,
    properties,
    constraint2{ { .prop = 3, .value = 4 }, { .prop = 2, .value = 4 } });

  // NOTE: 3rd level constraint
  task &= evaluate_constraint(properties,
                              constraint3{ .side = 'u',
                                           .lhs = { .prop = 3, .value = 8 },
                                           .rhs = { .prop = 0, .value = 4 } });
  task &= evaluate_constraint(properties,
                              constraint3{ .side = 'd',
                                           .lhs = { .prop = 2, .value = 5 },
                                           .rhs = { .prop = 3, .value = 7 } });
  task &= properties[0][7][7];
  task &= properties[0][8][8];

  task &= properties[1][0][0];
  task &= properties[1][1][1];
  task &= properties[1][2][2];
  task &= properties[1][3][3];
  task &= properties[1][4][4];
  task &= properties[1][5][5];
  task &= properties[1][6][6];
  task &= properties[1][7][7];
  task &= properties[1][8][8];

  task &= properties[2][0][0];
  task &= properties[2][1][1];
  task &= properties[2][2][2];
  task &= properties[2][3][3];

  // NOTE: do not need
  // task &= properties[2][4][4];
  // task &= properties[2][5][5];
  // task &= properties[2][6][6];
  // task &= properties[2][7][7];
  // task &= properties[2][8][8];
  //
  // task &= properties[3][0][0];
  // task &= properties[3][1][1];
  // task &= properties[3][2][2];
  // task &= properties[3][3][3];
  // task &= properties[3][4][4];
  // task &= properties[3][5][5];
  // task &= properties[3][6][6];
  // task &= properties[3][7][7];
  // task &= properties[3][8][8];

  // NOTE: 4th level constraints
  evaluate_constraint4(task,
                       properties,
                       course::constraint3{ .side = 'u',
                                            .lhs = { .prop = 1, .value = 6 },
                                            .rhs = { .prop = 2, .value = 5 } });
  evaluate_constraint4(task,
                       properties,
                       course::constraint3{ .side = 'u',
                                            .lhs = { .prop = 0, .value = 7 },
                                            .rhs = { .prop = 2, .value = 3 } });

  evaluate_constraint4(task,
                       properties,
                       course::constraint3{ .side = 'u',
                                            .lhs = { .prop = 0, .value = 7 },
                                            .rhs = { .prop = 2, .value = 3 } });
  evaluate_constraint4(task,
                       properties,
                       course::constraint3{ .side = 'd',
                                            .lhs = { .prop = 1, .value = 0 },
                                            .rhs = { .prop = 2, .value = 5 } });
  evaluate_constraint4(task,
                       properties,
                       course::constraint3{ .side = 'd',
                                            .lhs = { .prop = 1, .value = 3 },
                                            .rhs = { .prop = 2, .value = 8 } });
  evaluate_constraint4(task,
                       properties,
                       course::constraint3{ .side = 'u',
                                            .lhs = { .prop = 1, .value = 6 },
                                            .rhs = { .prop = 0, .value = 5 } });
  evaluate_constraint4(task,
                       properties,
                       course::constraint3{ .side = 'u',
                                            .lhs = { .prop = 2, .value = 4 },
                                            .rhs = { .prop = 2, .value = 0 } });

  // NOTE: lvl 5 constraints
  for (int k = 0; k < M; ++k)
  {
    for (int j = 0; j < N; ++j)
    {
      for (int i1 = 0; i1 < N; i1++)
      {
        for (int i2 = i1 + 1; i2 < N; i2++)
        {
          task &= bdd_imp(properties[k][i1][j], !properties[k][i2][j]);
        }
      }
    }
  }

  // NOTE: lvl 6 constraints
  for (int i = 0; i < N; ++i)
  {
    for (int j = 0; j < M; ++j)
    {
      bdd tmp_false = bddfalse;
      for (int k = 0; k < N; ++k)
      {
        tmp_false |= properties[j][i][k];
      }
      task &= tmp_false;
    }
  }

  double satcount = bdd_satcount(task);
  if (satcount)
  {
    out.open("out.txt");
    if (!out.is_open()) {
      return -1;
    }
    out << satcount << " solutions:\n";
    bdd_allsat(task, fun);
  }
  bdd_done();
  return 0;
}

static void
print()
{
  for (unsigned i = 0; i < N; i++)
  {
    out << i << ": ";
    for (unsigned j = 0; j < M; j++)
    {
      // J вычисляется как смещение i-й строки и j-го столбца
      unsigned J = (i * M + j) * LOG_N;
      unsigned num = 0;
      for (unsigned k = 0; k < LOG_N; k++)
      {
        // Извлекаем бит и сдвигаем его на позицию k
        if (var[J + k])
        {
          num |= (1u << k);
        }
      }
      out << num << " ";
    }
    out << '\n';
  }
  out << "\n\n";
}

static void
build(char* varset, unsigned total_bits)
{
  // Стек состояний: [текущий_индекс_бита, текущее_действие]
  // действие 0: пробуем положить 0, действие 1: пробуем положить 1, действие 2:
  // выход
  struct State
  {
    unsigned idx;
    int step;
  };

  std::vector< State > stack;
  stack.reserve(total_bits + 1);
  stack.push_back({ 0, 0 });

  while (!stack.empty())
  {
    State& curr = stack.back();
    unsigned i = curr.idx;

    if (i == total_bits)
    {
      print();
      stack.pop_back();
      continue;
    }

    if (varset[i] >= 0)
    {
      // Если бит фиксирован, просто записываем его и идем дальше
      if (curr.step == 0)
      {
        var[i] = varset[i];
        curr.step = 1; // Помечаем, что ветка отработана
        stack.push_back({ i + 1, 0 });
      }
      else
      {
        stack.pop_back();
      }
    }
    else
    {
      // Если бит свободный (-1), перебираем 0 и 1
      if (curr.step == 0)
      {
        var[i] = 0;
        curr.step = 1;
        stack.push_back({ i + 1, 0 });
      }
      else if (curr.step == 1)
      {
        var[i] = 1;
        curr.step = 2;
        stack.push_back({ i + 1, 0 });
      }
      else
      {
        stack.pop_back();
      }
    }
  }
}

static void
fun(char* varset, int size)
{
  build(varset, size);
}
