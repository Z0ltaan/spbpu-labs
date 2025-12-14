#include <stdio.h>
#define LEN 10

int main() {
  int num[10] = {522, 513, 250, 252, 17, 45, 61, 72, 0, 1};
  int tmp[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int sz = 1;
  int low = 2;
  int mid = 3;
  int high = 4;

  int i = 0, j, k;
  int r1, r2;

  sz = 1;
  while (sz < LEN) {
    low = 0;
    while (low < LEN - sz) {
      mid = low + sz - 1;
      r1 = low + 2 * sz - 1;
      r2 = LEN - 1;
      if (r1 < r2)
        high = r1;
      else
        high = r2;

      i = low;
      j = mid + 1;
      for (k = 0; k < LEN; ++k)
        if (i == mid + 1) {
          tmp[k] = num[j];
          j = j + 1;
        } else if (j == high + 1) {
          tmp[k] = num[i];
          i = i + 1;
        } else if (num[i] < num[j]) {
          tmp[k] = num[i];
          i = i + 1;
        } else {
          tmp[k] = num[j];
          j = j + 1;
        }

      // copying
      i = low;
      while (i <= high) {
        num[i] = tmp[i - low];
        i = i + 1;
      }
      // endCopying

      low += 2 * sz;
    }
    sz *= 2;
  }

  for (i = 0; i < LEN; ++i)
    printf("%d ", num[i]);
  printf("\n");
}
