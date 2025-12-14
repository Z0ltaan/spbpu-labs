#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

class MergeSort {
public:
  MergeSort();
  ~MergeSort();
  void merge_sort();
  void show();

private:
  int len;
  int *num;
  void merge(const int &, const int &, const int &);
};

MergeSort::MergeSort() : len(10), num(nullptr) {
  num = new int[len];
  num[0] = 100;
  num[1] = 10;
  num[2] = 60;
  num[3] = 0;
  num[4] = 1;
  num[5] = 234;
  num[6] = 70;
  num[7] = 100;
  num[8] = 34;
  num[9] = 67;
}

MergeSort::~MergeSort() { delete[] num; }

void MergeSort::merge(const int &low, const int &mid, const int &hi) {
  int sorted[10]{};
  int i = low, j = mid + 1, k = 0;

  for (k = 0; k < len; ++k) {
    if (i == mid + 1)
      sorted[k] = num[j++];
    else if (j == hi + 1)
      sorted[k] = num[i++];
    else if (num[i] < num[j])
      sorted[k] = num[i++];
    else
      sorted[k] = num[j++];
  }

  for (int i = low; i <= hi; ++i)
    num[i] = sorted[i - low];
}

// Bottom-up method reduces recursive function overhead
void MergeSort::merge_sort() {
  for (int sz = 1; sz < len; sz *= 2)
    for (int low = 0; low < len - sz; low += 2 * sz)
      merge(low, low + sz - 1, min(low + 2 * sz - 1, len - 1));
  // Every sorted portion is of size "sz" (only the last portion may be smaller)
}

void MergeSort::show() {
  cout << "Sorted:\n";
  for (int i = 0; i < len; ++i)
    cout << num[i] << ' ';
  cout << '\n';
}

int main() {
  MergeSort arr;
  arr.merge_sort();
  arr.show();
  return 0;
}
