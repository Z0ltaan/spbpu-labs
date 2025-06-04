#include <climits>
#include <iostream>
#define INF (INT_MAX)
#define N 8

namespace lab
{
  void printMtx(int mtx[N][N])
  {
    for (int i = 0; i < N; ++i)
    {
      for (int j = 0; j < N; ++j)
      {
        if (mtx[i][j] == INF)
          std::cout << "∞ ";
        else
          std::cout << mtx[i][j] << ' ';
      }
      std::cout << '\n';
    }
  }

  void printMatrix(int w[N][N], bool selected[N])
  {
    std::cout << "   ";
    for (int i = 0; i < N; ++i)
    {
      std::cout << i + 1 << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i < N; ++i)
    {
      std::cout << i + 1 << " ";
      for (int j = 0; j < N; ++j)
      {
        if (selected[i] && selected[j])
        {
          std::cout << "*" << " ";
        }
        else if (w[i][j] == 0)
        {
          std::cout << "0" << " ";
        }
        else
        {
          if (w[i][j] == INF)
            std::cout << "∞ ";
          else
            std::cout << w[i][j] << " ";
        }
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }

  void doPrim(int w[N][N])
  {
    int n_edge = 0;
    bool selected[N] = {false};
    int total_weight = 0;

    selected[0] = true;

    std::cout << "Initial matrix:" << std::endl;
    printMatrix(w, selected);

    while (n_edge < N - 1)
    {
      int min = INT_MAX;
      int x = 0, y = 0;

      for (int i = 0; i < N; i++)
      {
        if (selected[i])
        {
          for (int j = 0; j < N; j++)
          {
            if (!selected[j] && w[i][j] && w[i][j] < min)
            {
              min = w[i][j];
              x = i;
              y = j;
            }
          }
        }
      }

      selected[y] = true;
      total_weight += min;
      n_edge++;

      std::cout << "Step " << n_edge << ": Add edge "
          << x << " -> " << y << " (weight " << min << ")" << std::endl;
      printMatrix(w, selected);
    }

    std::cout << "Total weight: " << total_weight << std::endl;
  }

  void printState(int k, int D[N], int P[N], bool visited[N], int m)
  {
    using namespace std;
    cout << "k = " << k << endl;
    cout << "L: ";
    for (int i = 0; i < N; i++)
    {
      if (D[i] == INT_MAX)
      {
        cout << "∞ ";
      }
      else
      {
        cout << D[i] << " ";
      }
    }
    cout << endl;

    cout << "P: ";
    for (int i = 0; i < N; i++)
    {
      cout << P[i] << " ";
    }
    cout << endl;

    cout << "C: ";
    for (int i = 0; i < N; i++)
    {
      cout << (visited[i] ? "1 " : "0 ");
    }
    cout << endl;

    cout << "m(k): " << m << endl;
    cout << "---------------------" << endl;
  }

  int Dijkstra(int w[N][N], int st, int end)
  {
    bool visited[N] = {false};
    int D[N];
    int P[N] = {0}; // Вектор предшествующих вершин

    for (int i = 0; i < N; i++)
    {
      D[i] = w[st][i];
    }
    D[st] = 0;
    visited[st] = true;

    printState(0, D, P, visited, st);

    for (int k = 1; k < N; k++)
    {
      int u = -1;
      int min_dist = INT_MAX;
      for (int j = 0; j < N; j++)
      {
        if (!visited[j] && D[j] < min_dist)
        {
          min_dist = D[j];
          u = j;
        }
      }
      if (u == -1 || u == end)
      {
        break;
      }

      printState(k, D, P, visited, u);
      visited[u] = true;

      for (int j = 0; j < N; j++)
      {
        if (!visited[j] && w[u][j] != INT_MAX && D[u] != INT_MAX && (D[u] + w[u][j] < D[j]))
        {
          D[j] = D[u] + w[u][j];
          P[j] = u;
        }
      }
    }

    return D[end];
  }

  void doFW(int L[N][N], int P[N][N])
  {
    int T = 0;
    for (int k = 0; k < N; k++)
    {
      for (int i = 0; i < N; i++)
        if ((i != k) && (L[i][k] != INF))
          for (int j = 0; j < N; j++)
            if ((j != k) && (j != i) && (L[k][j] != INF))
            {
              T = L[i][k] + L[k][j];
              if ((L[i][j] == INF) || (T < L[i][j]))
              {
                L[i][j] = T;
                P[i][j] = P[i][k];
              }
            }
      std::cout << "L " << k << '\n';
      printMtx(L);
      std::cout << "P\n";
      printMtx(P);
    }
  }
}

int main()
{
  using namespace lab;

  int L[N][N]{
    {0, 11, INF, INF, INF, INF, 10, INF},
    {11, 0, 31, INF, INF, 25, INF, 21},
    {INF, 31, 0, INF, 3, 16, INF, INF},
    {INF, INF, INF, 0, 20, 14, INF, INF},
    {INF, INF, 3, 20, 0, INF, INF, 22},
    {INF, 25, 16, 14, INF, 0, 19, 1},
    {10, INF, INF, INF, INF, 19, 0, 5},
    {INF, 21, INF, INF, 22, 1, 5, 0}
  };

  int L2[N][N]{
    {0, 11, INF, INF, INF, INF, 10, INF},
    {11, 0, 31, INF, INF, 25, INF, 21},
    {INF, 31, 0, INF, 3, 16, INF, INF},
    {INF, INF, INF, 0, 20, 14, INF, INF},
    {INF, INF, 3, 20, 0, INF, INF, 22},
    {INF, 25, 16, 14, INF, 0, 19, 1},
    {10, INF, INF, INF, INF, 19, 0, 5},
    {INF, 21, INF, INF, 22, 1, 5, 0}
  };

  int P[N][N]
  {
    {0, 2, 0, 0, 0, 0, 7, 0},
    {1, 0, 3, 0, 0, 6, 0, 8},
    {0, 2, 0, 0, 5, 6, 0, 0},
    {0, 0, 0, 0, 5, 6, 0, 0},
    {0, 0, 3, 4, 0, 0, 0, 8},
    {0, 2, 3, 4, 0, 0, 7, 8},
    {1, 0, 0, 0, 0, 6, 0, 8},
    {0, 2, 0, 0, 5, 6, 7, 0}
  };

  doFW(L, P);

  std::cout << Dijkstra(L2, 3, 6) << '\n';

  doPrim(L2);
}
