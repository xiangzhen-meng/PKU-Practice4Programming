inp = input().split()
x, y = inp[0], inp[1]
x, y = int(x), int(y)
if abs(x) <= 1 and abs(y) <= 1:
  print("yes")
else:
  print("no")