#include <stddef.h>
#include <stdio.h>
#include <string.h>

size_t size1 = 6;
int outArr1[6] = {0,0,0,0,0,0};
int* merge(int arr[], int left, int mid, int right) {

  int i, j, k;
  int leftSize = mid - left + 1;
  int rightSize = right - mid;

  int leftArr[leftSize], rightArr[rightSize];
  memcpy(outArr1, arr, size1 * sizeof(int));

  for (i = 0; i < leftSize; ++i) leftArr[i] = arr[left + i];
  for (j = 0; j < rightSize; ++j) rightArr[j] = arr[mid + 1 + j];

  i = 0;
  j = 0;
  k = left;

  while (i < leftSize && j < rightSize)
    if (leftArr[i] <= rightArr[j])
      outArr1[k++] = leftArr[i++];
    else
      outArr1[k++] = rightArr[j++];

  while (i < leftSize) outArr1[k++] = leftArr[i++];
  while (j < rightSize) outArr1[k++] = rightArr[j++];

  return outArr1;
}

size_t size = 6;
int outArr[6] = {0,0,0, 0,0,0};
int* mergeSort(int arr[], int left, int right) {
  memcpy(outArr, arr, size * sizeof(int));

  if (left < right) {
    int mid = left + (right - left) / 2;
    memcpy(outArr, mergeSort(outArr, left, mid), size * sizeof(int));
    memcpy(outArr, mergeSort(outArr, mid + 1, right), size * sizeof(int));
    memcpy(outArr, merge(outArr, left, mid, right), size * sizeof(int));
  }

  return outArr;
}

int main() {
  int arr[] = { 12, 11, 13, 5, 124, 1 };
  int size = sizeof(arr) / sizeof(arr[0]);

  int *outarr = mergeSort(arr, 0, size - 1);

  for (int i = 0; i < size; ++i) printf("%d ", outarr[i]);
  printf("\n");

  return 0;
}
