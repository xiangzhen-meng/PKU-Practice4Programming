import math
n, x, y = (int(x) for x in input().split())
print(n - math.ceil(y / x))