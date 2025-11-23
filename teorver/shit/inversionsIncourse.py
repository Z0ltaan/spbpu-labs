sym = "xyxyyxyyxyxyxyyxxxxyxxxxyxyxyyyyyxyxyxxy"

inversions = 0

for i in range(0,39):
  if sym[i] == "y":
    for k in range(i, 39):
      if sym[k] == "x":
        inversions += 1

print(inversions)
