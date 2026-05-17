import math
s = int(input())
minn = 2147483647
for b in range(1, math.ceil(math.sqrt(s))):
  if s % b == 0:
    a = s // b
    minn = min(minn, 2 * (a + b))
print(minn)