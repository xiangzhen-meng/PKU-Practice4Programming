import math
n, x, y = (int(x) for x in input().split())
print(max(0, n - math.ceil(y / x)))